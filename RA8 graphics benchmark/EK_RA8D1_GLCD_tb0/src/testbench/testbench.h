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
#define TB_EVENT_VSYNC   (0x01000000)
#define TB_EVENT_SW1     (0x00000020)
#define TB_EVENT_SW2     (0x00000040)
#define TB_EVENT_STARTUP (0x02000000)
#define TB_EVENT_RXMSG   (0x00800000)
#define TB_EVENT_ADONE   (0x00400000)
/* define the valid event bits at startup */
#define TB_MASK_DEFAULT  ( \
        TB_EVENT_TIMER0  | \
        TB_EVENT_TIMER1  | \
        TB_EVENT_TIMER2  | \
        TB_EVENT_TIMER3  | \
        TB_EVENT_VSYNC   | \
        TB_EVENT_SW1     | \
        TB_EVENT_SW2     | \
        TB_EVENT_STARTUP | \
        TB_EVENT_RXMSG  )

#define TB_STATE_POLL_STATUS (0x00000001)
#define TB_STATE_COMMAND     (0x00000002)
#define TB_STATE_MASK_DEFAULT ( \
        TB_STATE_POLL_STATUS    )
#define TB_NUMBER_OF_COMMANDS (16)

typedef struct testbench_command_s {
   char *name;
   char *help;
   void (*func)(char*,char**);
}testbench_command_t;
#endif /* TESTBENCH_TESTBENCH_H_ */
