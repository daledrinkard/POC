/*
 *     CPAN.H
 *
 * Blinky Control Panel header file
 * modified from the base control panel by adding the leds
 *
 */

#ifndef CPAN_CPAN_H_
#define CPAN_CPAN_H_
#include <stdint.h>
#include "application_common.h"
/* USER INCLUDE */
//-->#include "bsp_api.h"

#define CPAN_NUM_OF_REGS          (2)
#define CPAN_STAT_UPDATE          (0x00000001)
#define CPAN_STAT_RESTART_APP     (0x00000002)
/* USER DEFINE */


#if APP_HAS_CMD_SHELL
extern char** CPAN_stat_str;    /* these are text names for bits that are used in the commands */
extern char** CPAN_udpate_str;
#endif

typedef struct cpan_s {

    uint32_t stat;
    uint32_t regs[CPAN_NUM_OF_REGS];
#if APP_HAS_CONSOLE
    char *p_console_string;
#endif
    /* USER SECTION */
    bsp_leds_t *leds;
    uint32_t led_state;	
}cpan_t;

extern volatile cpan_t ControlPanel;
cpan_t* CPAN_open(const cpan_t* p);
void CPAN_clear(void);
void CPAN_update(void);
/* USER ADDITIONAL PUBLIC FUNCTIONS */


#endif /* CPAN_CPAN_H_ */
