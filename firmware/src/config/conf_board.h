/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

//#define CONF_BOARD_UART_CONSOLE /** Enable Com Port. */
//#define CONSOLE_UART_ID          ID_UART0 /** Usart Hw ID used by the console (UART0). */

//below: conf_board from the OV7740 image sensor example
//
//#define CONF_BOARD_OV7740_IMAGE_SENSOR /** Enable OV7740 image sensor. */

//
//
/******************************* SPI definition *********************************/

/** Conf_board. */
#define CONF_BOARD_SPI /** SPI MACRO definition */
#define CONF_BOARD_SPI_NPCS0 /** SPI slave select MACRO definition */
#define SPI_ID          ID_SPI  /** Spi Hw ID . */
#define SPI_SLAVE_BASE       SPI /** SPI base address for SPI slave mode */
#define wifi_spi_handler	SPI_Handler

/** Initialization of pins **/
#define PIN_SPI_MISO                   {PIO_PA12A_MISO, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT} /** SPI MISO pin. */
#define PIN_SPI_MOSI                   {PIO_PA13A_MOSI, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT} /** SPI MOSI pin. */
#define PIN_SPI_SPCK                   {PIO_PA14A_SPCK, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT} /** SPI SPCK pin. */
#define PIN_SPI_NPCS0                  {PIO_PA11A_NPCS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT} /** SPI chip select pin. */

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO                  (PIO_PA12_IDX)
#define SPI_MISO_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_MISO_MASK                  PIO_PA12
#define SPI_MISO_PIO                   PIOA
#define SPI_MISO_ID                    ID_PIOA
#define SPI_MISO_TYPE                  PIO_PERIPH_A
#define SPI_MISO_ATTR                  PIO_DEFAULT

/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO                  (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_MOSI_MASK                  PIO_PA13
#define SPI_MOSI_PIO                   PIOA
#define SPI_MOSI_ID                    ID_PIOA
#define SPI_MOSI_TYPE                  PIO_PERIPH_A
#define SPI_MOSI_ATTR                  PIO_DEFAULT

/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO                  (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_SPCK_MASK                  PIO_PA14
#define SPI_SPCK_PIO                   PIOA
#define SPI_SPCK_ID                    ID_PIOA
#define SPI_SPCK_TYPE                  PIO_PERIPH_A
#define SPI_SPCK_ATTR                  PIO_DEFAULT

/** SPI chip select 0 pin definition. */
#define SPI_NPCS0_GPIO                 (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS0_MASK                 PIO_PA11
#define SPI_NPCS0_PIO                  PIOA
#define SPI_NPCS0_ID                   ID_PIOA
#define SPI_NPCS0_TYPE                 PIO_PERIPH_A
#define SPI_NPCS0_ATTR                 PIO_DEFAULT


//Wifi buffer config		From Ilya In-Class Example: Wifi input buffer configuration
//volatile uint8_t USART_INPUT_MAX_LEN = 1000;
#define MAX_LENGTH 1000 //image length for buffer

#define SPI_CHIP_SEL 0 /* Chip select. */
#define SPI_CHIP_PCS spi_get_pcs(SPI_CHIP_SEL)
#define SPI_CLK_POLARITY 0 /* Clock polarity. */
#define SPI_CLK_PHASE 0 /* Clock phase. */
#define SPI_DLYBS 0x40 /* Delay before SPCK. */
#define SPI_DLYBCT 0x10 /* Delay between consecutive transfers. */

/* SPI slave states for this example. */
#define SLAVE_STATE_IDLE           0
#define SLAVE_STATE_TEST           1
#define SLAVE_STATE_DATA           2
#define SLAVE_STATE_STATUS_ENTRY   3
#define SLAVE_STATE_STATUS         4
#define SLAVE_STATE_END            5

/* SPI example commands for this example. */
#define CMD_TEST     0x10101010 /* slave test state, begin to return RC_RDY. */
#define CMD_DATA     0x29380000 /* Slave data state, begin to return last data block. */
#define CMD_STATUS   0x68390384 /* Slave status state, begin to return RC_RDY + RC_STATUS. */
#define CMD_END      0x68390484 /* Slave idle state, begin to return RC_SYN. */
#define RC_SYN       0x55AA55AA /* General return value. */
#define RC_RDY       0x12345678 /* Ready status. */

#define CMD_DATA_MSK 0xFFFF0000 /* Slave data mask. */
#define DATA_BLOCK_MSK 0x0000FFFF /* Slave data block mask. */
#define NB_STATUS_CMD   20 /* Number of commands logged in status. */
#define NUM_SPCK_CONFIGURATIONS 4 /* Number of SPI clock configurations. */
#define COMM_BUFFER_SIZE   64 /* SPI Communicate buffer size. */
#define UART_BAUDRATE      115200 /* UART baudrate. */
#define MAX_DATA_BLOCK_NUMBER  4 /* Data block number. */
#define MAX_RETRY    4 /* Max retry times. */
#define SPI_BAUDRATE		800000 /* SPi baudrate, used in UART command to ESP32 to set the SPI baudrate (wow crazy) */


/******************************* TWI definition*********************************/

/** TWI0 pins */
#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0
#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn
#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT} /** TWI0 data pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT} /** TWI0 clock pin */

/** TWI0 Data pins definition */
#define TWI0_DATA_GPIO                 PIO_PA3_IDX
#define TWI0_DATA_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_DATA_MASK                 PIO_PA3
#define TWI0_DATA_PIO                  PIOA
#define TWI0_DATA_ID                   ID_PIOA
#define TWI0_DATA_TYPE                 PIO_PERIPH_A
#define TWI0_DATA_ATTR                 PIO_DEFAULT

/** TWI0 clock pins definition */
#define TWI0_CLK_GPIO                  PIO_PA4_IDX
#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_CLK_MASK                  PIO_PA4
#define TWI0_CLK_PIO                   PIOA
#define TWI0_CLK_ID                    ID_PIOA
#define TWI0_CLK_TYPE                  PIO_PERIPH_A
#define TWI0_CLK_ATTR                  PIO_DEFAULT

/** Conf_board */
#define CONF_BOARD_TWI0 /** Configure TWI0 pins (for OV7740  communications). */
#define CONF_BOARD_PCK1 /** Configure PCK1 pins (for OV7740  communications). */

/* TWI board defines. */
#define ID_BOARD_TWI                   ID_TWI0
#define BOARD_TWI                      TWI0
#define BOARD_TWI_IRQn                 TWI0_IRQn

/** PCK1 */
#define PIN_PCK1                       (PIO_PA17_IDX)
#define PIN_PCK1_FLAGS                 (PIO_PERIPH_B | PIO_DEFAULT)

/******************************* OV definition*********************************/


/* Image sensor board defines. */
// Image sensor Power pin.
//#define OV_POWER_PIO                   OV_SW_OVT_PIO
//#define OV_POWER_MASK                  OV_SW_OVT_MASK
// Image sensor VSYNC pin.
#define OV7740_VSYNC_PIO			   OV_VSYNC_PIO
#define OV7740_VSYNC_ID				   OV_VSYNC_ID
#define OV7740_VSYNC_MASK              OV_VSYNC_MASK
#define OV7740_VSYNC_TYPE              OV_VSYNC_TYPE

// Image sensor data pin.
#define OV7740_DATA_BUS_PIO            OV_DATA_BUS_PIO
#define OV7740_DATA_BUS_ID             OV_DATA_BUS_ID


// Board-level Pin Definitions
/** OV_SW_OVT pin definition */
//#define OV_SW_OVT_GPIO                 PIO_PC10_IDX
//#define OV_SW_OVT_FLAGS                (PIO_OUTPUT_1 | PIO_DEFAULT)
//#define OV_SW_OVT_MASK                 PIO_PC10
//#define OV_SW_OVT_PIO                  PIOC
//#define OV_SW_OVT_ID                   ID_PIOC
//#define OV_SW_OVT_TYPE                 PIO_OUTPUT_1

/** OV_RST pin definition */
#define OV_RST_GPIO                    PIO_PA20_IDX
#define OV_RST_FLAGS                   (PIO_OUTPUT_1 | PIO_DEFAULT)
#define OV_RST_MASK                    PIO_PC15
#define OV_RST_PIO                     PIOC
#define OV_RST_ID                      ID_PIOC
#define OV_RST_TYPE                    PIO_OUTPUT_1

/** OV_FSIN definition */
//#define OV_FSIN_GPIO                   PIO_PA21_IDX
//#define OV_FSIN_FLAGS                  (PIO_OUTPUT_0 | PIO_DEFAULT)
//#define OV_FSIN_MASK                   PIO_PA21
//#define OV_FSIN_PIO                    PIOA
//#define OV_FSIN_ID                     ID_PIOA
//#define OV_FSIN_TYPE                   PIO_OUTPUT_0

/** OV_HSYNC pin definition */
#define OV_HSYNC_GPIO                  PIO_PA16_IDX
#define OV_HSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_HSYNC_MASK                  PIO_PA16
#define OV_HSYNC_PIO                   PIOA
#define OV_HSYNC_ID                    ID_PIOA
#define OV_HSYNC_TYPE                  PIO_PULLUP

/** OV_VSYNC pin definition */
#define OV_VSYNC_GPIO                  PIO_PA15_IDX
#define OV_VSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_VSYNC_MASK                  PIO_PA15
#define OV_VSYNC_PIO                   PIOA
#define OV_VSYNC_ID                    ID_PIOA
#define OV_VSYNC_TYPE                  PIO_PULLUP

/** OV Data Bus pins */
#define OV_DATA_BUS_D2                 PIO_PA24_IDX
#define OV_DATA_BUS_D3                 PIO_PA25_IDX
#define OV_DATA_BUS_D4                 PIO_PA26_IDX
#define OV_DATA_BUS_D5                 PIO_PA27_IDX
#define OV_DATA_BUS_D6                 PIO_PA28_IDX
#define OV_DATA_BUS_D7                 PIO_PA29_IDX
#define OV_DATA_BUS_D8                 PIO_PA30_IDX
#define OV_DATA_BUS_D9                 PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT

// RST Pin (this is only driven high)
//#define OV2640_RST_MASK					PIO_PA20_IDX



/******************************* USART definition*********************************/


#define WIFI_ID_USART				ID_USART0
#define WIFI_USART					USART0
#define WIFI_USART_BAUDRATE		115200
#define wifi_usart_handler			USART0_Handler
#define USART_IRQn					USART0_IRQn

#define ALL_INTERRUPT_MASK  0xffffffff

/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

// Wifi COMM Pin definitions
#define WIFI_COMM_PIN_NUM			PIO_PA9
#define WIFI_COMM_PIO				PIOA
#define WIFI_COMM_ID				ID_PIOA
#define WIFI_COMM_MASK				PIO_PA9_IDX
#define WIFI_COMM_ATTR				PIO_IT_RISE_EDGE

// Wifi NET Pin definitions
#define WIFI_NET_PIN_NUM			PIO_PA10
#define WIFI_NET_PIO				PIOA
#define WIFI_NET_ID					ID_PIOA
#define WIFI_NET_MASK				PIO_PA10_IDX
//#define WIFI_NET_ATTR				PIO_IT_RISE_EDGE

// CLIENT_PIN config
#define WIFI_CLIENT_PIN_MASK			PIO_PB3_IDX
#define WIFI_CLIENT_PIN_ID				ID_PIOB
#define WIFI_CLIENT_PIN_PIO				PIOB
#define WIFI_CLIENT_PIN_NUM				PIO_PB3
//#define WIFI_CLIENT_PIN_ATTR			PIO_IT_RISE_EDGE //Double-check this

// RESET ESP32 Control pin
#define WIFI_RESET_MASK					PIO_PB0_IDX
#define WIFI_RESET_ID					ID_PIOB
#define WIFI_RESET_PIO					PIOB
#define WIFI_RESET_NUM					PIO_PB0

// WIFI_SETUP Pushbutton pin
#define WIFI_SETUP_BUTTON_MASK			PIO_PB14_IDX
#define WIFI_SETUP_BUTTON_ID            ID_PIOB
#define WIFI_SETUP_BUTTON_PIO           PIOB
#define WIFI_SETUP_BUTTON_NUM			PIO_PB14
#define WIFI_SETUP_BUTTON_ATTR          (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE)



#endif // CONF_BOARD_H