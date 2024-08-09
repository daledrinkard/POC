/*
 * glcd.c
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#include "glcd.h"
#include "mipi.h"
#include "testbench/testbench_api.h"
/*******************************************************************************************************************//**
 * User defined function to initialize glcdc module
 *
 * @param      none
 * @retval     none
 **********************************************************************************************************************/
 void glcd_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    display_instance_t *p_dsply = TB.driver->p_GLCDC;
    err = p_dsply->p_api->open(p_dsply->p_ctrl,p_dsply->p_cfg);
    /* Initialize GLCDC driver */
//    err = R_GLCDC_Open(&g_display_ctrl, &g_display_cfg);
    assert(err == FSP_SUCCESS);
#if defined(BOARD_RA8D1_EK)
    /* Initialize LCD. */
    mipi_dsi_push_table(g_lcd_init_focuslcd);
#endif
    err = p_dsply->p_api->start(p_dsply->p_ctrl);
    assert(err == FSP_SUCCESS);
}
// err = R_GLCDC_BufferChange(&g_display_ctrl, (uint8_t*) Gvar.p_framebuffer, DISPLAY_FRAME_LAYER_1);

void glcd_swap(void)
{
//    display_in_format_t v;
    uint32_t* base = TB.driver->p_GLCDC->p_cfg->input[0].p_base;
    TB.p_activeframe = (TB.p_activeframe == base) ? (base + TB.framesize) : base;
    TB.driver->p_GLCDC->p_api->bufferChange(TB.driver->p_GLCDC->p_ctrl,TB.p_activeframe, DISPLAY_FRAME_LAYER_1);
}
/*******************************************************************************************************************//**
 *  @brief       This function handles errors, closes all opened modules, and prints errors.
 *
 *  @param[in]   err       error status
 *  @param[in]   err_str   error string
 *  @retval      None
 **********************************************************************************************************************/
void glcd_handle_error (fsp_err_t err,  const char * err_str)
{
#if 0
    if(FSP_SUCCESS != err)
    {
        /* Print the error */
        APP_ERR_PRINT(err_str);

        /* Close opened GLCD module*/
//        if(0 != g_display_ctrl.state)
            if(0 != ((glcdc_instance_ctrl_t*) TB.driver->p_GLCDC->p_ctrl)->state)
        {
            TB.driver->p_GLCDC->p_api->open
                if(FSP_SUCCESS != R_GLCDC_Close (&g_display_ctrl))

            {
                APP_ERR_PRINT("GLCDC Close API failed\r\n");
            }
        }

        APP_ERR_TRAP(err);
    }
#endif
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
        TB.event_flag |= TB_EVENT_VSYNC;
    }
}
