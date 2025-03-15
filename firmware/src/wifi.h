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

// Interrupt and Control Line Variable Initialization
volatile bool reading_wifi_flag;
volatile bool provisioning_flag;
volatile char wifi_buffer_in[1000];
volatile uint32_t wifi_buffer_in_index;

uint32_t ul_id, ul_mask;

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

// SPIs
void wifi_spi_handler(void);
void configure_spi(void);
void spi_peripheral_initialize(void);
void prepare_spi_transfer(void);

void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_web(void);

#endif /* WIFI_H_ */