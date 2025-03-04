/*
 * wifi.c
 *
 * Created: 2025/2/10 14:30:22
 *  Author: 17713
 */

#include "wifi.h"
#include "camera.h"
#include "ov2640.h"
#include "timer_interface.h"

// ----------------------------------------------------------------------------
// SPI Code from spi_example

// static uint32_t gs_ul_spi_clock = 500000;	/* SPI clock setting (Hz). */

static uint32_t gs_ul_spi_cmd = RC_SYN;	/* Current SPI return code. */
static uint32_t gs_ul_spi_state = 0;	/* Current SPI state. */

/* 64 bytes data buffer for SPI transfer and receive. */
volatile uint8_t *gs_puc_transfer_buffer; 	/* Pointer to transfer buffer. */
volatile uint32_t gs_ul_transfer_index; 	/* Transfer buffer index. */
volatile uint32_t gs_ul_transfer_length; 	/* Transfer buffer length. */

volatile uint32_t times_through_buffer; //

// UART Communication and Control Line Variables
volatile uint32_t received_byte_wifi = 0;
volatile bool new_rx_wifi = false;
volatile uint32_t input_pos_wifi = 0;
volatile bool command_flag = false;

//DEFINE WiFi functions here

char buff_storage[50];

void wifi_usart_handler(void)
{
	uint32_t ul_status;

	/* Read USART Status. */
	ul_status = usart_get_status(WIFI_USART);

	if (ul_status & US_CSR_RXBUFF)
	{
		usart_read(WIFI_USART, &received_byte_wifi);
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}

void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	process_data_wifi();
	uint32_t ind = 0;
	while (ind < 1000)
	{
		input_line_wifi[ind] = 0;
		ind += 1;
	}
	input_pos_wifi = 0;
	command_flag = true;
}

void process_incoming_byte_wifi(uint8_t in_byte)
{
	input_line_wifi[input_pos_wifi++] = in_byte;
}

void process_data_wifi()
{
	if (strstr(input_line_wifi, "SUCCESS"))
	{
		reading_wifi_flag = true;
	}
}

void configure_usart(void)
{	
	const sam_usart_opt_t usart_console_settings =
	{
		WIFI_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		0 // IrDA mode only
	};

	/* Enable the peripheral clock in the PMC. */
	sysclk_enable_peripheral_clock(WIFI_ID_USART);
	//could be pmc_enable_peripheral_clock?)

	/* Configure USART in serial mode. */
	usart_init_rs232(WIFI_USART, &usart_console_settings, sysclk_get_peripheral_hz());

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);

	/* Enable the receiver and transmitter. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(USART_IRQn);
	
	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);
}

void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_NUM, WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_NUM);
}


void write_wifi_command(char* comm, uint8_t cnt) //TODO: Implement timeout mechanism
{
	/* Writes a command (comm) to the ESP32, and waits either for an acknowledgment or a timeout. The timeout can be
	 created by setting the global variable counts to zero, which will automatically increment every second, and waiting
	while counts < cnt. */
	counts = 0;
	command_flag = false;
	ioport_set_pin_level(LED_PIN2, 1);
	char wifi_buff[100];
	sprintf (wifi_buff, "%s\r\n", comm);
	usart_write_line(WIFI_USART, wifi_buff);
	while (true)
	{
		if (counts >= cnt)
		{
			break;
		}
		else if (command_flag)
		{
			break;
		}
	}
	if (command_flag)
	{
		command_flag = false;
	}
}


void wifi_provision_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	// Set provisioning flag to true
	provisioning_flag = true;
	//ioport_toggle_pin_level(LED_PIN);
	//write_wifi_command("test",10);
	
}


void configure_wifi_provision_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_SETUP_BUTTON_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_SETUP_BUTTON_PIO, WIFI_SETUP_BUTTON_ID, WIFI_SETUP_BUTTON_NUM, WIFI_SETUP_BUTTON_ATTR, wifi_provision_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_SETUP_BUTTON_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_SETUP_BUTTON_PIO, WIFI_SETUP_BUTTON_NUM);
}

void wifi_spi_handler(void){
	//// Handler for peripheral mode interrupts on SPI bus. When the
	//// ESP32 SPI controller requests data, this interrupt should 
	//// send one byte of the image at a time.
	
	uint32_t new_cmd = 0;
	static uint16_t data;
	uint8_t uc_pcs;


	//remove later
	//gs_ul_transfer_index++;
	//times_through_buffer++;



	//if status register says "ready" and Receive Data Register Full
	if (spi_read_status(SPI_SLAVE_BASE) & SPI_SR_RDRF) {
			
		spi_read(SPI_SLAVE_BASE, &data, &uc_pcs);
		times_through_buffer++;

		
		//gs_puc_transfer_buffer[gs_ul_transfer_index] = data;
			
		if (gs_ul_transfer_length--) {
			//transfer one byte of image
			spi_write(SPI_SLAVE_BASE, g_p_uc_cap_dest_buf[gs_ul_transfer_index++], 0, 0);
			//spi_write(SPI_SLAVE_BASE, (g_p_uc_cap_dest_buf[gs_ul_transfer_index]), 0, 0);
			//gs_ul_transfer_index++;

		}
		
		//if (gs_ul_transfer_length == 0) {
			//last_byte_sent = g_p_uc_cap_dest_buf[gs_ul_transfer_index];
			//second_to_last_byte_sent = g_p_uc_cap_dest_buf[gs_ul_transfer_index - 1];
			//third_to_last_byte_sent = g_p_uc_cap_dest_buf[gs_ul_transfer_index - 2];
			//last_byte_index = gs_ul_transfer_index;
		//}
		//else if (gs_ul_transfer_index == start_pos + 1) {
			//first_byte_sent =g_p_uc_cap_dest_buf[gs_ul_transfer_index-1];
			////second_byte_sent = g_p_uc_cap_dest_buf[gs_ul_transfer_index];
		//}
		//else if (gs_ul_transfer_index == start_pos + 2) {
			//second_byte_sent = g_p_uc_cap_dest_buf[gs_ul_transfer_index-1];
		//}
		
	}
}

void configure_spi(void){
		
	/* Configure SPI interrupts for slave only. */
	NVIC_DisableIRQ(SPI_IRQn);
	NVIC_ClearPendingIRQ(SPI_IRQn);
	NVIC_SetPriority(SPI_IRQn, 0);
	NVIC_EnableIRQ(SPI_IRQn);
	
	//Configuration of SPI port used to send images to the ESP32.
	

	
	spi_peripheral_initialize();
	
	
	/* Start waiting command. */
	prepare_spi_transfer();
}

void spi_peripheral_initialize(void){
	//Initialize the SPI port as a peripheral (slave) device.
	spi_enable_clock(SPI_SLAVE_BASE);
	spi_disable(SPI_SLAVE_BASE);
	spi_reset(SPI_SLAVE_BASE);
	spi_set_slave_mode(SPI_SLAVE_BASE);
	spi_disable_mode_fault_detect(SPI_SLAVE_BASE);
	spi_set_peripheral_chip_select_value(SPI_SLAVE_BASE, SPI_CHIP_PCS);
	spi_set_clock_polarity(SPI_SLAVE_BASE, SPI_CHIP_SEL, SPI_CLK_POLARITY);
	spi_set_clock_phase(SPI_SLAVE_BASE, SPI_CHIP_SEL, SPI_CLK_PHASE);
	spi_set_bits_per_transfer(SPI_SLAVE_BASE, SPI_CHIP_SEL, SPI_CSR_BITS_8_BIT);
	spi_enable_interrupt(SPI_SLAVE_BASE, SPI_IER_RDRF);	
	spi_enable(SPI_SLAVE_BASE);
	
}

void prepare_spi_transfer(void){
	//Set necessary parameters to prepare for SPI transfer.
	gs_puc_transfer_buffer = g_p_uc_cap_dest_buf;
	gs_ul_transfer_index = start_pos;
	// gs_ul_transfer_length = image_size;
	gs_ul_transfer_length = image_size+1;
	image_sent_flag = 0;
	
	
	//char* spi_test[1000];
	// sprintf(spi_test,"Test");
	
	
	//spi_write(SPI_SLAVE_BASE, gs_puc_transfer_buffer[gs_ul_transfer_index], 0,0);
	//sprintf(gs_puc_transfer_buffer,)
	
}

void write_image_to_web(void){
	 //Writes an image from the SAM4S8B to the ESP32. If the length of the image is zero 
	 //(i.e. the image is not valid), return. Otherwise, follow this protocol
	 //(illustrated in Appendix C):
	
	if (image_size == 0)
	{
		return;
	}
	
	//first_byte_sent = gs_puc_transfer_buffer[gs_ul_transfer_index];
	//second_byte_sent = gs_puc_transfer_buffer[gs_ul_transfer_index + 1];
	//first_byte_unsent = gs_puc_transfer_buffer[gs_ul_transfer_index -1];
	//last_byte_sent = gs_puc_transfer_buffer[gs_ul_transfer_index + gs_ul_transfer_length];

	//Configure the SPI interface to be ready for a transfer by setting its parameters appropriately.
	//times_through_buffer = 0;
	prepare_spi_transfer();

	//Issue the command �image_transfer xxxx? where xxxx is replaced by the length of the
	//image you want to transfer.
	char* command_buffer[100];
		//image_size = 1000;
		//sprintf(command_buffer, "image_transfer %d", image_size); // Full image transfer command
	sprintf(command_buffer, "image_transfer %d", image_size+3); // Test image transfer command
	write_wifi_command(command_buffer, 1);
	//while (!image_sent_flag){}
	//delay_ms(100);
	 
	//The ESP32 will then set the �command complete?pin low and begin transferring the image
	//over SPI. //After the image is done sending, the ESP32 will set the �command complete?pin high. The
	//MCU should sense this and then move on.
}