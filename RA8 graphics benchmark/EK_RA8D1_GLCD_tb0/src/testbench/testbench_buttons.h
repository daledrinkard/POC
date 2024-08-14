/*
 * testbench_buttons.h
 *
 *  Created on: Aug 11, 2024
 *      Author: daled
 */

#ifndef TESTBENCH_TESTBENCH_BUTTONS_H_
#define TESTBENCH_TESTBENCH_BUTTONS_H_

#include "common_data.h"
void SW1_callback(external_irq_callback_args_t *p_args);
void SW2_callback(external_irq_callback_args_t *p_args);
extern external_irq_callback_args_t SW1_cb_args,SW2_cb_args;

#endif /* TESTBENCH_TESTBENCH_BUTTONS_H_ */
