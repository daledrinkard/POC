/*
 * draw_core.h
 *
 *  Created on: Jun 21, 2024
 *      Author: daled
 */
#include <stdint.h>
#include "draw_core_cfg.h"
#include "dave_driver.h"

#ifndef DRAW_CORE_DRAW_CORE_H_
#define DRAW_CORE_DRAW_CORE_H_
//#include "hal_data.h"

//#define GLCDC_VSYNC_TIMEOUT     (1000)
//#define BYTES_PER_PIXEL         (2U)
#define ARRAY_INDEX             (0U)
#define BITFIELD_VALUE          (0U)
//#define RENDER_DELAY            (25U)
//#define SW_DELAY                (15U)
//#define RESET_FLAG              (0U)
//#define SET_FLAG                (1U)

#define ALPHA_VALUE             (155U)
#define ANTI_ALIASING_VAL       (1U)
#define BUFFER_CLEAR_VAL        (0x000000)
#define RED_COLOR_VAL           (0xFF0000)
#define GREEN_COLOR_VAL         (0x00FF00)
#define BLUE_COLOR_VAL          (0x0000FF)
#define YELLOW_COLOR_VAL        (0xFFFF00)
#define SHIFT_VALUE             (4U)

typedef struct draw_core_cfg_s {
    uint16_t *fb0;
    uint16_t *fb1;
    uint16_t horz;
    uint16_t vert;
    d2_device * gp_davey;
}draw_core_cfg_t;

typedef struct draw_core_ctrl_s {
    uint16_t *fb0;
    uint16_t *fb1;
}draw_core_ctrl_t;
/*
 *   API
 */
int draw_core_init(draw_core_cfg_t *);
int draw_core_run(uint32_t handle);
#endif /* DRAW_CORE_DRAW_CORE_H_ */
