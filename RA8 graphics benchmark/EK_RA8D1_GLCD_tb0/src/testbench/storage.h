/*
 * storage.h
 *
 *  Created on: Aug 15, 2024
 *      Author: daled
 */

#ifndef TESTBENCH_STORAGE_H_
#define TESTBENCH_STORAGE_H_

#include "common_data.h"
#include "testbench_cfg.h"
#include "draw_core/draw_core.h"

extern uint32_t Sample_Buffer_Animate[];
extern uint32_t Sample_Buffer_Render[];
extern uint32_t Sample_Buffer_VSYNC[];
extern uint32_t Sample_Buffer_ASYNC[];

extern char TBRXBuffer[];
extern char TBTXBuffer[];

extern dr_animate_t Animate[];
extern dr_render_t Render[];


#endif /* TESTBENCH_STORAGE_H_ */
