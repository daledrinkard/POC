/*
 * testbench_timers.h
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#ifndef TESTBENCH_TESTBENCH_TIMERS_H_
#define TESTBENCH_TESTBENCH_TIMERS_H_

#include "testbench_api.h"
extern const timer_cfg_t OneSecondPeriod_cfg;
extern timer_callback_args_t timer0_cb_args;
void testbench_timer0_cb(timer_callback_args_t *p_arg);
void testbench_timer2_cb(timer_callback_args_t *p_arg);

#endif /* TESTBENCH_TESTBENCH_TIMERS_H_ */
