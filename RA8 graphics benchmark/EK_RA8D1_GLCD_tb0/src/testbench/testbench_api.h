/*
 * testbench_api.h
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#ifndef TESTBENCH_TESTBENCH_API_H_
#define TESTBENCH_TESTBENCH_API_H_

#include "r_display_api.h"
#include "r_timer_api.h"
#include "testbench.h"
typedef struct driver_packet_s {
    display_instance_t* p_GLCDC;
    void* p_Console;
    timer_instance_t *p_tmr0[4];
}driver_packet_t;
typedef struct testbench_s {
    driver_packet_t *driver;
    void (*open)(void*);
}testbench_t;

extern testbench_t TB;

#endif /* TESTBENCH_TESTBENCH_API_H_ */
