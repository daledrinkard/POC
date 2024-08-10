/*
 * testbench.c
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "testbench_api.h"
#include "testbench_timers.h"
#include "draw_core/draw_core_api.h"
#include "glcd.h"
extern const bsp_leds_t g_bsp_leds;

#define CAPTURE_VSYNC    TB.vsync_data.data[TB.vsync_data.w_idx] = p_tmr_ctrl->p_reg->GTCNT; \
                         TB.vsync_data.w_idx = (TB.vsync_data.w_idx == TB.vsync_data.size) ? 0 : TB.vsync_data.w_idx + 1;
#define CAPTURE_ANIMATE  TB.animate_data.data[TB.animate_data.w_idx] = p_tmr_ctrl->p_reg->GTCNT; \
                         TB.animate_data.w_idx = (TB.animate_data.w_idx == TB.animate_data.size) ? 0 : TB.animate_data.w_idx + 1;
#define CAPTURE_RENDER   TB.render_data.data[TB.render_data.w_idx] = p_tmr_ctrl->p_reg->GTCNT; \
                         TB.render_data.w_idx = (TB.render_data.w_idx == TB.render_data.size) ? 0 : TB.render_data.w_idx + 1;
/*
 *   Create a poly
 */
d2_point Points[12] = {(200 << 4),(200 << 4),
                      (300 << 4),(200 << 4),
                      (300 << 4),(300 << 4),
                      (150 << 4),(150 << 4),
                      (100 << 4),(100 << 4),
                      (200 << 4),(300 << 4)



};
d2_point Deltas[12] = {(1 << 4),(1 << 4) * (-1),
                      (2 << 4),(2 << 4),
                      (1 << 4)*(-1),(8 << 4),
                      (5 << 4),(3 << 4),
                      (6 << 4),(6 << 4),
                      (5 << 4),(3 << 4)


};
d2_point Accel[12] = {0};
dr_animate_t Poly_animate = {
                             .atype = 1,
                              .coord = &Points[0],
                              .velocity = &Deltas[0],
                              .acceleration = &Accel[0],
                              .size = 6
};
dr_render_t Poly_render = {
                           .coords = &Points[0],
                           .rtype = 1,
                           .number = 6,
                           .state = 0
};
dr_render_t Box_render = {
                           .coords = &Points[0],
                           .rtype = 2,
                           .number = 6,
                           .state = 0
};
/*
 *    API functions
 */
static fsp_err_t testbench_Open(void*);
static fsp_err_t testbench_Start(void);

uint32_t AnimateBuffer[CAPTURE_BUFFER_SIZE];
uint32_t RenderBuffer[CAPTURE_BUFFER_SIZE];
uint32_t VSYNCBuffer[CAPTURE_BUFFER_SIZE];
// Global structures
testbench_t TB = {.open = testbench_Open,
                  .start = testbench_Start,
                  .idle_cycles = 0,
                  .sec_counts = 0
                 };

// static declarations
timer_callback_args_t timer0_cb_args;


static fsp_err_t testbench_Start(void)
{
    uint32_t events;
    uint32_t emask;
    timer_instance_t *p_tmr;
    gpt_instance_ctrl_t *p_tmr_ctrl;
    p_tmr = (timer_instance_t*) TB.driver->p_tmr0[1];
    p_tmr_ctrl = (gpt_instance_ctrl_t*) TB.driver->p_tmr0[1]->p_ctrl;
    while(1)
    {
        events = (TB.event_flag & TB.event_mask);
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
                case TB_EVENT_VSYNC:
                    CAPTURE_VSYNC
                    p_tmr->p_api->reset(p_tmr_ctrl);
                    p_tmr->p_api->start(p_tmr_ctrl);
                    draw_core_animate();
                    CAPTURE_ANIMATE
                    draw_core_draw(TB.p_activeframe);
                    CAPTURE_RENDER
                    glcd_swap();
                    break;
                case TB_EVENT_STARTUP:
//                    draw_core_init();
//                    FSP_CRITICAL_SECTION_DEFINE;
//                    FSP_CRITICAL_SECTION_ENTER;
//                    TB.event_flag &= (uint32_t) ~emask;
//                    FSP_CRITICAL_SECTION_EXIT;

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
    TB.event_mask = TB_MASK_DEFAULT;
    TB.animate_data.data = &AnimateBuffer[0];
    TB.animate_data.size = CAPTURE_BUFFER_SIZE;
    TB.animate_data.w_idx = 1;
    TB.render_data.data = RenderBuffer;
    TB.render_data.size = CAPTURE_BUFFER_SIZE+1;
    TB.render_data.w_idx = 2;
    TB.vsync_data.data = VSYNCBuffer;
    TB.vsync_data.size = CAPTURE_BUFFER_SIZE+2;
    TB.vsync_data.w_idx = 3;
    TB.driver = (driver_packet_t* ) data;
    TB.state = 0;
    // open, enable and start cadence timer
    p_tmr = TB.driver->p_tmr0[0];
    err = p_tmr->p_api->enable(p_tmr->p_ctrl);
    err |= p_tmr->p_api->callbackSet(p_tmr->p_ctrl,testbench_timer0_cb,NULL,&timer0_cb_args);
    err |= p_tmr->p_api->start(p_tmr->p_ctrl);
    assert(FSP_SUCCESS == err);
    //open and enable the measure timer
    p_tmr = TB.driver->p_tmr0[1];
    err = p_tmr->p_api->enable(p_tmr->p_ctrl);
    assert(FSP_SUCCESS == err);
    TB.p_activeframe = TB.driver->p_GLCDC->p_cfg->input[0].p_base;
    TB.framesize = (TB.driver->p_GLCDC->p_cfg->input[0].hsize * TB.driver->p_GLCDC->p_cfg->input[0].vsize * 2);//@@
    glcd_init();
    while ((TB.event_flag & (uint32_t) TB_EVENT_VSYNC) == 0);
    TB.event_flag &= (uint32_t) ~TB_EVENT_VSYNC;
    draw_core_init();
    //@@  startup
    /*
     *
    */
    RenderList.add(&Poly_render);
    AnimateList.add(&Poly_animate);
    return FSP_SUCCESS;
}
