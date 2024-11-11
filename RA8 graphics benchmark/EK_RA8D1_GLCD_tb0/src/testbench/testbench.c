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
#include "testbench_buttons.h"
#include "testbench_memory.h"
#include "draw_core/draw_core_api.h"
#include "Console/console_api.h"
#include "glcd.h"
#include "storage.h"

#include "common_utils.h"  //@@ YUCK... need a console !!!
//extern const bsp_leds_t g_bsp_leds;

#define CAPTURE_VSYNC    TB.vsync_data.data[TB.vsync_data.w_idx] = p_tmr_ctrl->p_reg->GTCNT; \
                         TB.vsync_data.w_idx = (TB.vsync_data.w_idx == (TB.vsync_data.size - 1)) ? 0 : TB.vsync_data.w_idx + 1;
#define CAPTURE_RENDER   TB.render_data.data[TB.render_data.w_idx] = p_tmr_ctrl->p_reg->GTCNT; \
                         TB.render_data.w_idx = (TB.render_data.w_idx == (TB.render_data.size - 1)) ? 0 : TB.render_data.w_idx + 1;

#define CAPTURE_ASYNC    TB.async_data.data[TB.async_data.w_idx] = p_tmr3_ctrl->p_reg->GTCNT; \
                         TB.async_data.w_idx = (TB.async_data.w_idx == (TB.async_data.size - 1)) ? 0 : TB.async_data.w_idx + 1;
#define CAPTURE_ANIMATE  TB.animate_data.data[TB.animate_data.w_idx] = p_tmr3_ctrl->p_reg->GTCNT; \
                         TB.animate_data.w_idx = (TB.animate_data.w_idx == (TB.animate_data.size - 1)) ? 0 : TB.animate_data.w_idx + 1;
/*
 *   Create a 6 sided poly  (needs 6 Y/X pairs, so 12 points)
 */
d2_point CirclePoints[4] = {(50 << 4),(50 << 4),
                            (40 << 4),(5 << 4)};

d2_point Points[12] = {(200 << 4),(200 << 4),
                      (300 << 4),(200 << 4),
                      (300 << 4),(300 << 4),
                      (200 << 4),(300 << 4),
                      (250 << 4),(250 << 4),
                      (200 << 4),(300 << 4)



};
d2_point Points2[12] = {(200 << 4),(600 << 4),
                      (300 << 4),(600 << 4),
                      (300 << 4),(700 << 4),
                      (200 << 4),(700 << 4),
                      (250 << 4),(650 << 4),
                      (200 << 4),(700 << 4)



};
d2_point Deltas[12] = {(1 << 4),(1 << 4) * (-1),
                      (2 << 4),(2 << 4),
                      (1 << 4)*(-1),(8 << 4),
                      (5 << 4),(3 << 4),
                      (6 << 4),(6 << 4),
                      (5 << 4),(3 << 4)
};
d2_point Deltas2[12] = {
                        (1 << 4),(1 << 4),
                        (1 << 4),(1 << 4),
                        (1 << 4)*(-1),(8 << 4),
                        (1 << 4),(1 << 4),
                        (1 << 4),(1 << 4),
                        (1 << 4),(1 << 4)
};
d2_point Accel[12] = {0};
dr_animate_t Poly_animate = {
                             .atype = 0,
                              .coord = (dr_point_t*) &Points[0],
                              .velocity = (dr_point_t*) &Deltas[0],
                              .acceleration = (dr_point_t*) &Accel[0],
                              .coord_size = 6,
                              .velocity_size = 6,
                              .acceleration_size = 6
};
dr_animate_t Poly_animate2 = {
                             .atype = 0,
                              .coord = (dr_point_t*) &Points2[0],
                              .velocity = (dr_point_t*) &Deltas2[0],
                              .acceleration = (dr_point_t*) &Accel[0],
                              .coord_size = 6,
                              .velocity_size = 6,
                              .acceleration_size = 6
};
dr_animate_t Circle_animate = {
                               .atype = 0,
                               .coord = (dr_point_t*) &Points[0],  //@@ not used anymore
                               .velocity = (dr_point_t*) &Deltas2[0],
                               .acceleration = (dr_point_t*) &Accel[0],
                               .coord_size = 1,
                               .velocity_size = 1,
                               .acceleration_size = 1,
                               .drag = 0,
                               .pull = 0

};
dr_render_t Poly_render = {
                           .coords = &Points[0],
                           .rtype = 1,  // 1 = PolyRender
                           .number = 6,
                           .state = 0,
                           .color = 0x00FF0000
};
dr_render_t Poly_render2 = {
                           .coords = &Points2[0],
                           .rtype = 1,  // 1 = PolyRender
                           .number = 6,
                           .state = 0,
                           .color = 0x0000FF00
};
dr_render_t Poly_render3 = {
                           .coords = &Points2[0],
                           .rtype = 1,  // 1 = PolyRender
                           .number = 6,
                           .state = 0,
                           .color = 0x000000FF
};
dr_render_t Circle_render = {
                           .coords = &CirclePoints[0],
                           .rtype = 2, // 2 = circle
                           .number = 4,
                           .state = 0,
                           .animator = (0xFFFF),
                           .color = 0x00000FF0
};
/*
 *   command functions
 */
static uint32_t ato32(char*  s);
static uint32_t ato32(char*  s)
{
    uint32_t rv;
    rv = 0;
    while(*s != 0)
    {
        rv = rv << 4;
        if (*s >= '0' && *s <= '9')
        {
            rv |= (uint32_t) (*s - '0');
        }
        else
        {
            rv |= (uint32_t) ((*s - 'a')+10);
        }
        s++;
    }
    return rv;
}
static d2_point atod2(char*);
static d2_point atod2(char* s)
{
    d2_point ret;
    d2_point sgn = 1;
    char *p;
    if (*s == '-')
    {
        s++;
        sgn = (-1);
    }
    p = s;
    while((*p != 0) && (*p != '.')) p++;
    if (*p == '.')
    {
        *p = 0; // terminate here for integer part.
        p++;
        ret = (ato32(p) & 0x0000000F);
    }
    ret = ret | (ato32(s) << 4);
    ret = ret * sgn;
    return ret;
}

static void command_Kick(char* cmd,char** args);
static void command_Set(char* cmd,char** args);
static void command_Tmr(char* cmd,char** args);
static void command_Emsk(char* cmd,char** args);

static void command_Kick(char* cmd,char** args)
// kick obj X Y
{
    FSP_PARAMETER_NOT_USED(cmd);
    int i;
    i = atoi(args[0]);
    dr_point_t *dpl;
    dr_point_t kck;
    dpl = (dr_point_t*)Render[i].coords;
    kck.X = atod2(args[1]);
    kck.Y = atod2(args[2]);
    dpl->X = dpl->X + kck.X;
    dpl->Y = dpl->Y + kck.Y;

}
static void command_Set(char* cmd,char** args)
{
   //set {object number} {V|A|C} {value} {value}
    FSP_PARAMETER_NOT_USED(cmd);
    int i;
    dr_point_t *dp1;
    i = atoi(args[0]);
    switch (args[1][0]) {
        case 'p':  // set the location
            ((dr_point_t*) Render[i].coords)->X = atod2(args[2]);
            ((dr_point_t*) Render[i].coords)->Y = atod2(args[3]);
            break;
        case 'v': // set the velocity
            Animate[Render[i].animator].velocity->X = atod2(args[2]);
            Animate[Render[i].animator].velocity->Y = atod2(args[3]);
            break;
        case 'a': // set the acceleration
            Animate[Render[i].animator].acceleration->X = atod2(args[2]);
            Animate[Render[i].animator].acceleration->Y = atod2(args[3]);
            Animate[Render[i].animator].atimer = (args[4] == NULL) ? 1 : (uint16_t) ato32(args[4]);
            break;
        case 'c': // set the color
            Render[i].color = ato32(args[2]);
            break;
        case 'r': // set the radius
            dp1 = ((dr_point_t*) Render[i].coords);
            dp1++;
            dp1->X = atod2(args[2]); // the radius for a circle.
            break;
    }
}
static void command_Emsk(char* cmd,char** args)
{
    FSP_PARAMETER_NOT_USED(cmd);
    uint32_t m = ato32(args[1]);
    switch (args[0][0]) {
        case 's':  // set the event.
            TB.event_mask |= m;
            break;
        case 'c': // stop timer.
            TB.event_mask &= ~m;
            break;
    }

}
static void command_Tmr(char* cmd,char** args)
{
   //set {object number} {V|A|C} {value} {value}
    FSP_PARAMETER_NOT_USED(cmd);
    int i;
    uint32_t j;
    dr_point_t *dp1;
    i = atoi(args[0]);
    switch (args[1][0]) {
        case 'p':  // set the period
            j = (uint32_t) atol(args[2]);
            testbench_timer_period_set((uint8_t) i, j);
            break;
        case 's': // stop timer.
            testbench_timer_stop((uint8_t) i);
            break;
        case 'g': // start timer.
            testbench_timer_start((uint8_t) i);
            break;
    }
}
void silly_putty(void);

/*
 *    API functions
 */
static fsp_err_t testbench_Open(void*);
static fsp_err_t testbench_Start(void) BSP_PLACE_IN_SECTION(".itcm");
static void testbench_Led(uint16_t idx,bsp_io_level_t x);
static void testbench_process(void);
static void testbench_RX(void);

static void testbench_RX_cb(console12_args_t *args);

// Global structures
testbench_command_t TB_CMD[TB_NUMBER_OF_COMMANDS] = {
{.name = "set",.help = "set various stuff", .func = command_Set},
{.name = "kick",.help = "kick a ball",      .func = command_Kick},
{.name = "tmr",.help = "t p|s|g [period]",  .func = command_Tmr},
{.name = "emsk",.help = "set timer stuff",   .func = command_Emsk},
                                                       {0}
};
const testbench_t TB_initial =
 {.open = testbench_Open,
  .start = testbench_Start,
  .led = (void*) testbench_Led,
  .idle_cycles = 0,
  .sec_counts = 0,
  .frame_count = 0,
  .animate_count = 0,
  .rxbuffer = TBRXBuffer,
  .rxbuffer_size = TB_RX_BUFFER_SIZE,
  .rxbuffer_idx = 0,
  .txbuffer = TBTXBuffer,
  .txbuffer_size = TB_TX_BUFFER_SIZE,
  .txbuffer_idx = 0,
  .state = TB_STATE_MASK_DEFAULT

 };


testbench_t TB; // BSP_PLACE_IN_SECTION(".dtcm_data");
// static declarations
timer_callback_args_t timer0_cb_args, timer2_cb_args;

static void testbench_Poll(void);
static void testbench_Poll(void)
{
    uint32_t A,B,C,D,i;
    double F;
    char c1[8];
    A = B = C = 0;
    for(i=0;i<TB.render_data.size;i++)
    {
        A = A + TB.render_data.data[i];
    }
    for(i=0;i<TB.vsync_data.size;i++)
    {
        B = B + TB.vsync_data.data[i];
    }
    for(i=0;i<TB.animate_data.size;i++)
    {
        C = C + TB.animate_data.data[i];
    }
    for(i=0;i<TB.async_data.size;i++)
    {
        D = D + TB.async_data.data[i];
    }
    A = A /(TB.render_data.size * 120);
    B = B /(TB.vsync_data.size * 120);
    C = C /(TB.animate_data.size * 120);
    D = D /(TB.async_data.size * 120);
//    A = A / 120; //@@ assumes clocking at 120mHz
//    B = B / 120;
    F = (double) 1000000 / B;
    sprintf(c1,"%2.4f",F);

    //A = (TB.render_data.data[0] * 100) // TB.vsync_data.data[0];
    sprintf(TB.txbuffer,"\r\nVideo:   %lduS [fr: %s] \r\n",B,c1);
    CON12.prints(TB.txbuffer);
    F = (double) 1000000 / D;
    sprintf(c1,"%2.4f",F);
    sprintf(TB.txbuffer,"Animate: %lduS [fr: %s] \r\n",D,c1);
    CON12.prints(TB.txbuffer);
    sprintf(TB.txbuffer,"animation (%lduS)  render = (%lduS)\r\n",C,A);
    CON12.prints(TB.txbuffer);
    void *p_ctrl = TB.driver->p_tmr0[2]->p_ctrl;
    timer_info_t Tinfo;
    TB.driver->p_tmr0[2]->p_api->infoGet(p_ctrl,&Tinfo);
//    gpt_instance_ctrl_t *r = (gpt_instance_ctrl_t *) TB.driver->p_tmr0[1]->p_ctrl;
    sprintf(TB.txbuffer,"animate timer period = %ld\r\n",Tinfo.period_counts);
    CON12.prints(TB.txbuffer);

}
static fsp_err_t testbench_Start(void)
{
    uint32_t events;
    uint32_t emask;
    uint16_t handle;
    timer_instance_t *p_tmr;
    timer_instance_t *p_tmr3;
    gpt_instance_ctrl_t *p_tmr_ctrl;
    gpt_instance_ctrl_t *p_tmr3_ctrl;
    p_tmr = (timer_instance_t*) TB.driver->p_tmr0[1];
    p_tmr3 = (timer_instance_t*) TB.driver->p_tmr0[3];
    p_tmr_ctrl = (gpt_instance_ctrl_t*) TB.driver->p_tmr0[1]->p_ctrl;
    p_tmr3_ctrl = (gpt_instance_ctrl_t*) TB.driver->p_tmr0[3]->p_ctrl;
    events = 0;
    FSP_CRITICAL_SECTION_DEFINE;
    while(1)
    {
        events = (TB.event_flag & TB.event_mask);
        emask = 0x80000000;
        while(emask){
            switch(events & emask) {
                case 0: // crawl through the emask until another event is found.
                    while( (emask != 0x00000000) && ((emask & events) == 0))
                        {emask = (emask == 0x00000001) ? 0 : emask >> 1;}
                    continue;
                case TB_EVENT_TIMER0:
                    if (TB_STATE_POLL_STATUS & TB.state)
                    {
                       testbench_Poll();
                    }
                    TB.sec_counts++;
                    break;
                case TB_EVENT_TIMER2:
                    CAPTURE_ASYNC
                    p_tmr3->p_api->reset(p_tmr3_ctrl);
                    p_tmr3->p_api->start(p_tmr3_ctrl);
                    TB.animate_count++;
                    draw_core_animate();
                    CAPTURE_ANIMATE
                    FSP_CRITICAL_SECTION_ENTER;
                    TB.event_flag |= TB_EVENT_ADONE;
                    FSP_CRITICAL_SECTION_EXIT;
                    break;
                case TB_EVENT_VSYNC:
                    CAPTURE_VSYNC
                    p_tmr->p_api->reset(p_tmr_ctrl);
                    p_tmr->p_api->start(p_tmr_ctrl);
#if 0
                    TB.animate_count++;
                    draw_core_animate();
                    CAPTURE_ANIMATE
#endif
     //               if (TB.event_flag & TB_EVENT_ADONE)
                    if(TB.event_flag & TB_EVENT_ADONE)
                    {
                        draw_core_draw((uint32_t *)TB.p_activeframe);
                        CAPTURE_RENDER
                        TB.frame_count++;
                        glcd_swap();
                        FSP_CRITICAL_SECTION_ENTER;
                        TB.event_flag &= (uint32_t) ~TB_EVENT_ADONE;
                        FSP_CRITICAL_SECTION_EXIT;
                    }
                    break;
                case TB_EVENT_STARTUP:
//                    draw_core_init();
//                    FSP_CRITICAL_SECTION_DEFINE;
//                    FSP_CRITICAL_SECTION_ENTER;
//                    TB.event_flag &= (uint32_t) ~emask;
//                    FSP_CRITICAL_SECTION_EXIT;
                    break;
                case TB_EVENT_SW1:
                    TB.led(0,BSP_IO_LEVEL_HIGH);
                    handle = RenderList.add(&Circle_render);
                    AnimateList.add(handle,&Circle_animate);
                    break;
                case TB_EVENT_SW2:
                    TB.led(0,BSP_IO_LEVEL_LOW);
                    Animate[0].atype = 1;
                    Animate[1].atype = 1;
                    Animate[2].atype = 1;
                    break;
                case TB_EVENT_RXMSG:
                    testbench_RX();
                    break;
                default: while(1); //@@ unhandled event.

            }

            FSP_CRITICAL_SECTION_ENTER;
            TB.event_flag &= (uint32_t) ~emask;
            FSP_CRITICAL_SECTION_EXIT;
            emask = (emask == 0x00000001) ? 0 : emask >> 1;
       }
    }
    return FSP_SUCCESS;
}
static void testbench_Led(uint16_t idx,bsp_io_level_t x)
{
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,TB.driver->board_leds->p_leds[idx],x);
}

static fsp_err_t testbench_Open(void *data)
{
    timer_instance_t *p_tmr;
    console12_t *p_con12;
    fsp_err_t err;
    TB.event_flag = TB_EVENT_STARTUP;
    TB.event_mask = TB_MASK_DEFAULT;
    TB.animate_data.data = Sample_Buffer_Animate;
    TB.animate_data.size = CAPTURE_BUFFER_SIZE;
    TB.animate_data.w_idx = 1;
    TB.render_data.data = Sample_Buffer_Render;
    TB.render_data.size = CAPTURE_BUFFER_SIZE;
    TB.render_data.w_idx = 1;
    TB.async_data.data = Sample_Buffer_ASYNC;
    TB.async_data.size = CAPTURE_BUFFER_SIZE;
    TB.async_data.w_idx = 1;
    TB.vsync_data.data = Sample_Buffer_VSYNC;
    TB.vsync_data.size = CAPTURE_BUFFER_SIZE;
    TB.vsync_data.w_idx = 1;
    TB.driver = (driver_packet_t* ) data;
    TB.mode = 0; //@@enum wanted

    p_con12 = (console12_t*)TB.driver->p_Console;
    p_con12->rx_cb = testbench_RX_cb;
    p_con12->open(TB.rxbuffer,TB.rxbuffer_size,NULL);
    // open, enable and start cadence timer
    for(int i=0;i<TB_NUMBER_OF_TIMERS;i++)
    {
        p_tmr = TB.driver->p_tmr0[i];
        err = p_tmr->p_api->open(p_tmr->p_ctrl,p_tmr->p_cfg);
        err |= p_tmr->p_api->enable(p_tmr->p_ctrl);
        switch(i) {
            case 0: // poll timer.. typically 1 second.
                err |= p_tmr->p_api->callbackSet(p_tmr->p_ctrl,testbench_timer0_cb,NULL,&timer0_cb_args);
                err |= p_tmr->p_api->start(p_tmr->p_ctrl);
                break;
            case 1: // measuring timer for vertical syncs
                break;
            case 2: // animation cadence timer ..  typically 100mS
                err |= p_tmr->p_api->callbackSet(p_tmr->p_ctrl,testbench_timer2_cb,NULL,&timer2_cb_args);
                err |= p_tmr->p_api->start(p_tmr->p_ctrl);
                break;
            case 3:  // measuring timer for animation cadence
                break;
        }
        assert(FSP_SUCCESS == err);
    }
    // open and enable the press buttons external interrupts.
    void *pctrl;
    void *pcfg;

    for(int i=0;i<TB_NUMBER_OF_PRESS_BUTTONS;i++)
    {
        pctrl = (void *) TB.driver->p_SW[i]->p_ctrl;
        pcfg = (void*) TB.driver->p_SW[i]->p_cfg;
        if (TB.driver->p_SW[i] != NULL)
        {
            err  = TB.driver->p_SW[i]->p_api->open(pctrl,pcfg);
            switch (i) {
                case 0:
                    err |= TB.driver->p_SW[i]->p_api->callbackSet(pctrl,SW1_callback,NULL,&SW1_cb_args);
                    break;
                case 1:
                    err |= TB.driver->p_SW[i]->p_api->callbackSet(pctrl,SW2_callback,NULL,&SW2_cb_args);
                    break;
                default: while(1); //TRAP
            }
            err |= TB.driver->p_SW[0]->p_api->enable(pctrl);
    //        fsp_err_t (* callbackSet)(external_irq_ctrl_t * const p_ctrl, void (* p_callback)(external_irq_callback_args_t *),
    //                                  void const * const p_context, external_irq_callback_args_t * const p_callback_memory);

        }

    }
    TB.p_activeframe = TB.driver->p_GLCDC->p_cfg->input[0].p_base;
    TB.framesize = (TB.driver->p_GLCDC->p_cfg->input[0].hsize * TB.driver->p_GLCDC->p_cfg->input[0].vsize * (uint32_t) 2);//@@
    glcd_init();
    while ((TB.event_flag & (uint32_t) TB_EVENT_VSYNC) == 0);
    TB.event_flag &= (uint32_t) ~TB_EVENT_VSYNC;
    draw_core_init();

    //@@  startup
    /*
     *
    */
    MEM.clear(32);
    uint16_t handle;
    uint32_t i;
    handle = RenderList.add(&Poly_render);
    AnimateList.add(handle,&Poly_animate);  //@@ uses side-effect... don't like-a dat-a
#if 0
    for(i=0;i < (Poly_render.number * 2); i++)
    {
    Poly_render.coords[i] += (50 << 4);// Poly_render.coords[i+1] += 50;
    }

    Poly_render.color = 0x0000FF00;
    handle = RenderList.add(&Poly_render);
    AnimateList.add(handle,&Poly_animate);
    for(i=0;i < (Poly_render.number * 2); i++)
    {
    Poly_render.coords[i] += (50 << 4);// Poly_render.coords[i+1] += 50;
    }

    Poly_render.color = 0x000000FF;
    handle = RenderList.add(&Poly_render);
    Poly_animate.atype = 1;
    AnimateList.add(handle,&Poly_animate);
#endif
//    AnimateList.add(handle,&Poly_animate2);
//    dr_point_t C;
//    dr_point_t *zebra;
//    C.X = (200 << 4); C.Y = (200 <<4);
//@@    zebra =  draw_core_star(C,(50 << 4), (25 << 4),5);

    return FSP_SUCCESS;
}
volatile uint32_t freddy;
static void testbench_process(void)
{
    int i;
    i=0;
    while ((TB_CMD[i].name != NULL) && (strcmp((char*) TB.console_args->cmd,TB_CMD[i].name))) i++;
    if (TB_CMD[i].func != NULL)
    {
        TB_CMD[i].func(NULL,(char**) TB.console_args->args);
    }
    else
    {
        CON12.prints("\r\n not found\r\n>");
    }
}
static void testbench_RX(void)
{

    if (TB.rxbuffer[0] == 27)
    {
        if ((TB.state & TB_STATE_POLL_STATUS) == 0)
        {
            TB.state |= TB_STATE_POLL_STATUS;
            TB.state &= (uint32_t) ~TB_STATE_COMMAND;
            CON12.prints("\r\n(polling)\r\n");
        }
        else
        {
            TB.state &= (uint32_t)~TB_STATE_POLL_STATUS;
            TB.state |= TB_STATE_COMMAND;
            CON12.prints("\r\n(command)\r\n> ");
        }
        return;
    }
    testbench_process();
    CON12.prints("\r\n > ");

}

static void testbench_RX_cb(console12_args_t *args)
{
    TB.event_flag |= TB_EVENT_RXMSG;
    TB.rxbuffer_size = (uint16_t) args->len;
    TB.console_args = args;
    // the data is already in TB.rxbuffer
}


/*
 *   Call this in the Warmstart of hal_entry.
 */
void testbench_warmstart(void)
{
  TB = TB_initial;
  memset((uint8_t*) &Animate[0],0,sizeof(dr_animate_t) * DRAW_CORE_NUMBER_OF_ANIMATIONS);
  memset((uint8_t*) &Render[0],0,sizeof(dr_render_t) * DRAW_CORE_NUMBER_OF_RENDERS);
}


void silly_putty(void)
{
    Render[0].color = 0x0000FF00;
}
