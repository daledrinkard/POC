/*
 * draw_core.c
 *
 *  Created on: Aug 6, 2024
 *      Author: daled
 */

#include "common_utils.h"
#include <stdbool.h>
#include "draw_core_api.h"

//dr_object_t circle = {.X = 100,.Y = 100,.color = 0xFF00FF00,.object_type = Circle};


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


dr_poly_t Poly = {
                  .coords = Points,
                  .deltas = Deltas,
                  .number = 6
};



//dr_cirlce_t Circle1 = {.center = &CircleCenter,.radius = (50 << 4),.width = 0};
#include "dave_driver.h"
d2_device * gp_davey;
d2_s32             DRW_err;

static void error_handler(d2_s32 err, d2_device * gp_dave);
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


/* User defined function to initialize drw module
 *
 * @param      none
 * @retval     none
 **********************************************************************************************************************/
void draw_core_init(void)
{
    /* Open the D2 driver and initialize the hardware */
    gp_davey = d2_opendevice(BITFIELD_VALUE);
    DRW_err = d2_inithw(gp_davey, BITFIELD_VALUE);
        error_handler(DRW_err, gp_davey);

    /* set the transparency and the alpha mode */
    DRW_err = d2_setalpha(gp_davey, ALPHA_VALUE);
        error_handler(DRW_err, gp_davey);

    DRW_err = d2_setalphamode(gp_davey, d2_am_constant);
        error_handler(DRW_err, gp_davey);

    /* Set alphablend mode */
    DRW_err = d2_setblendmode(gp_davey, d2_bm_alpha, d2_bm_one_minus_alpha);
        error_handler(DRW_err, gp_davey);
}
void draw_core_set(uint32_t * framebuffer)
{
    /* Start a frame */
    DRW_err = d2_startframe(gp_davey);
    error_handler(DRW_err, gp_davey);

    /* Set the frame buffer */
    DRW_err = d2_framebuffer(gp_davey, framebuffer, DRW_TEST_IMAGE_RES_H, DRW_TEST_IMAGE_RES_H, DRW_TEST_IMAGE_RES_V, d2_mode_rgb565);
    error_handler(DRW_err, gp_davey);
    /* Clear the frame buffer */
    DRW_err = d2_clear(gp_davey, BUFFER_CLEAR_VAL);
    error_handler(DRW_err, gp_davey);

    /* Set anti-aliasing and line cap settings */
    DRW_err = d2_setantialiasing(gp_davey, ANTI_ALIASING_VAL);
    error_handler(DRW_err, gp_davey);

    DRW_err = d2_setlinecap(gp_davey, d2_lc_round);
    error_handler(DRW_err, gp_davey);

    DRW_err = d2_setlinejoin(gp_davey, d2_lj_none);
    error_handler(DRW_err, gp_davey);

    /* Set render mode */
    DRW_err = d2_selectrendermode(gp_davey , d2_rm_solid);
    error_handler(DRW_err, gp_davey);
}
void draw_core_draw (uint32_t * framebuffer)
{
    /* DRW operations happens here */
    /*Animation of different figures */
//    draw_core_animate();

    /* set frame buffer properties */
    draw_core_set(framebuffer);

    /* Set the render color to red */
    DRW_err = d2_setcolor(gp_davey, ARRAY_INDEX, RED_COLOR_VAL);
    error_handler(DRW_err, gp_davey);

    DRW_err = d2_renderpolyline( gp_davey, Poly.coords, Poly.number, 32, d2_le_closed);
    /* Render a circle */
//    DRW_err = d2_rendercircle(gp_davey,(d2_point)(g_c_c_x1 << SHIFT_VALUE), (d2_point)(g_c_c_y1 << SHIFT_VALUE), (d2_width)(g_c_r << SHIFT_VALUE), (d2_width)(g_c_w << SHIFT_VALUE));
//    error_handler(DRW_err, gp_davey);
//    DRW_err = d2_rendercircle(gp_davey,Circle1.center->X,Circle1.center->Y, Circle1.radius, Circle1.width);
//    error_handler(DRW_err, gp_davey);

    /* Set the render color to green */
    DRW_err = d2_setcolor(gp_davey, ARRAY_INDEX, GREEN_COLOR_VAL);
    error_handler(DRW_err, gp_davey);

    /* Render a triangle */
    DRW_err = d2_rendertri(gp_davey, (d2_point)( g_t_x1 << SHIFT_VALUE),(d2_point)( g_t_y1 << SHIFT_VALUE),(d2_point)( g_t_x2 << SHIFT_VALUE),(d2_point)( g_t_y2 << SHIFT_VALUE),(d2_point)( g_t_x3 << SHIFT_VALUE),(d2_point)( g_t_y3 << SHIFT_VALUE),d2_edge2_shared);
    error_handler(DRW_err, gp_davey);

    /* Set the render color to blue */
    DRW_err = d2_setcolor(gp_davey, ARRAY_INDEX, BLUE_COLOR_VAL);
    error_handler(DRW_err, gp_davey);

    /* Render a box */
    DRW_err = d2_renderbox(gp_davey,(d2_point)(g_b_x1 << SHIFT_VALUE), (d2_point)(g_b_y1 << SHIFT_VALUE), (d2_width)(g_b_l << SHIFT_VALUE), (d2_width)(g_b_w << SHIFT_VALUE));
    error_handler(DRW_err, gp_davey);

    /* Set the render color to yellow */
    DRW_err = d2_setcolor(gp_davey, ARRAY_INDEX, YELLOW_COLOR_VAL);
    error_handler(DRW_err, gp_davey);

    /* Render a line */
    DRW_err = d2_renderline(gp_davey, (d2_point) (g_l_x1 << SHIFT_VALUE), (d2_point) (g_l_y1 << SHIFT_VALUE), (d2_point) (g_l_x2 << SHIFT_VALUE), (d2_point) (g_l_y2 << SHIFT_VALUE),
                            (d2_width) (g_l_w << SHIFT_VALUE), d2_le_exclude_both);
    error_handler(DRW_err, gp_davey);

    /* End the display list, then call d2_start frame to begin execution */
    DRW_err = d2_endframe(gp_davey);
    error_handler(DRW_err, gp_davey);
}

void draw_core_animate(void)
{
    /* circle animation */
    int i;
#if 1
    for(i=0;i<Poly.number*2;i++)
//    for(i=0;i<8;i++)
    {
        Poly.coords[i] += Poly.deltas[i];
        if (Poly.coords[i] <= 0)
        {
            Poly.coords[i] = 0;
            if (Poly.deltas[i] > 0)
            {
                Poly.deltas[i] = Poly.deltas[i] - 8;
            }
            Poly.deltas[i] = Poly.deltas[i] * (-1);
        }
        else
        if (i & 0x01) // this is a Y cord
        {
            if (Poly.coords[i] >= (DRW_TEST_IMAGE_RES_V << 4))
            {
                Poly.coords[i] = (DRW_TEST_IMAGE_RES_V << 4);
                Poly.deltas[i] = Poly.deltas[i] * (-1);
                if (Poly.deltas[i] > 0)
                {
                    Poly.deltas[i] = Poly.deltas[i] - 8;
                }
            }
        }
        else
        {
            if (Poly.coords[i] >= (DRW_TEST_IMAGE_RES_H << 4))
            {
                Poly.coords[i] = (DRW_TEST_IMAGE_RES_H << 4);
                Poly.deltas[i] = Poly.deltas[i] * (-1);
            }
            if (Poly.deltas[i] > 0)
            {
                Poly.deltas[i] = Poly.deltas[i] - 8;
            }
        }
    }
#endif
//    CircleCenter.X += CircleCenter.dX;
//    CircleCenter.Y += CircleCenter.dY;
//    if (CircleCenter.X >= (DRW_TEST_IMAGE_RES_H << 4))
//    {
//        CircleCenter.X = (DRW_TEST_IMAGE_RES_H << 4);
//        CircleCenter.dX = CircleCenter.dX * (-1);
//    }



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
#if 0
static void rbg_render565(void)
{
    /* generate the rendered image */

    //Variable to hold display configuration
    display_input_cfg_t const *p_input = &g_display.p_cfg->input[0];

    //pointer to frame buffer
    uint16_t * buf_ptr = (uint16_t *)g_framebuffer[0];

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
#endif
/*******************************************************************************************************************//**
 * Error handler for DRW.
 *
 * @param[in]  err       Error returned from DRW APIs
 * @param[in]  gp_dave   Pointer to device structure
 * @retval     none
 **********************************************************************************************************************/
static void error_handler(d2_s32 err, d2_device * gp_dave)
{
    if (D2_OK != err)
    {
        /* print the error message */
        APP_ERR_PRINT(d2_geterrorstring( gp_dave ));
        APP_ERR_TRAP(err);
    }
}
