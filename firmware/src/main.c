/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"

//user-defined files
#include "wifi.h"
#include "camera.h"
#include "ov2640.h"
#include "timer_interface.h"

char* pbuf_test[50];

#define capture_ready ioport_get_pin_level(WIFI_CLIENT_PIN_MASK) //&& ioport_get_pin_level(WIFI_NET_MASK)
int main (void)
{
	//Initialize clock and board definitions.
	sysclk_init();
	wdt_disable(WDT);
	ioport_init();
	board_init();
	
	//* Configure and start the Timer. (Look in the ?timer interface? functions.)
	configure_tc();
	
	configure_usart();
	configure_spi();
	configure_wifi_comm_pin();
	configure_wifi_provision_pin();
	 
	//* Initialize and configure the camera.
	init_camera();
	configure_camera();
	 
	 //* Reset the WiFi and wait for it to connect to a network. While waiting, make sure to listen
	 //for the ?provision? pin.	 
	 
	 //* Send ?test? to the WiFi module and wait for a response of ?SUCCESS?. If you do not receive
	 //it, wait 10 seconds, reset the WiFi module, and try again.
	 
	
	// Camera Test Capture:
	//start_capture(); 
	 
	
	// Thomas' Main Code Loop Code

	ioport_set_pin_dir(LED_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED_PIN, false);
	ioport_set_pin_dir(LED_PIN2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED_PIN2, false);


	// Control line pin ioport config
	//ioport_set_pin_dir(WIFI_COMM_MASK,IOPORT_DIR_INPUT);
	ioport_set_pin_dir(WIFI_NET_MASK, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(WIFI_CLIENT_PIN_MASK, IOPORT_DIR_INPUT);

	// Pushbutton pin ioport config
	//ioport_set_pin_dir(WIFI_SETUP_BUTTON_MASK, IOPORT_DIR_INPUT); // Maybe
	ioport_set_pin_mode(WIFI_SETUP_BUTTON_MASK, IOPORT_MODE_PULLUP);
	ioport_set_pin_dir(WIFI_RESET_MASK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(WIFI_RESET_MASK, IOPORT_MODE_PULLUP);

	//// RST Pin Management - for camera
	//ioport_set_pin_dir(OV2640_RST_MASK,IOPORT_DIR_OUTPUT);
	//ioport_set_pin_level(OV2640_RST_MASK,true);

	// Reset wifi chip
	ioport_set_pin_level(WIFI_RESET_MASK,false);
	delay_ms(200);
	ioport_set_pin_level(WIFI_RESET_MASK,true);
	delay_ms(200);


	// Set Control Line pins on ESP32
	char* buff[100];
	sprintf (buff, "set comm_gpio %d", ESP_COMM_GPIO);
	delay_ms(10);
	write_wifi_command(buff,2);
	sprintf (buff, "set net_gpio %d", ESP_NET_GPIO);
	delay_ms(10);
	write_wifi_command(buff,2);
	sprintf (buff, "set clients_gpio %d", ESP_CLIENT_GPIO);
	delay_ms(10);
	write_wifi_command(buff,2);

	// Send ESP32 indicator LED config commands
	sprintf (buff, "set wlan_gpio %d", ESP_NET_LED);
	write_wifi_command(buff,2);
	sprintf (buff, "set websocket_gpio %d", ESP_CLIENT_LED);
	write_wifi_command(buff,2);
	sprintf (buff, "set ap_gpio %d", ESP_PROV_LED);
	write_wifi_command(buff,2);
	
	// Set SPI Baud Rate
	sprintf(buff, "set spi_baud %d", SPI_BAUDRATE);
	write_wifi_command(buff,2);

	//write_wifi_command("test",10);

	reading_wifi_flag = false;
	provisioning_flag = false;


	//// Loop while waiting for Network connection confirmation
	while (!ioport_get_pin_level(WIFI_NET_MASK))	{
		//ioport_set_pin_level(LED_PIN,true);
		//delay_ms(500);
		// Check if WIFI_SETUP_BUTTON was pressed and send provision command if so
		if (provisioning_flag) {
			//ioport_set_pin_level(LED_PIN2,true);
			//delay_ms(500);
			write_wifi_command("provision",1);
			provisioning_flag = false;
			//ioport_set_pin_level(LED_PIN2,false);
			//delay_ms(500);
			write_wifi_command("get mac",1);
		}
		
		//ioport_set_pin_level(LED_PIN,false);
		//delay_ms(500);
	}

	//delay_ms(5000);
	//ioport_set_pin_level(LED_PIN,true);
	write_wifi_command("test",10);
	delay_s(8);
	
	while (!reading_wifi_flag)	{
		
		// Reset wifi chip
		ioport_set_pin_level(WIFI_RESET_MASK,false);
		delay_ms(100);
		ioport_set_pin_level(WIFI_RESET_MASK,true);
		delay_ms(5000);
	
		//ioport_set_pin_level(LED_PIN,true);
		//delay_ms(1000);
		//ioport_set_pin_level(LED_PIN,false);
		//delay_ms(1000);
	
		// Send UART Test command and wait 10 seconds
		write_wifi_command("test",10);
		delay_s(8);
	
	}

	// Start Main Loop
	while (true)
	{
		// Check if WIFI_SETUP_BUTTON was pressed and send provision command if so
		//ioport_set_pin_level(LED_PIN,false);
		//ioport_set_pin_level(LED_PIN,true);
		//delay_ms(500);
		//ioport_set_pin_level(LED_PIN,false);
		//delay_ms(500);
		if (provisioning_flag)
		{
			write_wifi_command("provision", 1);
			provisioning_flag = false;
		}
		else if (capture_ready)
		{
			start_capture();
			if (len_success)
			{
				write_image_to_web();
			}
		}
	}
	
}