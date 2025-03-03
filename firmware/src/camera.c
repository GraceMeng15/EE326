/*
 * camera.c
 *
 * Created: 2025/2/10 14:32:27
 *  Author: Grace & Xiaoyi
 */

#include "camera.h"

// Intialize LCD display.
#define IMAGE_WIDTH		(320UL)
#define IMAGE_HEIGHT	(240UL)
#define TWI_CLK			(400000UL) 					// TWI clock frequency in Hz (400KHz)

uint16_t g_us_cap_line = (IMAGE_WIDTH * 2);			// (IMAGE_WIDTH *2 ) because ov7740 use YUV422 format in color mode
static volatile uint32_t g_ul_vsync_flag = false;	// Vsync signal information (true if it's triggered and false otherwise)


// Camera Functions

void vsync_handler(uint32_t ul_id, uint32_t ul_mask){
	// Handler for vertical synchronisation using by the OV7740 image sensor
	unused(ul_id);
	unused(ul_mask);

	g_ul_vsync_flag = true;
}

void init_vsync_interrupts(void){
	// Initialize PIO interrupt handler, see PIO definition in conf_board.h
	pio_handler_set(OV7740_VSYNC_PIO, OV7740_VSYNC_ID, OV7740_VSYNC_MASK,
			OV7740_VSYNC_TYPE, vsync_handler);	

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV7740_VSYNC_ID);
}

void configure_twi(void){
	// Two Wire Interface
	twi_options_t opt;
	pmc_enable_periph_clk(ID_BOARD_TWI);	// Enable TWI peripheral

	// Init TWI peripheral
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);

	// Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
}

void init_camera(void){
	//Configuration of camera pins, camera clock (XCLK), and
	//calling the configure_twi() function.
	configure_twi();
	init_vsync_interrupts();							// Init Vsync handler
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);	// Init PIO capture

	// Enable XCLCK
	pmc_enable_pllbck(7, 0x1, 1); /* PLLA work at 96 Mhz */ // PA17 is xclck signal
	
	// Init PCK1 to work at 24 Mhz
	// 96/4=24 Mhz
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {}	
	
	// ov7740 Initialization */
	while (ov_init(BOARD_TWI) == 1) {}

}

void configure_camera(void){
	// Don't touch: From Firmware PDF
	ov_configure(BOARD_TWI, JPEG_INIT);
	ov_configure(BOARD_TWI, YUV422);
	ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_320x240);
	
	//let the camera adapt to environment
	delay_ms(2000);
}

/**
 * \brief Initialize PIO capture for the OV7740 image sensor communication.
 *
 * \param p_pio PIO instance to be configured in PIO capture mode.
 * \param ul_id Corresponding PIO ID.
 */
void pio_capture_init(Pio *p_pio, uint32_t ul_id){
	///* Enable peripheral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable PIO capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable RXBUFF interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);
}

/**
 * \brief Capture OV7740 data to a buffer.
 *
 * \param p_pio PIO instance which will capture data from OV7740 iamge sensor.
 * \param p_uc_buf Buffer address where captured data must be stored.
 * \param ul_size Data frame size.
 */
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size){
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
	} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
	} else {
		return 0;
	}
}

uint8_t start_capture(void){
	//Captures an image after a rising edge of VSYNC, and gets image	
	//length. Returns 1 on success (i.e. a nonzero image length), 0 on error.
	
	
	pio_enable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);	// Enable vsync interrupt*/
	
	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait g_vsync_flag = 1 before start process */
	while (!g_ul_vsync_flag)
	{
		delay_ms(10);		// !!! TRY WITHOUT
	}
	
	/* Disable vsync interrupt*/
	pio_disable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

	/* Enable pio capture*/
	pio_capture_enable(OV7740_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory thanks to PDC
	 * feature */
	pio_capture_to_buffer(OV7740_DATA_BUS_PIO, g_p_uc_cap_dest_buf, 25000);		// 100000 >> 2 = 25000

	/* Wait end of capture*/
	/*
	while (!((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) == PIO_PCIMR_RXBUFF)) {
	}*/
	while (!end_cap)
	{
		delay_ms(10);		// !!! TRY WITHOUT
	}

	// #define end_cap ((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) == PIO_PCIMR_RXBUFF)
	// while (!end_cap)
	// {
	// 	delay_ms(10);		// !!! TRY WITHOUT
	// }
	// and
	
	// while (!((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
	// 		PIO_PCIMR_RXBUFF)) {
	// }
	

	/* Disable pio capture*/
	pio_capture_disable(OV7740_DATA_BUS_PIO);

	/* Reset vsync flag*/
	g_ul_vsync_flag = false;
	
	/* Check Size  */
	len_success = 0;
	find_image_len();
	//return len_success;
	
}

uint8_t find_image_len(void){
	//Finds image length based on JPEG protocol. Returns 1 on success
	//(i.e. able to find ?end of image? and ?start of image? markers),
	// 0 on error.
	
	//iterate through the buffer and look for start and end of image markers
	//start is 0xFFD8; end is 0xFFD9
	image_size = 0;
	image_started = 0;
	image_ended = 0;
	uint8_t byte;
	uint8_t next_byte;
	uint8_t complete = 0;
	
	for (uint32_t i = 0; i < 100000; ++i){
		
		//look for start of image
		//#ifndef next_byte
		//
			//next_byte = g_p_uc_cap_dest_buf[i+1];
		//#endif
		byte = g_p_uc_cap_dest_buf[i];
		next_byte = g_p_uc_cap_dest_buf[i+1];
		
		if (byte == 0xff && next_byte == 0xd8) {
			image_started = 1;
			image_ended = 0;
			start_pos = i;
		}
		else if (byte == 0xff && next_byte == 0xd9 && image_started == 1) {
			image_ended = 1;
			end_pos = i+1;
			len_success = 1;
			break;
		}
		//if (image_started && image_ended) {
			//len_success = 1;
			//break; 			//add +1 to the length counter
		//}
		image_size += image_started;
		}
		
		
	
	//return 0 if start or end markers weren't encountered, 1 otherwise
	
}