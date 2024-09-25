/*
 * draw_core_api.h
 *
 *  Created on: Aug 6, 2024
 *      Author: daled
 */

#ifndef DRAW_CORE_DRAW_CORE_API_H_
#define DRAW_CORE_DRAW_CORE_API_H_

#include "draw_core.h"
extern d2_s32             DRW_err;
void draw_core_init(void);
void draw_core_draw (uint32_t * framebuffer) BSP_PLACE_IN_SECTION(".itcm");
void draw_core_set(uint32_t * framebuffer);
void draw_core_animate(void) BSP_PLACE_IN_SECTION(".itcm");

#endif /* DRAW_CORE_DRAW_CORE_API_H_ */
