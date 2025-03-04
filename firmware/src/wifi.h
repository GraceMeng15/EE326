/*
 * wifi.h
 *
 * Created: 2025/2/10 14:30:41
 *  Author: 17713
 */ 

#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>

// moved to conf_board
// // Test LED Pins, for breadboard debugging
// #define LED_PIN		PIO_PA19_IDX
// #define LED_PIN2	PIO_PA22_IDX

// Control Pins Declaration - PA9, PA10, PB3 // change name
#define ESP_COMM_GPIO					21	// COMM PIN   - connect to MCU | PA9
#define ESP_NET_GPIO					22	// NET PIN    - connect to MCU | PA10
#define ESP_CLIENT_GPIO					32	// CLIENT PIN - connect to MCU | PB3


// ESP32 LED Indicator pins (on the ESP32 NOT the MCU!)
#define ESP_PROV_LED					26	// Indicator LED Pin (ON ESP32): Chip is in provisioning mode
#define ESP_NET_LED						25	// Indicator LED Pin (ON ESP32): Chip is connected to the internet
#define ESP_CLIENT_LED					27	// Indicator LED Pin (ON ESP32): Chip has at least one client connected


//DECLARE WiFi variables here
uint8_t in_byte, cnt;
uint32_t ul_id, ul_mask;
char* comm;
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