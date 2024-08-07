/*
 * testbench.c
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#include "testbench_api.h"
extern const bsp_leds_t g_bsp_leds;
/*
 *    API functions
 */
static fsp_err_t testbench_Open(void*);
static fsp_err_t testbench_Start(void);


// Global structures
testbench_t TB = {.open = testbench_Open,
                  .start = testbench_Start,
                  .idle_cycles = 0,
                  .sec_counts = 0
                 };

// static declarations
timer_callback_args_t timer0_cb_args;


void testbench_timer0_cb(timer_callback_args_t *p_arg);
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
static fsp_err_t testbench_Start(void)
{
    uint32_t events;
    uint32_t emask;
    while(1)
    {
        events = TB.event_flag;
        emask = 0x80000000;
        while(emask){
            switch(events & emask) {
                case 0:
                    while( (emask != 0x00000000) && ((emask & events) == 0))
                    {
                        emask = (emask == 0x00000001) ? 0 : emask >> 1;
                    }
                    continue;
                case TB_EVENT_TIMER0:
                    TB.sec_counts++;

                    break;
                case TB_EVENT_STARTUP:
                   break;
                default: while(1); //@@ unhandled event.

            }
            FSP_CRITICAL_SECTION_DEFINE;
            FSP_CRITICAL_SECTION_ENTER;
            TB.event_flag &= (uint32_t) ~emask;
            FSP_CRITICAL_SECTION_EXIT;
            emask = (emask == 0x00000001) ? 0 : emask >> 1;
       }
    }
    return FSP_SUCCESS;
}
static fsp_err_t testbench_Open(void *data)
{
    timer_instance_t *p_tmr;
    fsp_err_t err;
    TB.event_flag = TB_EVENT_STARTUP;
    TB.driver = (driver_packet_t* ) data;
    p_tmr = TB.driver->p_tmr0[0];
    err = p_tmr->p_api->enable(p_tmr->p_ctrl);
    err |= p_tmr->p_api->callbackSet(p_tmr->p_ctrl,testbench_timer0_cb,NULL,&timer0_cb_args);
    err |= p_tmr->p_api->start(p_tmr->p_ctrl);
    assert(FSP_SUCCESS == err);
    p_tmr = TB.driver->p_tmr0[1];
    err = p_tmr->p_api->enable(p_tmr->p_ctrl);
//    err |= p_tmr->p_api->callbackSet(p_tmr->p_ctrl,testbench_timer0_cb,NULL,&timer0_cb_args);
//    err |= p_tmr->p_api->start(p_tmr->p_ctrl);
    return FSP_SUCCESS;
}
