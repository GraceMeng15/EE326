/*
 * wifi.h
 *
 * Created: 2025/2/10 14:30:41
 *  Author: Grace & Xiaoyi
 */ 

#ifndef WIFI_H_
#define WIFI_H_
#include <asf.h>
#include <string.h>

// Control Pins Declaration - PA9, PA10, PB3 // change name
#define ESP_COMM_GPIO					21	// COMM PIN   - connect to MCU | PA9
#define ESP_NET_GPIO					22	// NET PIN    - connect to MCU | PA10
#define ESP_CLIENT_GPIO					32	// CLIENT PIN - connect to MCU | PB3


// WIFI BOARD LEDs
#define ESP_NET_LED						25  // LED2
#define ESP_PROV_LED					26  // LED3
#define ESP_CLIENT_LED					27  // LED4



uint32_t ul_id, ul_mask;
//not used?
// uint8_t in_byte, cnt;
// char* comm;
//uint8_t cnt;

// Interrupt and Control Line Variable Initialization
volatile bool reading_wifi_flag; // Flag for test/success confirmation
volatile bool provisioning_flag; // Flag for provisioning button
volatile char input_line_wifi[1000]; // UART Data buffer
volatile uint32_t input_pos_wifi; // UART Data index variable

// WiFi Function Declarations from PDF
void wifi_usart_handler(void);
void process_incoming_byte_wifi(uint8_t in_byte);
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void process_data_wifi(void);
void wifi_provision_handler(uint32_t ul_id, uint32_t ul_mask);

// USARTs
void configure_usart(void);
void configure_usart_wifi(void);
void configure_wifi_comm_pin(void);
void configure_wifi_provision_pin(void);

//SPIs
void wifi_spi_handler(void);
void configure_spi(void);
void spi_peripheral_initialize(void);
void prepare_spi_transfer(void);

void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_web(void);

#endif /* WIFI_H_ */