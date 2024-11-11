/*
 * testbench_api.h
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#ifndef TESTBENCH_TESTBENCH_API_H_
#define TESTBENCH_TESTBENCH_API_H_

//#include "common_data.h"
#include "r_display_api.h"
#include "r_timer_api.h"
#include "r_external_irq_api.h"

#include "testbench.h"
#include "Console/console_api.h"
typedef struct timer_object_s {

}timer_object_t;
typedef struct driver_packet_s {
    display_instance_t* p_GLCDC;
    void* p_Console;
    timer_instance_t *p_tmr0[4];
    external_irq_instance_t *p_SW[2];
    bsp_leds_t *board_leds;
}driver_packet_t;
typedef struct capture_buffer_s {
    uint32_t *data;
    uint16_t size;
    uint16_t w_idx;
}capture_buffer_t;
typedef struct testbench_s {
    uint32_t state;
    uint32_t mode;
    driver_packet_t *driver;
    console12_args_t *console_args;

    char *rxbuffer;
    uint16_t rxbuffer_size;
    uint16_t rxbuffer_idx;
    char *txbuffer;
    uint16_t txbuffer_size;
    uint16_t txbuffer_idx;
    capture_buffer_t vsync_data;
    capture_buffer_t async_data;
    capture_buffer_t animate_data;
    capture_buffer_t render_data;
    volatile uint32_t event_flag;
    uint32_t event_mask;
    uint32_t idle_cycles;
    uint32_t sec_counts;
    uint32_t frame_count;
    uint32_t animate_count;
    volatile uint32_t* p_activeframe;
    uint32_t framesize;
    fsp_err_t (*open)(void*);
    fsp_err_t (*start)(void);
    void (*led)(uint8_t,bsp_io_level_t);

}testbench_t;

extern testbench_t TB;

#endif /* TESTBENCH_TESTBENCH_API_H_ */
