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
    d2_point dX,dY;
    uint8_t bounce;
}dr_point_t;
typedef struct dr_poly_s {
    d2_point *coords;
    d2_point *deltas;
    uint16_t number;
}dr_poly_t;
typedef struct dr_circle_s {
    dr_point_t *center;
    d2_width radius;
    d2_width width;
}dr_cirlce_t;


#endif /* DRAW_CORE_DRAW_CORE_H_ */
