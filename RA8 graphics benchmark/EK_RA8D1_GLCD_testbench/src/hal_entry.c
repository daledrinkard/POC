/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"
#include "graphics_settings.h"
#include <stdio.h>
#include "test_core/test_core.h"
#include "draw_core/draw_core.h"
//#include "SCI_debug_print/SCI_debug_print.h"  //only because LLVM is used and printf_redirect isn't figured out yet.

#define PIXEL_CACHE_SIZE (256)
#define APP_ERR_TRAP(err)               ({\
    if(err)\
    {\
        __asm("BKPT #0\n"); /* trap upon the error  */ \
    }\
})
/*
 *   GLOBALS
 */
draw_core_cfg_t DrawCore_cfg BSP_PLACE_IN_SECTION(".dtcm_data");
char buffer1[64];
display_input_cfg_t *p_display_config;
uint16_t horz_size,vert_size;
uint16_t temp_pixel;
uint16_t pixel_cache[PIXEL_CACHE_SIZE] BSP_PLACE_IN_SECTION(".dtcm_data");
uint32_t xpixel_cache[PIXEL_CACHE_SIZE/2] BSP_PLACE_IN_SECTION(".dtcm_data");
//uint16_t pixel_cache[PIXEL_CACHE_SIZE];
uint32_t double_pixel;
uint64_t quad_pixel;
uint32_t current_test;
uint32_t loop_counter;
volatile uint32_t fxmode = 0;
volatile uint32_t aux_timer = 0;
void R_BSP_WarmStart(bsp_warm_start_event_t event);
void polly(void);
extern volatile uint8_t g_vsync_flag;


/*
 *    PRIVATE
 */
static void rbg_render565(uint8_t);
static void test1(void);
static void setup_LCD(void);
static void open_timers(void);


/*
 *   ENTRY
 */
/*
 *    PUBLIC FUNCTIONS
 */
/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/

#define BYTES_PER_PIXEL (2)
void hal_entry (void)
/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/

{
#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
    volatile uint32_t tempA,tempB;
    uint32_t * p_framebuffer = NULL;
    volatile fsp_err_t           err     = FSP_SUCCESS;
    volatile uint32_t u32_temp;
    /* Initialize SDRAM */


    bsp_sdram_init();
    memset(&g_framebuffer[0],0,(1024*1024));
    rbg_render565(0);
    setup_LCD();
    p_display_config = &g_display0.p_cfg->input[0];
    open_timers();
    DrawCore_cfg.fb0 = (uint16_t *)g_framebuffer[0];
    DrawCore_cfg.horz = p_display_config->hsize;
    DrawCore_cfg.vert = p_display_config->vsize;
    draw_core_init(&DrawCore_cfg);

printf("\r\n");

    Core.p_fb = (uint16_t *)g_framebuffer[0];
    Core.xp_fb= (uint32_t *)g_framebuffer[0];
    Core.bp_fb = (uint8_t*) g_framebuffer[0];
    horz_size = p_display_config->hsize;
    vert_size = p_display_config->vsize;
    Core.image_size = (horz_size*vert_size);
//    horz_size = 1280;
//    vert_size = 720;
    sprintf(buffer1,"horz = %d\r\nvert = %d\r\npixels = %d\r\n",horz_size,vert_size,(horz_size*vert_size));
    printf(buffer1);
    printf("Calibrate free run timer..");
    Core.code_timer_done = 0;
    err = R_GPT_Start(&g_one_second_timer_ctrl);
    err = R_GPT_Start(&g_freerun_timer_ctrl);
    while (Core.code_timer_done == 0);
    err = R_GPT_Stop(&g_freerun_timer_ctrl);
    sprintf(buffer1,"counts = %ld\r\n",g_freerun_timer_ctrl.p_reg->GTCNT);
    printf(buffer1);











    draw_core_run(0);
    while(1);

    err = R_GLCDC_Start(&g_display0_ctrl);






//test_run(NULL);
#if 0 //DMA
    transfer_addr_mode_t f;
    transfer_chain_mode_t rre;
    err = R_DMAC_Open(&g_transfer0_ctrl, &g_transfer0_cfg);
    memcpy(&Core.tilkx,g_transfer0_ctrl.p_cfg->p_info,sizeof(transfer_info_t));
    Core.tilkx.p_src = &double_pixel;
    Core.tilkx.p_dest = Core.xp_fb;
//    err = R_DMAC_Reconfigure(&g_transfer0_ctrl, &tilkx);
//err = R_DMAC_Enable(&g_transfer0_ctrl);

#endif
//    extern void dd(void);
//    dd();
    while(1)
    {
        /*
         *     SDRAM address space 0x6800_0000
         */
        switch(current_test) {
            case 0: printf("-------------------------------\r\nstartup()\r\n");
            for(uint32_t jj=0;jj<2;jj++)
            {
                Core.capture32_idx = 0;
                tempA = 0;
                for(uint32_t j=0;j<10;j++)
                {
                    test_run(0);
                    tempA = tempA + Core.capture32[j];
                }
                tempA = (tempA/10) / 1024;
                tempB = (16*1024*1024) / 1024;
                tempA = (tempB*10000)/tempA;
                sprintf(buffer1," BW: %d.%d\r\n",(tempA/100),(tempA%100));
                printf(buffer1);
                if(0 == jj)
                {
                    printf("Starting GLCD\r\n");
                    err = R_GLCDC_Start(&g_display0_ctrl);
                    R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
                    R_GPT_Reset(&g_aux_timer_ctrl);
                    R_GPT_Start(&g_aux_timer_ctrl);
                    aux_timer = 0;
                    fxmode = 0;
                    while(aux_timer < 1);
                    R_GPT_Stop(&g_aux_timer_ctrl);
                    sprintf(buffer1,"fps = %d\r\n",fxmode);
                    printf(buffer1);
                }
            }
            printf("Stopping GLCD\r\n");
            err = R_GLCDC_Stop(&g_display0_ctrl);
            R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);

            break;
            case 1: printf("memcpy(from Flash)\r\n");
                    Core.capture32_idx = 0;
                    test_run(1);
                    tempB = Core.code_timer_counts * Core.image_size; // how many clocks it should take
                    tempA = Core.capture32[0]; // how many it did.
                    tempA = tempA / 1024;  // chop off some resolution to fit in 32-bit numbers.
                    tempB = tempB / 1024;
                    tempA = (tempB*10000)/tempA;
                    sprintf(buffer1," BW: %d.%d\r\n",(tempA/100),(tempA%100));
                    printf(buffer1);
                    break;
            case 2: printf("for(pixel loop) const\r\n"); break;
            case 3: printf("for(pixel loop) from Flash\r\n"); break;
            case 4: printf("megabyte \r\n"); break;
            case 5: printf("DMAC\r\n"); break;
            case 6:
                printf("GLCD started\r\n");
                err = R_GLCDC_Start(&g_display0_ctrl);
                R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
                printf("draw\r\n");
//                draw_core_run(0);
                break;
        }

//        sprintf(buffer1,"counts = %d",Core.code_timer_counts);
//        printf(buffer1);
//        sprintf(buffer1,"\r\n  time = %d\r\n",(g_freerun_timer_ctrl.p_reg->GTCNT/120));
//        printf(buffer1);
        switch (current_test) {
            case 0: current_test = 6; break;
            case 6: current_test = 100; break;
//            case 2: current_test =
            case 99:
                loop_counter++;
                current_test = 1;
                if (1 == loop_counter)
                {
                    printf("GLCD started\r\n");
                    err = R_GLCDC_Start(&g_display0_ctrl);
                    R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
                    R_GPT_Reset(&g_aux_timer_ctrl);
                    R_GPT_Start(&g_aux_timer_ctrl);
                    aux_timer = 0;
                    fxmode = 0;
                    while(aux_timer < 1);
                    sprintf(buffer1,"fps = %ld\r\n",fxmode);
                    printf(buffer1);

                }
                break;
            case 100: current_test = 100; break;
            default:
                current_test++;
        }
        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
    }

}




void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&g_ioport_ctrl, g_ioport.p_cfg);
    }
}
/*
 *   PRIVATE FUNCTIONS
 */
static void rbg_render565(uint8_t buf_idx)
{
    /* generate the rendered image */

    //Variable to hold display configuration
    display_input_cfg_t const *p_input = &g_display0.p_cfg->input[0];

    //pointer to frame buffer
    uint16_t * buf_ptr = (uint16_t *)g_framebuffer[buf_idx];

    uint16_t bar_width = p_input->vsize/7;

    //for loop across vertical, then horizontal pixels
    for(uint32_t y = 0; y < p_input->vsize; y++)
    {
        for(uint32_t x = 0; x < p_input->hsize; x++)
        {
            if(y < bar_width)
            {
                buf_ptr[x] = BLUE;
            }
            else if(y < (bar_width*2) )
            {
                buf_ptr[x] = CYAN;
            }
            else if(y < (bar_width*3) )
            {
                buf_ptr[x] = GREEN;
            }
            else if(y < (bar_width*4) )
            {
                buf_ptr[x] = WHITE;
            }
            else if(y < (bar_width*5) )
            {
                buf_ptr[x] = MAGENTA;
            }
            else if(y < (bar_width*6) )
            {
                buf_ptr[x] = BLACK;
            }
            else
            {
                buf_ptr[x] = YELLOW;
            }
        }
        buf_ptr += p_input->hstride;
    }
}
static void open_timers(void)
{
    fsp_err_t err = FSP_SUCCESS;

err = R_GPT_Open(&g_one_second_timer_ctrl, &g_one_second_timer_cfg);
err = R_GPT_Enable(&g_one_second_timer_ctrl);
err = R_GPT_Open(&g_freerun_timer_ctrl, &g_freerun_timer_cfg);
err = R_GPT_Enable(&g_freerun_timer_ctrl);
err = R_GPT_Open(&g_sample_timer_ctrl, &g_sample_timer_cfg);
err = R_GPT_Enable(&g_sample_timer_ctrl);
err = R_GPT_Open(&g_aux_timer_ctrl, &g_aux_timer_cfg);
err = R_GPT_Enable(&g_aux_timer_ctrl);


}
static void setup_LCD(void)
{
    //set up LCD display
    fsp_err_t err;
    /* Configure display GPIO used by this application */
    R_IOPORT_PinCfg(&g_ioport_ctrl, DISP_RST, IOPORT_PERIPHERAL_IO | IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    R_IOPORT_PinCfg(&g_ioport_ctrl, DISP_BLEN, IOPORT_PERIPHERAL_IO | IOPORT_CFG_PORT_DIRECTION_OUTPUT);

    /* Reset Display - active low
     * NOTE: Wake may not be issued for 120 ms after HW reset */
    R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_RST, BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(7, BSP_DELAY_UNITS_MICROSECONDS); // Set active for 5-9 us
    R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_RST, BSP_IO_LEVEL_HIGH);

    /* open MIPI DSI Interface */
    err = R_GLCDC_Open(&g_display0_ctrl, &g_display0_cfg);
    APP_ERR_TRAP(err);

    /* NOTE: cannot send commands for 5 ms after HW reset */
    R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MILLISECONDS);
    mipi_dsi_push_table(g_lcd_init_focuslcd);
    /* Note: When video is started, timing of any further messages must carefully controlled to not interfere with transmission. */
    err = R_GLCDC_Start(&g_display0_ctrl);
    APP_ERR_TRAP(err);

    /* Wait for the LCD to display valid data, so no white flash on screen */
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);

    /* Enable the backlight */
    err = R_IOPORT_PinWrite(&g_ioport_ctrl, DISP_BLEN, BSP_IO_LEVEL_HIGH);
    APP_ERR_TRAP(err);
}
static void test1(void)
{
    // loop through various screen resolutions.

}
/*
 *   CALLBACK FUNCTIONS
 */
void sample_timer_cb(timer_callback_args_t *p_args)
{
    timer_event_t t;
    switch (p_args->event) {
        case TIMER_EVENT_CYCLE_END:                     ///< Requested timer delay has expired or timer has wrapped around
//         code_timer_done = 1;
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

void glcd_callback(display_callback_args_t *p_args)
{
    /* TODO: add your own code here */
        fxmode++;


}

void my_mipi_cb(mipi_dsi_callback_args_t *p_args)
{
    /* TODO: add your own code here */
    switch(fxmode) {
        case 1:
            R_GPT_Reset(&g_freerun_timer_ctrl);
            R_GPT_Start(&g_freerun_timer_ctrl);
            fxmode++; break;
        case 2: R_GPT_Stop(&g_freerun_timer_ctrl);
            fxmode++; break;
        default:;
    }
}


void aux_timer_cb(timer_callback_args_t *p_args)
{
    /* TODO: add your own code here */
    aux_timer++;
}
