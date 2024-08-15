/*
 * draw_core.h
 *
 *  Created on: Aug 6, 2024
 *      Author: daled
 */

#ifndef DRAW_CORE_DRAW_CORE_H_
#define DRAW_CORE_DRAW_CORE_H_
#include "draw_core_cfg.h"
#include <stdint.h>
#include "dave_driver.h"
#define BITFIELD_VALUE          (0U)
#define RED_COLOR_VAL           (0xFF0000)
#define GREEN_COLOR_VAL         (0x00FF00)
#define BLUE_COLOR_VAL          (0x0000FF)
#define YELLOW_COLOR_VAL        (0xFFFF00)
#define ANTI_ALIASING_VAL       (1U)
#define BUFFER_CLEAR_VAL        (0x000000)
#define SHIFT_VALUE             (4U)

typedef enum e_object_type {Circle} object_type_t;
typedef struct dr_point_s {
    d2_point X,Y;
}dr_point_t;
typedef struct dr_animate_s {
    uint32_t atype;
    dr_point_t *coord;
    dr_point_t *velocity;
    dr_point_t *acceleration;
    uint32_t coord_size, velocity_size, acceleration_size;
}dr_animate_t;
typedef struct dr_poly_s {
    d2_point *coords;
//    d2_point *deltas;
    uint16_t number;
}dr_poly_t;
typedef struct dr_render_s {
    uint32_t rtype;  // 0 = no object 1 = polygon, 2 = circle, ..
    d2_point *coords;
//    d2_point *deltas;
    uint16_t number;
    uint16_t state;
    uint32_t color;
}dr_render_t;
typedef struct dr_circle_s {
    dr_point_t *center;
    d2_width radius;
    d2_width width;
}dr_cirlce_t;
typedef struct dr_animate_list_s {
    dr_animate_t *list;
    void (*add)(uint16_t,dr_animate_t*);
    void (*rem)(dr_animate_t*);
    uint16_t idx;
}dr_animate_list_t;
typedef struct rd_render_list_s {
   dr_render_t *list;
   uint16_t (*add)(dr_render_t*);
   void (*rem)(dr_render_t*);
   uint16_t idx;

}dr_render_list_t;

extern dr_animate_list_t AnimateList;
extern dr_render_list_t RenderList;

dr_point_t* draw_core_star(dr_point_t center,d2_point r1, d2_point r2,uint16_t number);
#endif /* DRAW_CORE_DRAW_CORE_H_ */
