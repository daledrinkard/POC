/*
 * testbench_timers.c
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#include "testbench_api.h"
const timer_cfg_t OneSecondPeriod_cfg =
{ .mode = TIMER_MODE_PERIODIC,
/* Actual period: 1 seconds. Actual duty: 50%. */.period_counts = (uint32_t) 0x7270e00,
  .duty_cycle_counts = 0x3938700, .source_div = (timer_source_div_t) 0, .channel = 0, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = &g_timer0_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW)
    .cycle_end_irq       = VECTOR_NUMBER_GPT0_COUNTER_OVERFLOW,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
