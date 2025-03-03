/*
 * camera.h
 *
 * Created: 2025/2/10 14:30:53
 *  Author: 17713
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <asf.h>
#include "ov2640.h"

uint32_t ul_id, ul_mask, ul_size;   // For interrupts
uint8_t *uc_buf;        // CHANGE TO PREALLOCATED
Pio *p_pio;

// JPEG Detecting Variables
uint8_t found_markers;
volatile uint32_t image_size;
volatile uint32_t start_pos;
volatile uint32_t end_pos;
volatile uint8_t image_started;
volatile uint8_t image_ended;

volatile uint8_t g_p_uc_cap_dest_buf[100000];
volatile uint8_t first_byte_sent;
volatile uint8_t last_byte_sent;
volatile uint8_t second_to_last_byte_sent;
volatile uint8_t third_to_last_byte_sent;
volatile uint8_t second_byte_sent;
volatile uint8_t first_byte_unsent;
volatile uint8_t third_byte_sent;
volatile uint32_t last_byte_index;
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