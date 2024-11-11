/*
 * draw_core.c
 *
 *  Created on: Aug 6, 2024
 *      Author: daled
 */

#include "common_utils.h"
#include <stdbool.h>
#include "draw_core_api.h"
#include "../testbench/testbench_memory.h"
#include "../storage.h"

#include <math.h>
#define PI 3.141592654
//dr_object_t circle = {.X = 100,.Y = 100,.color = 0xFF00FF00,.object_type = Circle};



void draw_core_animate_add(uint16_t,dr_animate_t *);
void draw_core_animate_rem(dr_animate_t *);
uint16_t draw_core_render_add(dr_render_t *);
void draw_core_render_rem(dr_render_t *);
static void dc_move_points(dr_animate_t * p);
static void dc_move_group(dr_animate_t * p, uint32_t x);
static void dc_bounce_points(dr_animate_t * p);
static void dc_bounce_group(dr_animate_t * p, uint32_t x);

dr_animate_list_t AnimateList = {
                                 .add = draw_core_animate_add,
                                 .rem = draw_core_animate_rem,
                                 .idx = 0,
                                 .list = Animate};
dr_render_list_t RenderList = {
                               .add = draw_core_render_add,
                               .rem = draw_core_render_rem,
                               .idx = 0,
                               .list = Render};

//dr_cirlce_t Circle1 = {.center = &CircleCenter,.radius = (50 << 4),.width = 0};
#include "dave_driver.h"
d2_device * gp_davey;
d2_s32             DRW_err;

static void error_handler(d2_s32 err, d2_device * gp_dave);
#if 0
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
#endif

/* User defined function to initialize drw module
 *
 * @param      none
 * @retval     none
 **********************************************************************************************************************/
void draw_core_init(void)
{
    /* Open the D2 driver and initialize the hardware */
    gp_davey = d2_opendevice(BITFIELD_VALUE);
    DRW_err = d2_lowlocalmemmode(gp_davey,32,32);// dlistblockfactor, dlistblocks)
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
    int i = 0;
    d2_point *dp;
    dr_point_t center;
    d2_width radius;
    d2_width line_width;
    uint32_t n;
    draw_core_set(framebuffer);
    error_handler(DRW_err, gp_davey);
    while (RenderList.list[i].rtype > 0)
    {
        dp = RenderList.list[i].coords;
        n = RenderList.list[i].number;
        switch (RenderList.list[i].rtype) {
            case 1: // polygon
                DRW_err = d2_setcolor(gp_davey, ARRAY_INDEX, RenderList.list[i].color);
                DRW_err |= d2_renderpolyline( gp_davey, dp, n, 32, d2_le_closed);
                while(DRW_err); //@@
                break;
            case 2: // circle
                center.X = dp[0];
                center.Y = dp[1];
                radius   = (d2_width) dp[2];
                line_width = (d2_width) dp[3];
                DRW_err = d2_setcolor(gp_davey, ARRAY_INDEX, RenderList.list[i].color);
                DRW_err |=     DRW_err = d2_rendercircle(gp_davey, center.X, center.Y, radius, line_width);

                while(DRW_err); //@@
                break;
            default: while(1); //@@
        }
        i++;
    }


    /* Set the render color to green */
    DRW_err = d2_setcolor(gp_davey, ARRAY_INDEX, GREEN_COLOR_VAL);
    error_handler(DRW_err, gp_davey);
#if 0
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
#endif
    /* End the display list, then call d2_start frame to begin execution */
    DRW_err = d2_endframe(gp_davey);
    error_handler(DRW_err, gp_davey);
}

#define ZPX (0)
#define MPX (DRW_TEST_IMAGE_RES_H << 4)
#define ZPY (0)
#define MPY (DRW_TEST_IMAGE_RES_V << 4)


static void dc_bounce_balls(dr_animate_t *p);
static void dc_bounce_balls(dr_animate_t *p)
{
    // parse render list looking for balls.
    uint16_t i = 0;
    uint16_t j;
    while(RenderList.list[i].rtype > 0)
    {
        if (RenderList.list[i].rtype == rCircle)
        {
            j = 0;
            while(RenderList.list[j].rtype > rNothing)
            {
                if (RenderList.list[j].rtype == rCircle)
                {
                    // does this ball collide with ball i

                }

        }
        i++;
    }

}

static void dc_move_points(dr_animate_t * p)
{
    for (int j = 0; j < p->coord_size; j++)
    {
        p->coord[j].X += p->velocity[j].X;
        p->velocity[j].X += p->acceleration[j].X;
        p->coord[j].Y += p->velocity[j].Y;
        p->velocity[j].Y += p->acceleration[j].Y;
    }
}
static void dc_bounce_points(dr_animate_t * p)
{
    for (int j = 0; j < p->coord_size; j++)
    {
        // check for bounce.
        if (p->coord[j].X >= MPX)
        {
            p->coord[j].X = MPX - (p->coord[j].X - MPX);
            p->velocity[j].X = p->velocity[j].X * (-1);
        }
        if (p->coord[j].Y >= MPY)
        {
            p->coord[j].Y = MPY - (p->coord[j].Y - MPY);
            p->velocity[j].Y = p->velocity[j].Y * (-1);
        }
        if (p->coord[j].X <= ZPX)
        {
            p->coord[j].X = (ZPX - p->coord[j].X) + ZPX;
            p->velocity[j].X = p->velocity[j].X * (-1);
        }
        if (p->coord[j].Y <= ZPY)
        {
            p->coord[j].Y = (ZPY - p->coord[j].Y) + ZPY;
            p->velocity[j].Y = p->velocity[j].Y * (-1);
        }
    }
}
static void dc_move_group(dr_animate_t * p, uint32_t x)
{
    d2_point tX,tY,mX,mY,zX,zY;
    int j;
    // move only the first point.
    p->coord[0].X += p->velocity[0].X;
    p->velocity[0].X += p->acceleration[0].X;
    p->coord[0].Y += p->velocity[0].Y;
    p->velocity[0].Y += p->acceleration[0].Y;
    // calculate the delta
    tX = p->coord[0].X - tX;
    tY = p->coord[0].Y - tY;
    // move all the other points
    mX = p->coord[0].X;
    zX = p->coord[0].X;
    mY = p->coord[0].Y;
    zY = p->coord[0].Y;
    for(j=1;j < p->coord_size; j++)
    {
        p->coord[j].X += tX;
        p->coord[j].Y += tY;
        if (p->coord[j].X > mX)  mX = p->coord[j].X;
        else if (p->coord[j].X < zX) zX = p->coord[j].X;
        if (p->coord[j].Y > mY)  mY = p->coord[j].Y;
        else if (p->coord[j].Y < zY) zY = p->coord[j].Y;
    }

}
static void dc_bounce_group(dr_animate_t * p, uint32_t x)
{
    d2_point tX,tY,mX,mY,zX,zY;
    // check for bounce.
    tX = (mX >= MPX) ? (MPX - mX) : 0;
    tY = (mY >= MPY) ? (MPY - mY) : 0;
int j;
    if (mX >= MPX)
    {
        p->velocity[0].X = p->velocity[0].X * (-1);
        for( j=0;j < p->coord_size;j++)
        {
            p->coord[j].X = (d2_point) (p->coord[j].X + (tX * 2));
        }
    }
    if (mY >= MPY)
    {
        p->velocity[0].Y = p->velocity[0].Y * (-1);
        for( j=0;j < p->coord_size;j++)
        {
            p->coord[j].Y = (d2_point) (p->coord[j].Y + (tY * 2));
        }
    }
    tX = (mX <= ZPX) ? (ZPX - mX) : 0;
    tY = (mY <= ZPY) ? (ZPY - mY) : 0;

    if (zX < ZPX)
    {
        p->velocity[0].X = p->velocity[0].X * (-1);
        for(j=0;j < p->coord_size;j++)
        {
            p->coord[j].X = (d2_point) (p->coord[j].X + (tX * 2));
        }
    }
    if (zY < ZPY)
    {
        p->velocity[0].Y = p->velocity[0].Y * (-1);
        for(j=0;j < p->coord_size;j++)
        {
            p->coord[j].Y = p->coord[j].Y + (tY * 2);
        }
    }

}

void draw_core_animate(void)
{
    /* circle animation */
    int i,k;
    i = 0;
    uint32_t j;
    dr_animate_t * p;

    while (AnimateList.list[i].coord_size != 0)
    {
        p = &AnimateList.list[i];
        switch (AnimateList.list[i].atype) {
            case 0:  // move all points in the list independently
                dc_move_points(p);
                dc_bounce_points(p);
                break;
            case 1:  // move group as a whole with 0 = the anchor point
                dc_move_group(p,0);
                dc_bounce_group(p,0);
                break;
            default: k = 1; while(k);
        }
        switch (p->atimer) {
            case 1: //@@ do a callback
                p->atimer--;
                p->acceleration[0].X = 0;
                p->acceleration[0].Y = 0;
                break;
            case 0: // don't do anything
                break;
            default: p->atimer--;
        }

        i++;
    }
#if 0
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


#if 0
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
#endif
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
    volatile d2_char *pp;
    if (D2_OK != err)
    {
        /* print the error message */
        pp = d2_geterrorstring( gp_dave );
        APP_ERR_PRINT(pp);
        APP_ERR_TRAP(err);
    }
}


void draw_core_animate_add(uint16_t handle,dr_animate_t *p_animate)
{
   AnimateList.idx = 0;
   while(AnimateList.list[AnimateList.idx].coord_size > 0) //@@ need a better way to detect a null entry
       AnimateList.idx++; //@@ no checks
   memcpy((uint8_t*) &AnimateList.list[AnimateList.idx],(uint8_t*) p_animate, sizeof(dr_animate_t));
   // allocate a buffer for the velocity vectors
   MEM.allocate((d2_point **)(&AnimateList.list[AnimateList.idx].velocity), p_animate->velocity_size);
   memcpy((uint8_t*) AnimateList.list[AnimateList.idx].velocity,p_animate->velocity, p_animate->velocity_size * sizeof(dr_point_t));
   // allocate a buffer for the acceleration vectors
   MEM.allocate((d2_point **)(&AnimateList.list[AnimateList.idx].acceleration), p_animate->acceleration_size);
   memcpy((uint8_t*) AnimateList.list[AnimateList.idx].acceleration,p_animate->acceleration, p_animate->acceleration_size * sizeof(dr_point_t));




   RenderList.list[handle].animator = AnimateList.idx;

   AnimateList.list[AnimateList.idx].coord = RenderList.list[handle].coords;
}
void draw_core_animate_rem(dr_animate_t *)
{

}
uint16_t draw_core_render_add(dr_render_t *p_render)
{
    RenderList.idx = 0;
    while(RenderList.list[RenderList.idx].rtype > 0)
        RenderList.idx++; //@@ no checks
    // copy the object to the Render list
    memcpy((uint8_t*) &RenderList.list[RenderList.idx],(uint8_t*) p_render, sizeof(dr_render_t));
    // Now allocate storage for the coord set.
    MEM.allocate(&RenderList.list[RenderList.idx].coords,(uint32_t) RenderList.list[RenderList.idx].number);
    // Now initialize that set of coords.
    memcpy((uint8_t*) RenderList.list[RenderList.idx].coords,p_render->coords,p_render->number * sizeof(dr_point_t));
    //@@ should the initial animator be set to 0 here?
    return RenderList.idx;

}
void draw_core_render_rem(dr_render_t *)
{

}

static d2_point cvrt(double x);
static d2_point cvrt(double x)
{
    d2_point zz;
    int d,f;
    double q;
    double i;
    q = modf(x,&i);
    d = (int)x;
    f = (int) (1/q) * 16;
    zz = ((d & 0x000003FF) << 4) | f;
    return zz;
}
static double todbl(d2_point x);
static double todbl(d2_point x)
{
    d2_point zz;
    uint32_t d,f;
    double q;
    d = (x & 0x7FF0) >> 4;
    f = (x & 0x000F);
    q = (double) d + (double) 16 * f;
    return q;
}

dr_point_t* draw_core_star(dr_point_t center,d2_point r1, d2_point r2,uint16_t number)
{
    static dr_point_t *points;
    MEM.allocate(&points,number * 2);
    double x,y,theda,dtheda;
    dr_point_t P;
    int i;
    dtheda = (double) (2*PI)/number;
    theda = dtheda;
    for(i=0;i<number;i++)
    {
        x = cos(theda) * todbl(r1);
        y = sin(theda) * todbl(r1);
        P.X = cvrt(x);
        P.Y = cvrt(y);
        P.X = P.X + center.X;
        P.Y = P.Y + center.Y;
        points[i] = P;
        theda += dtheda;
    }



    return points;
}
