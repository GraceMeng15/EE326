/*
 * wifi.h
 *
 * Created: 2025/2/10 14:30:41
 *  Author: 17713
 */ 
/*
 * wifi.h
 *
 * Created: 2/5/2023 10:15:35 AM
 *  Author: seanp
 */ 

#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>

// Test LED Pins, for breadboard debugging
#define LED_PIN		PIO_PA19_IDX
#define LED_PIN2	PIO_PA22_IDX

// ESP32 GPIO pins (on the ESP32 NOT the MCU!)
#define ESP_COMM_GPIO					21	// Control line to MCU (ON ESP32)
#define ESP_NET_GPIO					22	// Control line to MCU (ON ESP32)
#define ESP_CLIENT_GPIO					32	// Control line to MCU (ON ESP32)


// ESP32 LED Indicator pins (on the ESP32 NOT the MCU!)
#define ESP_PROV_LED					26	// Indicator LED Pin (ON ESP32): Chip is in provisioning mode
#define ESP_NET_LED						25	// Indicator LED Pin (ON ESP32): Chip is connected to the internet
#define ESP_CLIENT_LED					27	// Indicator LED Pin (ON ESP32): Chip has at least one client connected


//DECLARE WiFi variables here
uint8_t in_byte;
uint32_t ul_id, ul_mask;
char* comm;
uint8_t cnt;

///* Status block. */
//struct status_block_t {
	//uint32_t ul_total_block_number; 	/** Number of data blocks. */
	//uint32_t ul_total_command_number;	/** Number of SPI commands (including data blocks). */
	//uint32_t ul_cmd_list[NB_STATUS_CMD]; 	/** Command list. */
//};
//
///* SPI clock configuration. */
//static const uint32_t gs_ul_clock_configurations[] =
//{ 500000, 1000000, 2000000, 5000000 };
	
//static uint32_t gs_ul_spi_clock; /* SPI clock setting (Hz). */
//static uint32_t gs_ul_spi_cmd; /* Current SPI return code. */
//static uint32_t gs_ul_spi_state; /* Current SPI state. */
//
///* 64 bytes data buffer for SPI transfer and receive. */
////static uint8_t gs_uc_spi_buffer[COMM_BUFFER_SIZE]; /* Transfer buffer. */
//static uint8_t *gs_puc_transfer_buffer; /* Pointer to transfer buffer. */
//static uint32_t gs_ul_transfer_index; /* Transfer buffer index. */
//static uint32_t gs_ul_transfer_length; /* Transfer buffer length. */
//static struct status_block_t gs_spi_status; /* SPI Status. */
//static uint32_t gs_ul_test_block_number;

// Interrupt and Control Line Variable Initialization
volatile bool reading_wifi_flag; // Flag for test/success confirmation
volatile bool provisioning_flag; // Flag for provisioning button
volatile char input_line_wifi[1000]; // UART Data buffer
volatile uint32_t input_pos_wifi; // UART Data index variable

//DECLARE WiFi functions here
void wifi_usart_handler(void);
void wifi_provision_handler(uint32_t ul_id, uint32_t ul_mask);

void configure_usart(void);
void configure_usart_wifi(void);
void configure_wifi_comm_pin(void);
void configure_wifi_provision_pin(void);

void process_incoming_byte_wifi(uint8_t in_byte);
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void process_data_wifi(void);
void write_wifi_command(char* comm, uint8_t cnt);

//SPI-like functions
void wifi_spi_handler(void);
void configure_spi(void);
void spi_peripheral_initialize(void);
void prepare_spi_transfer(void);
void write_image_to_web(void);


#endif /* WIFI_H_ */