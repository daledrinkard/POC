/*
 * testbench.c
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#include "testbench_api.h"
static void testbench_Open(void*);


// Global structures
testbench_t TB = {.open = testbench_Open };

// static declarations







static void testbench_Open(void *data)
{
    TB.driver = (driver_packet_t* ) data;
    TB.driver->p_tmr0[0]->p_api->open(TB.driver->p_tmr0[0]->p_ctrl,TB.driver->p_tmr0[0]->p_cfg);
}
