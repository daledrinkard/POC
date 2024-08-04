/*
 * draw_core.c
 *
 *  Created on: Jun 21, 2024
 *      Author: daled
 */


#include "draw_core.h"
#include "common_data.h"
#define D2_OK (0)  // don't know why I have to do this???
extern volatile uint32_t fxmode;
/*
 *   PRIVATE declarations
 */
static void drw_init(void);
static d2_device * gp_davey;
static volatile uint32_t g_message_sent;
/* Variables for different figures */
/* Circle variables */
static d2_point g_c_c_x1 = CIRCLE_VAL_X, g_c_c_y1 = CIRCLE_VAL_Y, g_c_r = CIRCLE_VAL_R, g_c_w = CIRCLE_VAL_W;
static bool g_c_left_flag = true, g_c_right_flag = false;
/* Triangle variables */
static d2_point g_t_x1 = TRIANGLE_X1, g_t_y1 = TRIANGLE_Y1, g_t_x2 = TRIANGLE_X2, g_t_y2 = TRIANGLE_Y2, g_t_x3 = TRIANGLE_X3, g_t_y3 = TRIANGLE_Y3;
static bool g_t_top_flag = true, g_t_bottom_flag = false;
/* Box variables */
static d2_point g_b_x1 = BOX_VAL_X1, g_b_y1 = BOX_VAL_Y1, g_b_l = BOX_VAL_LEN, g_b_w = BOX_VAL_WIDTH;
static bool g_p_top_flag = false, g_p_bottom_flag = true;
/* line variables */
static d2_point g_l_x1 = LINE_VAL_X1, g_l_y1 = LINE_VAL_Y1, g_l_x2 = LINE_VAL_X2, g_l_y2 = LINE_VAL_Y2, g_l_w = LINE_VAL_WIDTH;
static bool g_l_left_flag = false, g_l_right_flag = true;
volatile uint8_t g_vsync_flag = 0;
//static d2_s32             DRW_err;

/*
 *    PRIVATE functions
 */
volatile void * becky;
static void drw_init(void)
{
    d2_device *pp;
    /* Open the D2 driver and initialize the hardware */
    gp_davey = (void*) d2_opendevice(BITFIELD_VALUE);

//    (d2_device*) gp_davey = d2_opendevice(BITFIELD_VALUE);
//    gp_davey = becky;
    assert(D2_OK == d2_inithw(gp_davey, BITFIELD_VALUE));
    /* set the transparency and the alpha mode */
    assert(D2_OK == d2_setalpha(gp_davey, ALPHA_VALUE));
    assert(D2_OK == d2_setalphamode(gp_davey, d2_am_constant));
    assert(D2_OK == d2_setblendmode(gp_davey, d2_bm_alpha, d2_bm_one_minus_alpha));
}
static void drw_animation(void)
{
    /* circle animation */
    if((g_c_c_y1 < TRIANGLE_Y2) && (g_c_left_flag == true))
    {
        g_c_c_y1 = g_c_c_y1 + 2;
    }
    if(g_c_c_y1 >= TRIANGLE_Y2)
    {
        g_c_right_flag = true;
        g_c_left_flag = false;
    }

    if((g_c_c_y1 > CIRCLE_VAL_Y) && (g_c_right_flag == true))
    {
        g_c_c_y1 = g_c_c_y1 - 2;
    }
    if(g_c_c_y1 <= CIRCLE_VAL_Y)
    {
        g_c_left_flag = true;
        g_c_right_flag = false;
    }

    /* line animation */
    if((g_l_y1 > BOX_VAL_Y1) && (g_l_right_flag == true))
    {
        g_l_y1 = g_l_y1 - 2;
        g_l_y2 = g_l_y2 - 2;
    }
    if(g_l_y2 <= BOX_VAL_Y1)
    {
        g_l_left_flag = true;
        g_l_right_flag = false;
    }

    if((g_l_y1 < LINE_VAL_Y1) && (g_l_left_flag == true))
    {
        g_l_y1 = g_l_y1 + 2;
        g_l_y2 = g_l_y2 + 2;
    }
    if(g_l_y1 >= LINE_VAL_Y1)
    {
        g_l_right_flag = true;
        g_l_left_flag = false;
    }

    /* triangle animation */
    if((g_t_x1 <= TRIANGLE_X1) && (g_t_top_flag == true))
    {
        g_t_x1++;
        g_t_x2++;
        g_t_x3++;
    }
    if(g_t_x1 > TRIANGLE_X1)
    {
        g_t_top_flag = false;
        g_t_bottom_flag = true;
    }

    if((g_t_x1 >= LINE_VAL_X2) && (g_t_bottom_flag == true))
    {
        g_t_x1--;
        g_t_x2--;
        g_t_x3--;
    }
    if(g_t_x1 < LINE_VAL_X2)
    {
        g_t_top_flag = true;
        g_t_bottom_flag = false;
    }

    /* Polygon animation */
    if((g_b_x1 > BOX_VAL_X1) && (g_p_bottom_flag == true))
    {
        g_b_x1--;
    }
    if(g_b_x1 <= BOX_VAL_X1)
    {
        g_p_top_flag = true;
        g_p_bottom_flag = false;
    }

    if((g_b_x1 < BOX_PARAM) && (g_p_top_flag == true))
    {
        g_b_x1++;
    }
    if(g_l_y1 < BOX_PARAM)
    {
        g_p_bottom_flag = true;
        g_p_top_flag = false;
    }

}
static void drw_set(uint32_t * framebuffer)
{
    /* Start a frame */
    assert(D2_OK == d2_startframe(gp_davey));

    /* Set the frame buffer */
    assert(D2_OK == d2_framebuffer(gp_davey, framebuffer, DRW_TEST_IMAGE_RES_H, DRW_TEST_IMAGE_RES_H, DRW_TEST_IMAGE_RES_V, d2_mode_rgb888));
    /* Clear the frame buffer */
    assert(D2_OK == d2_clear(gp_davey, BUFFER_CLEAR_VAL));
    /* Set anti-aliasing and line cap settings */
    assert(D2_OK == d2_setantialiasing(gp_davey, ANTI_ALIASING_VAL));
    assert(D2_OK == d2_setlinecap(gp_davey, d2_lc_round));
    assert(D2_OK == d2_setlinejoin(gp_davey, d2_lj_none));

    /* Set render mode */
    assert(D2_OK == d2_selectrendermode(gp_davey , d2_rm_solid));
}
static void display_draw (uint32_t * framebuffer)
{
    /* DRW operations happens here */
    /*Animation of different figures */
    d2_s32 DRW_err;
    drw_animation();  // move starting point around

    /* set frame buffer properties */
    drw_set(framebuffer);

    /* Set the render color to red */
    assert(D2_OK == d2_setcolor(gp_davey, ARRAY_INDEX, RED_COLOR_VAL));

    /* Render a circle */
    assert(D2_OK == d2_rendercircle(gp_davey,(d2_point)(g_c_c_x1 << SHIFT_VALUE), (d2_point)(g_c_c_y1 << SHIFT_VALUE), (d2_width)(g_c_r << SHIFT_VALUE), (d2_width)(g_c_w << SHIFT_VALUE)));

    /* Set the render color to green */
    assert(D2_OK == d2_setcolor(gp_davey, ARRAY_INDEX, GREEN_COLOR_VAL));

    /* Render a triangle */
    assert(D2_OK == d2_rendertri(gp_davey, (d2_point)( g_t_x1 << SHIFT_VALUE),(d2_point)( g_t_y1 << SHIFT_VALUE),(d2_point)( g_t_x2 << SHIFT_VALUE),(d2_point)( g_t_y2 << SHIFT_VALUE),(d2_point)( g_t_x3 << SHIFT_VALUE),(d2_point)( g_t_y3 << SHIFT_VALUE),d2_edge2_shared));

    /* Set the render color to blue */
    assert(D2_OK == d2_setcolor(gp_davey, ARRAY_INDEX, BLUE_COLOR_VAL));

    /* Render a box */
    assert(D2_OK == d2_renderbox(gp_davey,(d2_point)(g_b_x1 << SHIFT_VALUE), (d2_point)(g_b_y1 << SHIFT_VALUE), (d2_width)(g_b_l << SHIFT_VALUE), (d2_width)(g_b_w << SHIFT_VALUE)));

    /* Set the render color to yellow */
    assert(D2_OK == d2_setcolor(gp_davey, ARRAY_INDEX, YELLOW_COLOR_VAL));

    /* Render a line */
    assert(D2_OK == d2_renderline(gp_davey, (d2_point) (g_l_x1 << SHIFT_VALUE), (d2_point) (g_l_y1 << SHIFT_VALUE), (d2_point) (g_l_x2 << SHIFT_VALUE), (d2_point) (g_l_y2 << SHIFT_VALUE),
                            (d2_width) (g_l_w << SHIFT_VALUE), d2_le_exclude_both));

    /* End the display list, then call d2_start frame to begin execution */
    assert(D2_OK == d2_endframe(gp_davey));
}
/*
 *    PUBLISHED functions
 */
draw_core_ctrl_t DrawCore BSP_PLACE_IN_SECTION(".dtcm_data");
int draw_core_init(draw_core_cfg_t *q)
{
    DrawCore.fb0 = q->fb0;
    g_vsync_flag = 1;
    drw_init();
    return 0;
}
int draw_core_run(uint32_t handle)
{
    switch(handle) {
        case 0: display_draw(DrawCore.fb0);
        break;
    }
    return 0;
}

/*
 *    CALLBACK functions
 */
void glcdc_callback (display_callback_args_t * p_args)
{
    if (p_args->event == DISPLAY_EVENT_LINE_DETECTION)
    {
        g_vsync_flag = 1; //SET_FLAG;
        fxmode++;
    }
}
/*******************************************************************************************************************//**
 * @brief      Callback functions for MIPI DSI interrupts
 *
 * @param[in]  p_args    Callback arguments
 * @retval     none
 **********************************************************************************************************************/
void mipi_dsi_callback(mipi_dsi_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case MIPI_DSI_EVENT_SEQUENCE_0:
        {
            if (MIPI_DSI_SEQUENCE_STATUS_DESCRIPTORS_FINISHED == p_args->tx_status)
            {
                g_message_sent = 1; //SET_FLAG;
            }
            break;
        }
        default:
        {
            break;
        }

    }
}
