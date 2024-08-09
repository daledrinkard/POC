/*
 * testbench_timers.c
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#include "testbench_api.h"



/*
 *    Callback functions
 */
void testbench_timer0_cb(timer_callback_args_t *p_arg)
{

    switch(p_arg->event) {
        case TIMER_EVENT_CYCLE_END:                     ///< Requested timer delay has expired or timer has wrapped around
//        case TIMER_EVENT_CREST = TIMER_EVENT_CYCLE_END: ///< Timer crest event (counter is at a maximum, triangle-wave PWM only)
            TB.event_flag |= TB_EVENT_TIMER0;
            break;
        case TIMER_EVENT_CAPTURE_A:                     ///< A capture has occurred on signal A
        case TIMER_EVENT_CAPTURE_B:                     ///< A capture has occurred on signal B
        case TIMER_EVENT_TROUGH:                        ///< Timer trough event (counter is 0: triangle-wave PWM only
        case TIMER_EVENT_COMPARE_A:                     ///< A compare has occurred on signal A
        case TIMER_EVENT_COMPARE_B:                     ///< A compare has occurred on signal B
        case TIMER_EVENT_COMPARE_C:                     ///< A compare has occurred on signal C
        case TIMER_EVENT_COMPARE_D:                     ///< A compare has occurred on signal D
        case TIMER_EVENT_COMPARE_E:                     ///< A compare has occurred on signal E
        case TIMER_EVENT_COMPARE_F:                     ///< A compare has occurred on signal F
        case TIMER_EVENT_DEAD_TIME:
            break;
        default: assert(0);
    }
}

