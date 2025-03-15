/*
 * camera.h
 *
 * Created: 2025/2/10 14:30:53
 *  Author: Grace & Xiaoyi
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <asf.h>
#include "ov2640.h"

//DECLARE camera variables here
#define end_cap ((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) == PIO_PCIMR_RXBUFF)
uint32_t ul_id, ul_mask, ul_size; // For interrupts
uint8_t *uc_buf;    //buffer to store the image in
Pio *p_pio;
uint8_t find_len_success;

volatile uint32_t image_size;
volatile uint32_t image_start;
volatile uint32_t image_end;
volatile uint8_t image_started;
volatile uint8_t image_ended;
volatile uint8_t g_p_uc_cap_dest_buf[100000];

volatile uint8_t image_sent_flag;


// Required Camera Functions
void vsync_handler(uint32_t ul_id, uint32_t ul_mask);
void init_vsync_interrupts(void);
void configure_twi(void);
void pio_capture_init(Pio *p_pio, uint32_t ul_id);
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size);
void init_camera(void);
void configure_camera(void);
uint8_t start_capture(void);
uint8_t find_image_len(void);

#endif /* CAMERA_H_ */