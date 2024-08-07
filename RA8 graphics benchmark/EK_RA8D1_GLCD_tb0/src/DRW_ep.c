/***********************************************************************************************************************
 * File Name    : DRW_ep.c
 * Description  : Contains data structures and functions used in DRW_ep.c.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2022-2024 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
#include "DRW_ep.h"
#include "hal_data.h"
#include "common_utils.h"
#include "graphics_settings.h"
#include "testbench_cfg.h"
#include "SCI_RTT.h"
#include "draw_core/draw_core_api.h"



volatile uint8_t g_vsync_flag = RESET_FLAG;

/* User defined functions */
void glcdc_callback(display_callback_args_t * args);
static void glcdc_init(void);


typedef struct gvar_s {
    fsp_pack_version_t version;
    uint32_t * p_framebuffer;
    uint16_t hsize;
    uint16_t vsize;
    uint32_t buffer_size;
    fsp_err_t (* open)(void*);
    void (*swap)(void*);
}gvar_t;
fsp_err_t Gvar_Open(void* p);
void Gvar_Swap(void* p);
gvar_t Gvar = {.open = Gvar_Open,
               .swap = Gvar_Swap};

void Gvar_Swap(void* p)
{
    gvar_t *p_gvar = (gvar_t*) p;
    p_gvar->p_framebuffer = (p_gvar->p_framebuffer == g_framebuffer[0]) ?
            g_framebuffer[1] : g_framebuffer[0];
}
fsp_err_t Gvar_Open(void* p)
{
    gvar_t *p_gvar = (gvar_t*) p;
    R_FSP_VersionGet(&p_gvar->version);
    p_gvar->p_framebuffer = (uint32_t*) &g_framebuffer[0][0];
    p_gvar->hsize = g_display_cfg.input[0].hsize;
    p_gvar->vsize = g_display_cfg.input[0].vsize;
    p_gvar->buffer_size = (uint32_t) (p_gvar->hsize * p_gvar->vsize * BYTES_PER_PIXEL);
    return FSP_SUCCESS;
}
volatile uint32_t tommy=0;
/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/
void DRW_entry(void)
{
    //#if defined(BOARD_RA8D1_EK)
    fsp_err_t err = FSP_SUCCESS;
    SCI_RTT_Open((void*) &g_printf_uart, NULL);
    APP_PRINT("test");
    Gvar.open(&Gvar);
    bsp_sdram_init();
    glcdc_init();
      memset((uint16_t*) &g_framebuffer[0],0x07E0,sizeof(g_framebuffer));
    while(!g_vsync_flag);
    g_vsync_flag = RESET_FLAG;
    draw_core_init();
    while(true)
    {
        /* Swap the active framebuffer */
        Gvar.swap(&Gvar);
        while(!g_vsync_flag);
        g_vsync_flag = RESET_FLAG;
        err = R_GLCDC_BufferChange(&g_display_ctrl, (uint8_t*) Gvar.p_framebuffer, DISPLAY_FRAME_LAYER_1);
        /* Handle error */
        if(FSP_SUCCESS != err)
        {
            /* GLCDC initialization failed  */
            APP_ERR_PRINT("\r\n ** GLCDC Buffer change FAILED ** \r\n");
            APP_ERR_TRAP(err);
        }
//        while(3 == tommy);
        /* Draw the new framebuffer now */
        draw_core_draw(Gvar.p_framebuffer);
        tommy++;
    }

    APP_PRINT("\r\n ** Image rendering successful ** \r\n");
#if 0
    /* Wait for rendering operations to finish */
    DRW_err = d2_flushframe(gp_davey);
    error_handler(DRW_err, gp_davey);

    /* De-initialize hardware and close the handle */
    DRW_err = d2_deinithw(gp_davey);
    error_handler(DRW_err, gp_davey);

    DRW_err = d2_closedevice(gp_davey);
    error_handler(DRW_err, gp_davey);

    /* Stop GLCDC */
    err = R_GLCDC_Stop(&g_display_ctrl);
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        /* GLCDC initialization failed  */
        APP_ERR_PRINT("\r\n ** GLCDC driver stop FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
    else
    {
        APP_PRINT("\r\n ** GLCDC driver stopped ** \r\n");
    }

    /* Close GLCDC */
    err = R_GLCDC_Close(&g_display_ctrl);
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        /* GLCDC initialization failed  */
        APP_ERR_PRINT("\r\n ** GLCDC driver close FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
    else
    {
        APP_PRINT("\r\n ** GLCDC driver closed ** \r\n");
    }
#endif
}

/*******************************************************************************************************************//**
 * User defined function to initialize glcdc module
 *
 * @param      none
 * @retval     none
 **********************************************************************************************************************/
static void glcdc_init(void)
{
    fsp_err_t err = FSP_SUCCESS;
    /* Initialize GLCDC driver */
    err = R_GLCDC_Open(&g_display_ctrl, &g_display_cfg);
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        /* GLCDC initialization failed  */
        APP_ERR_PRINT("\r\n ** GLCDC driver initialization FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
    else
    {
        APP_PRINT("\r\n ** GLCDC driver initialization SUCCESS ** \r\n");
    }

#if defined(BOARD_RA8D1_EK)
    /* Initialize LCD. */
    mipi_dsi_push_table(g_lcd_init_focuslcd);
#endif

    /* Start GLCDC display output */
    err = R_GLCDC_Start(&g_display_ctrl);
    /* Handle error */
    if(FSP_SUCCESS != err)
    {
        /* GLCDC initialization failed  */
        APP_ERR_PRINT("\r\n ** GLCDC driver start FAILED ** \r\n");
        APP_ERR_TRAP(err);
    }
    else
    {
        APP_PRINT("\r\n ** GLCDC driver start SUCCESS ** \r\n");
    }
}

/*******************************************************************************************************************//**


/*******************************************************************************************************************//**
 * Callback functions for GLCDC interrupts
 *
 * @param[in]  p_args    Callback arguments
 * @retval     none
 **********************************************************************************************************************/
void glcdc_callback (display_callback_args_t * p_args)
{
    if (p_args->event == DISPLAY_EVENT_LINE_DETECTION)
    {
        g_vsync_flag = SET_FLAG;
    }
}



#if defined(BOARD_RA8D1_EK)

/*******************************************************************************************************************//**
 *  @brief       This function handles errors, closes all opened modules, and prints errors.
 *
 *  @param[in]   err       error status
 *  @param[in]   err_str   error string
 *  @retval      None
 **********************************************************************************************************************/
void handle_error (fsp_err_t err,  const char * err_str)
{
    if(FSP_SUCCESS != err)
    {
        /* Print the error */
        APP_ERR_PRINT(err_str);

        /* Close opened GLCD module*/
        if(RESET_VALUE != g_display_ctrl.state)
        {
            if(FSP_SUCCESS != R_GLCDC_Close (&g_display_ctrl))
            {
                APP_ERR_PRINT("GLCDC Close API failed\r\n");
            }
        }

        APP_ERR_TRAP(err);
    }
}



#endif
/*******************************************************************************************************************//**
 * @} (end addtogroup DRW_ep)
 **********************************************************************************************************************/
