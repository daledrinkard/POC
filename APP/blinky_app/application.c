/*
        BLINKY base application
		
	This application includes a control panel and blinks the LEDS
	under the control of the COntrolPanel.
	Users can suspend the program and use the ControlPanel to make
	changes at runtime.
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
#endif
/* static functions */
static int app_func_reset(void);
static int app_func_startup(void);
static int app_func_restart(void);

const app_t app_initial = {
		.state = APP_STATE_RESET,
};

/* For Blinky app, create a control panel for the leds.  */
extern bsp_leds_t g_bsp_leds;
cpan_t *CP;
const cpan_t control_panel_initial = { 
        .stat = 0,
        .regs = {0},
        /* USER */
        .leds = &g_bsp_leds  /* add the led structure */
};

void app_entry(void) {
	R_BSP_PinAccessEnable();
	memcpy(&App,&app_initial,sizeof(app_t)); /* initialize the app */
	CP = CPAN_open(&control_panel_initial);  /* open the control panel */
#if APP_HAS_CONSOLE
    CN = RA_console_init("CON1", &g_console_uart, Console_callback, NULL);
#endif		
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
#if APP_HAS_CONSOLE						
                    if (App.events & SYSFLG_CONSOLE_DATA)
                    {
                        /* USER code */
                        /* at this point, data has been input through the console and is accessed by CP->p_console_string */
                        /* the default action is to execute this string a a command */
                        console_Exec(CP->p_console_string);
						App.events &= (uint32_t) ~SYSFLG_CONSOLE_DATA;
                    }
                    CPAN_POLL  /* service the control panel (if there is one) */
#endif
                    R_BSP_SoftwareDelay(CP->regs[0], BSP_DELAY_UNITS_MILLISECONDS);
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
			case APP_STATE_SLEEP:
			    __DSB();
			    __WFI();
			    App.state = APP_STATE_RUNNING;
			    break;
	    }
	}
}

static int app_func_startup(void)
{
    APP_INFO_PRINT("\nAPP STARTUP\n");
    CP->regs[0] = 500;
    for(int i=0;i<CP->leds->led_count;i++)
    {
        CP->regs[1] |= (1 << i);
    }
    CP->led_state = CP->regs[1];
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
        case CONSOLE_EVENT_NULL: /* The console has nothing to say */
            break;
        case CONSOLE_EVENT_LF:  /* The Enter key was pressed.  ctx points to a null terminated string of characters */
            CP->p_console_string = (char*) ctx;
#if   (BSP_CFG_RTOS == APPCFG_RTOS_NONE) /* Bare METAL */
            App.events |= SYSFLG_CONSOLE_DATA;
#elif (BSP_CFG_RTOS == APPCFG_RTOS_AZURE) /* Azure */
            tx_event_flags_set(&App.events,SYSFLG_CONSOLE_DATA,TX_OR);
#elif (BSP_CFG_RTOS == APPCFG_RTOS_FREERTOS) /* Fee RTOS */
#error needs implementing
#elif (BSP_CFG_RTOS == APPCFG_RTOS_ZEPHYR) /* Zephyr */
#error needs implementing
#endif

            break;
        case CONSOLE_EVENT_CHAR:
         /* A single character is returned from the uart */
            /* USER */
            break;
        default:;
    }
}
#endif

