#include "application_common.h"
/* For Blinky app, create a control panel for the leds.  */
extern bsp_leds_t g_bsp_leds;
/* weak overrides */
int app_func_reset   (void);
int app_func_startup (void);
int app_func_restart (void);
int app_func_run     (void);
extern cpan_t *CP;
const cpan_t control_panel_initial = { 
                                      .stat = 0,
                                      .regs = {0},
                                      /* USER */
                                      .leds = &g_bsp_leds  /* add the led structure */
};
int app_func_reset   (void)
{
    APP_INFO_PRINT("\nBLINKY RESET\n");
    CP = CPAN_open(&control_panel_initial);  /* open the control panel */
    return (CP == NULL) ? -1 : 0;
}
int app_func_startup (void)
{
    APP_INFO_PRINT("\nnBLINKY STARTUP\n");
    CP->regs[0] = 500;
    for(int i=0;i<CP->leds->led_count;i++)
    {
        CP->regs[1] |= (1 << i);
    }
    CP->led_state = CP->regs[1];
    return 0;
}
int app_func_restart (void)
{
    return 0;
}
int app_func_run     (void)
{
    for(int i=0;i<CP->leds->led_count;i++) /* for each LED */
    {
        if (CP->regs[1] & (1 << i))        /* if it is enabled */
        {
            if (CP->led_state & (1 << i))
            {
                R_BSP_PinWrite(CP->leds->p_leds[i], BSP_IO_LEVEL_LOW);
            }
            else
            {
                R_BSP_PinWrite(CP->leds->p_leds[i], BSP_IO_LEVEL_HIGH);
            }
        }
    }
    CP->led_state ^= CP->regs[1]; // toggle the bits that are enabled
    app_delay_ms(CP->regs[0]);
    CPAN_POLL;
    return 0;
}

