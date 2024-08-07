/*
 * testbench.h
 *
 *  Created on: Aug 7, 2024
 *      Author: daled
 */

#ifndef TESTBENCH_TESTBENCH_H_
#define TESTBENCH_TESTBENCH_H_

#include "testbench_cfg.h"

#define TB_EVENT_TIMER0  (0x00000001)
#define TB_EVENT_TIMER1  (0x00000002)
#define TB_EVENT_TIMER2  (0x00000004)
#define TB_EVENT_TIMER3  (0x00000008)
#define TB_EVENT_VSYNC   (0x00000010)
#define TB_EVENT_STARTUP (0x01000000)

#endif /* TESTBENCH_TESTBENCH_H_ */
