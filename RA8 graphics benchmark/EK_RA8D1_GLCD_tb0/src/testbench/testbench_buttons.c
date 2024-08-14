/*
 * testbench_buttons.c
 *
 *  Created on: Aug 11, 2024
 *      Author: daled
 */


/*
 *    press buttons callback
 */
#include "common_data.h"
#include "testbench_api.h"
#include "testbench_buttons.h"
external_irq_callback_args_t SW1_cb_args,SW2_cb_args;
/* Callback function */
void SW1_callback(external_irq_callback_args_t *p_args)
{
    TB.event_flag |= TB_EVENT_SW1;
}
/* Callback function */
void SW2_callback(external_irq_callback_args_t *p_args)
{
    TB.event_flag |= TB_EVENT_SW2;
}

