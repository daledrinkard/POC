/*
         APPLICATION.C
		 
    Base app_entry() for application development
*/
#include "application_common.h"

#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
#include "hal_data.h"
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
#include "app_thread.h"
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#include "app_thread.h"
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#include "app_thread.h"
#endif

app_t App;

#if APP_HAS_CONSOLE
console_t *CN;
void Console_callback(console_event_t event, void *ctx);
#endif
#if APP_HAS_CONTROLPANEL
cpan_t *CP;
const cpan_t control_panel_initial = { 
        .stat = 0,
        .event = 0,
        .regs = {0},
};
#endif
/* static functions */
static int app_func_reset(void);
static int app_func_startup(void);
static int app_func_restart(void);
static uint32_t app_event_flag(bool);

/* define the initial condition of the application */
const app_t app_initial = {
		.state = APP_STATE_RESET,
};

void app_entry(void) {
	R_BSP_PinAccessEnable();
#if APP_HAS_CONTROLPANEL
    CP = CPAN_open(&control_panel_initial);  /* open the control panel */
#endif
#if APP_HAS_CONSOLE
    CN = RA_console_init("CON1", &g_console_uart, Console_callback, NULL);
#endif	
	memcpy(&App,&app_initial,sizeof(app_t));
	while (1) {
        switch (App.state) {
            case APP_STATE_RESET: /* initialize things */
            	App.state =  (0 == app_func_reset()) ? APP_STATE_STARTUP : APP_STATE_ERROR;
           	    break;
            case APP_STATE_STARTUP:  /* start things */
                App.state =  (0 == app_func_startup()) ? APP_STATE_RUNNING : APP_STATE_ERROR;
            	break;
            /* MAIN LOOP PROCESSING*/
            case APP_STATE_RUNNING:  /* process things */
                APP_INFO_PRINT("\nAPP RUNNING\n");
                /* TODO: add your own code here */
                do { /* hang in this state */
                    if (App.events & SYSFLG_APP_RESTART)
                    {
                        App.state = APP_STATE_RESTART;
                        App.events &= (uint32_t) ~SYSFLG_APP_RESTART;
                    }
                } while(App.state == APP_STATE_RUNNING);
                break;
            case APP_STATE_RESTART: /* shut down things and restart */
                App.state =  (0 == app_func_restart()) ? APP_STATE_STARTUP : APP_STATE_ERROR;
            	break;
            case APP_STATE_ERROR:
                APP_ERR_PRINT("\nAPP ERROR\n");
                while(1){}
                break;
	    
	    }
	}
}

static int app_func_startup(void)
{
    APP_INFO_PRINT("\nAPP STARTUP\n");
    return 0;
}
static int app_func_reset(void)
{
    APP_INFO_PRINT("\nAPP RESET\n");
    return 0;
}
static int app_func_restart(void)
{
    APP_INFO_PRINT("\nAPP RESTART\n");
    return 0;
}
static uint32_t app_event_flag(bool block)
{
    uint32_t flg;

    return flg;
}

/* _____      _ _ _                _      ______                _   _
  / ____|    | | | |              | |    |  ____|              | | (_)
 | |     __ _| | | |__   __ _  ___| | __ | |__ _   _ _ __   ___| |_ _  ___  _ __  ___
 | |    / _` | | | '_ \ / _` |/ __| |/ / |  __| | | | '_ \ / __| __| |/ _ \| '_ \/ __|
 | |___| (_| | | | |_) | (_| | (__|   <  | |  | |_| | | | | (__| |_| | (_) | | | \__ \
  \_____\__,_|_|_|_.__/ \__,_|\___|_|\_\ |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|___/
 */
#if APP_HAS_CONSOLE
void Console_callback(console_event_t event, void *ctx)
{
    // callback from console.
    switch(event) {
        case CONSOLE_NULL_EVENT:
            break;
        case CONSOLE_LF_EVENT:
            CP->p_console_string = (char*) ctx;
#if   (0 == BSP_CFG_RTOS) /* Bare METAL */
            CP->event |= CPAN_EVENT_CONSOLE;
#elif (1 == BSP_CFG_RTOS) /* Azure */
            tx_event_flags_set(&g_system_event,SYSFLG_CONSOLE,TX_OR);
#elif (2 == BSP_CFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (3 == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif

            break;
        default:;
    }
}
#endif

