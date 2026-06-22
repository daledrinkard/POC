/*
 * cpan.c
 *
 *  Created on: Dec 13, 2025
 *      Author: daled
 */
/*
 *   This is a control panel foe the AiZip LPR project
 */
#include "cpan.h"
volatile cpan_t ControlPanel = {0};

#if APP_HAS_CMD_SHELL
const char* CPAN_stat_str[] = {
	"Update",
	/* USER stat bits names */
NULL };
const char* CPAN_event_str[] = {
	"Console",
	/* USER event bits names */
NULL };
#endif

cpan_t* CPAN_open(const cpan_t* p)
{
    if (NULL != p)
        memcpy((cpan_t*) &ControlPanel,p,sizeof(cpan_t));
    return (cpan_t*) &ControlPanel;
}
void CPAN_clear(void)
{
}
void CPAN_update(void)
{
    ControlPanel.stat &= (uint32_t) ~CPAN_STAT_UPDATE;
}
