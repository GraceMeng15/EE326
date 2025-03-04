/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	// TWI Pin config
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
		
	// Data Bus and Camera Utility Pin config
	gpio_configure_pin(OV_RST_GPIO, OV_RST_TYPE);
	gpio_configure_pin(OV_HREF_GPIO, OV_HREF_FLAGS);
	gpio_configure_pin(OV_VSYNC_GPIO, OV_VSYNC_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D8, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D9, OV_DATA_BUS_FLAGS);
		
	// XCLK
	gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);
		
	// SPI
	gpio_configure_pin(SPI_MISO_GPIO, SPI_MISO_FLAGS);
	gpio_configure_pin(SPI_MOSI_GPIO, SPI_MOSI_FLAGS);
	gpio_configure_pin(SPI_SPCK_GPIO, SPI_SPCK_FLAGS);
	gpio_configure_pin(SPI_NPCS0_GPIO, SPI_NPCS0_FLAGS);
		
	// USART
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);

	// IO Directions and Init Values for LED 1+2
	ioport_set_pin_dir(LED_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED_PIN, false);
	ioport_set_pin_dir(LED_PIN2, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(LED_PIN2, false);

	// Control line Pins - MASK+CLIENT
	ioport_set_pin_dir(WIFI_NET_MASK, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(WIFI_CLIENT_PIN_MASK, IOPORT_DIR_INPUT);

	// Pushbutton  - SETUP+RESET
	ioport_set_pin_mode(WIFI_SETUP_BUTTON_MASK, IOPORT_MODE_PULLUP);
	ioport_set_pin_dir(WIFI_RESET_MASK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_mode(WIFI_RESET_MASK, IOPORT_MODE_PULLUP);
}
