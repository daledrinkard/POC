/*
 *        CPAN.C
 *  
 *  Control Panel base.
 *  
 */
#include "cpan.h"
volatile cpan_t ControlPanel = {0};

#if APP_HAS_CMD_SHELL
const char* CPAN_stat_str[] = {
	"Update",
	"Restart App",
	/* USER stat bits names */
NULL };
#endif

cpan_t* CPAN_open(const cpan_t* p)
{
    if (NULL != p)
    {
        memcpy((cpan_t*) &ControlPanel,p,sizeof(cpan_t));
    }
    return (cpan_t*) &ControlPanel;
}
void CPAN_clear(void)
{
    /* USER code */
}
void CPAN_update(void)
{
    /* USER code */
    ControlPanel.stat &= (uint32_t) ~CPAN_STAT_UPDATE;
}
