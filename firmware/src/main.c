#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"
#include "wifi.h"
#include "camera.h"
#include "ov2640.h"
#include "timer_interface.h"

char* buff[100];

int main (void)
{
	// Board / Clock Initializations
	sysclk_init();
	wdt_disable(WDT);
	ioport_init();
	board_init();
	
	// Configurations and Timer
	configure_tc();
	configure_usart();
	configure_spi();
	configure_wifi_comm_pin();
	configure_wifi_provision_pin();
	 
	// Camera Initializations
	init_camera();
	configure_camera();

	// Reset Wifi
	ioport_set_pin_level(WIFI_RESET_MASK, false);
	delay_ms(100);
	ioport_set_pin_level(WIFI_RESET_MASK, true);
	delay_ms(5000);
	
	// Set SPI Baud Rate
	sprintf(buff, "set spi_baud %d", SPI_BAUDRATE);
	write_wifi_command(buff, 2);

	// Send ESP32 indicator LED config commands
	sprintf (buff, "set wlan_gpio %d", ESP_NET_LED);
	write_wifi_command(buff, 2);
	sprintf (buff, "set websocket_gpio %d", ESP_CLIENT_LED);
	write_wifi_command(buff, 2);
	sprintf (buff, "set ap_gpio %d", ESP_PROV_LED);
	write_wifi_command(buff, 2);

	// Wifi Board Control Line Pin Config
	sprintf (buff, "set comm_gpio %d", ESP_COMM_GPIO);
	write_wifi_command(buff, 2);
	sprintf (buff, "set net_gpio %d", ESP_NET_GPIO);
	write_wifi_command(buff, 2);
	sprintf (buff, "set clients_gpio %d", ESP_CLIENT_GPIO);
	write_wifi_command(buff, 2);

	reading_wifi_flag = false;
	provisioning_flag = false;

	// Wait for Network Connection ACK
	while (!wifi_ready)
	{
		if (provisioning_flag)  // if Wifi Setup button is pressed
		{
			write_wifi_command("provision", 1);
			provisioning_flag = false;
			write_wifi_command("get mac", 1);
		}
	}

	delay_ms(100);
	write_wifi_command("test", 10);
	delay_ms(1000);	// test to find optimal time?
	
	while (!reading_wifi_flag)
	{
		// Reset wifi chip
		ioport_set_pin_level(WIFI_RESET_MASK,false);
		delay_ms(100);
		ioport_set_pin_level(WIFI_RESET_MASK,true);
		delay_ms(5000);
		write_wifi_command("test", 10);
		delay_ms(1000);
	
	}

	// Start Main Loop
	while (true)
	{
		if (provisioning_flag) // if Wifi Setup button is pressed
		{
			write_wifi_command("provision", 1);
			provisioning_flag = false;
		}
		else if (capture_ready)
		{
			start_capture();
			if (find_len_success)
			{
				write_image_to_web();
			}
		}
	}
}