/*
 * test_core.c
 *
 *  Created on: Jun 17, 2024
 *      Author: daled
 */

#include "test_core.h"
#include "bsp_api.h"
#include "hal_data.h"
#include "r_transfer_api.h"
uint32_t capture_buffer[TEST_CORE_CAPTURE_BUFFER_SIZE] BSP_PLACE_IN_SECTION(".dtcm_data");
uint16_t capture_idx BSP_PLACE_IN_SECTION(".dtcm_data");

core_ctrl_t Core BSP_PLACE_IN_SECTION(".dtcm_data");

const uint16_t flash_frame[TEST_CORE_HORZ*TEST_CORE_VERT] = {0x0000};
void test_core_init(void)
{

}
uint32_t test_run(uint32_t lcurrent_test)
{
    fsp_err_t err;
    uint32_t j;
    R_BUS->SDRAM.SDRFEN_b.RFEN = 0U; // turn off auto refresh

    Core.code_timer_done = 0;
    Core.code_timer_counts = 0;
    err = R_GPT_Start(&g_one_second_timer_ctrl);
    switch(lcurrent_test) {
        case 0:  // capture how many clocks it takes to write a const to all memory
            err = R_GPT_Reset(&g_freerun_timer_ctrl);
            err = R_GPT_Start(&g_freerun_timer_ctrl);
             memset((uint8_t*) Core.p_fb,0x00,(32*1024*1024)); //(horz_size*vert_size*2));
             Core.capture32[Core.capture32_idx] = g_freerun_timer_ctrl.p_reg->GTCNT;
             err = R_GPT_Stop(&g_freerun_timer_ctrl);
             Core.capture32_idx++;
        break;
        case 1:  //memcpy
            err = R_GPT_Reset(&g_freerun_timer_ctrl);
            Core.current_pixel_idx = 0;
            err = R_GPT_Start(&g_freerun_timer_ctrl);
            err = R_GPT_Start(&g_one_second_timer_ctrl);
        do
         {
             memcpy((uint8_t*) Core.p_fb,(uint8_t*) flash_frame,(TEST_CORE_HORZ*TEST_CORE_VERT*2)); //(horz_size*vert_size*2));
             Core.code_timer_counts++;
         } while (Core.code_timer_done == 0);
        Core.capture32[Core.capture32_idx] = g_freerun_timer_ctrl.p_reg->GTCNT;
        err = R_GPT_Stop(&g_freerun_timer_ctrl);
        Core.capture32_idx++;
        break;
        case 2:
        do
        {
            for(uint32_t i=0;i<(TEST_CORE_HORZ*TEST_CORE_VERT);i++)
            {
                *(Core.p_fb+i) = 0x1234; //flash_frame[i];
            }
            Core.code_timer_counts++;
        } while (Core.code_timer_done == 0);
        break;
        case 3:
        do
         {
             for(uint32_t i=0;i<(TEST_CORE_HORZ*TEST_CORE_VERT);i++)
             {
                 *(Core.p_fb + i) = flash_frame[i];
             }
             Core.code_timer_counts++;
         } while (Core.code_timer_done == 0);
        break;
        case 4:  // megabyte
        do
         {
            uint32_t i;
            for(i=0;i<0x01FFFFFF;i++)
             *(Core.xp_fb+i) = 0x0123;
            Core.code_timer_counts = i;
            j=0;
            while (Core.code_timer_done == 0)
            {
                *(Core.xp_fb+i) = 0x0405;
                j++;
            }
         } while (Core.code_timer_done == 0);
        Core.code_timer_counts += j;
        break;
        case 5:
            break;
#if 0
        do
         {
//                     for(uint32_t i=0;i<(horz_size*vert_size)/1024;i++)
//                     {
//                 double_pixel = RED_PIXELS;
                 Core.dma_done = 0;
                 Core.tilkx.p_dest = Core.xp_fb;
                 //tilkx.length = 1024;
                 err = R_DMAC_Reconfigure(&g_transfer0_ctrl, &Core.tilkx);
                 while(FSP_SUCCESS != err);
                 err = R_DMAC_Enable(&g_transfer0_ctrl);
                 while(FSP_SUCCESS != err);
                 err = R_DMAC_SoftwareStart(&g_transfer0_ctrl, TRANSFER_START_MODE_REPEAT);
                 while(FSP_SUCCESS != err);
                 while(0 == Core.dma_done);
//                     }

                 Core.code_timer_counts++;
         } while (Core.code_timer_done == 0);
        break;
#endif
        case 6:
            break;
    } // switch
    err = R_GPT_Stop(&g_freerun_timer_ctrl);
    err = R_GPT_Stop(&g_one_second_timer_ctrl);
    return(Core.code_timer_counts);



}


void code_timer_callback(timer_callback_args_t *p_args)
{
    timer_event_t t;
    switch (p_args->event) {
        case TIMER_EVENT_CYCLE_END:                     ///< Requested timer delay has expired or timer has wrapped around
         Core.code_timer_done = 1;
         break;
//        case TIMER_EVENT_CREST: ///< Timer crest event (counter is at a maximum: triangle-wave PWM only)
        case TIMER_EVENT_CAPTURE_A:                     ///< A capture has occurred on signal A
            break;
        case TIMER_EVENT_CAPTURE_B:                     ///< A capture has occurred on signal B
            break;
        case TIMER_EVENT_TROUGH:                        ///< Timer trough event (counter is 0: triangle-wave PWM only
            break;
        case TIMER_EVENT_COMPARE_A:                     ///< A compare has occurred on signal A
            break;
        case TIMER_EVENT_COMPARE_B:                     ///< A compare has occurred on signal B
            break;
        case TIMER_EVENT_COMPARE_C:                     ///< A compare has occurred on signal C
            break;
        case TIMER_EVENT_COMPARE_D:                     ///< A compare has occurred on signal D
            break;
        case TIMER_EVENT_COMPARE_E:                     ///< A compare has occurred on signal E
            break;
        case TIMER_EVENT_COMPARE_F:                     ///< A compare has occurred on signal F
            break;
        case TIMER_EVENT_DEAD_TIME:                      ///< Dead time event
            break;

    }
    /* TODO: add your own code here */
}

#if 0
void dma_callback(dmac_callback_args_t *cb_data)
{
    /* TODO: add your own code here */
    Core.dma_done = 1;
}

#endif

