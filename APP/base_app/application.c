/*
  BASE application.
*/
#include "application_common.h"



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
/* public functions */


const app_t app_initial = {
        .state = APP_STATE_RESET,
        .events = {0}
};

void app_entry(void) {
    uint32_t event_flag;
    R_BSP_PinAccessEnable();
    memcpy(&App,&app_initial,sizeof(app_t)); /* initialize the app */
#if   (APPCFG_RTOS == APPCFG_RTOS_NONE) /* Bare METAL */
    App.event = 0x00000000;
#elif (APPCFG_RTOS == APPCFG_RTOS_AZURE) /* Azure */
    UINT err;
    err = tx_event_flags_create(&App.events,(CHAR*) "app_event");
    while(TX_SUCCESS != err); //@@@ needs fixin...
#elif (APPCFG_RTOS == APPCFG_RTOS_FREERTOS) /* Fee RTOS */
#include "app_thread.h"
#elif (APPCFG_RTOS == APPCFG_RTOS_ZEPHYR) /* Zephyr */
#include "app_thread.h"
#endif

#if APP_HAS_CONTROLPANEL
    CP = CPAN_open(&control_panel_initial);  /* open the control panel */
#endif
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
                    /* USER code */
#if APP_HAS_CONSOLE
                    if (0 == app_event_flag_get(SYSFLG_CONSOLE_DATA,true,1,&event_flag))
                    {
                        /* at this point, data has been input through the console and is accessed by CP->p_console_string */
                        /* the default action is to execute this string a a command */
                        console_Exec(CP->p_console_string);
                    }
                    CPAN_POLL  /* service the control panel (if there is one) */
#endif
//!!                    R_BSP_SoftwareDelay(CP->regs[0], BSP_DELAY_UNITS_MILLISECONDS);
                    /* USER code end */
                    if (0 == app_event_flag_get(SYSFLG_APP_RESTART,true,1,&event_flag)) /*DWR timeout must be 1 */
                    {
                        App.state = APP_STATE_RESTART;
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
int app_event_flag_get(uint32_t msk,bool clr,uint32_t timeout,uint32_t *flgs)
{
    uint32_t flg;
#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    bool block = (timeout == 0) ? false : true;
    do {
        flg = App.events;
        if (block)
        {
            if ( (flg & msk) || (0 == timeout))
            {
                block = false;
            }
            else
            {
                if (0xFFFFFFFF != timeout)
                {
                    timeout--;
                    if (0 == timeout)
                    {
                        return -1;
                    }
                }
                R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
            }
        }
    } while(block);
    if (clr)
    {
        App.events &= ~msk;  /*note flg returns the bit that is cleared here */
    }
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    if (clr)
    {
        tx_event_flags_get(&App.events, msk, TX_OR_CLEAR,(ULONG*)  &flg, timeout);
    }
    else
    {
        tx_event_flags_get(&App.events, msk, TX_OR, (ULONG*) &flg, timeout);
    }
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#error needs implementing
#endif
    if (NULL != flgs)
    {
        *flgs = flg;
    }
    return 0;
}

int app_event_flag_set(uint32_t val,uint32_t *flgs)
{

#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    App.events |= val;
    if (NULL != flgs) *flgs = App.events;
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    tx_event_flags_set(&App.events,val,TX_OR);
    if (NULL != flgs)
    {
        tx_event_flags_get(&App.events,(ULONG) 0,TX_OR,(ULONG*) flgs,TX_NO_WAIT);
    }
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#error needs implementing
#endif
    return 0;
}
int app_event_flag_clr(uint32_t val,uint32_t *flgs)
{

#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    App.events &= ~val;
    if (NULL != flgs) *flgs = App.events;
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    tx_event_flags_set(&App.events,(ULONG) ~val,TX_AND);
    if (NULL != flgs)
    {
        tx_event_flags_get(&App.events,0,TX_OR,(ULONG*) flgs,TX_NO_WAIT);
    }
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#error needs implementing
#endif
    return 0;
}
int app_event_flag_geti(uint32_t msk,bool clr,uint32_t timeout,uint32_t *flgs)
{
//!!    uint32_t flg;
//!!    bool block = (timeout == 0) ? false : true;
#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    return app_event_flag_get(msk,clr,timeout,flgs);
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    return app_event_flag_get(msk,clr,timeout,flgs);
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#error needs implementing
#endif
}

int app_event_flag_seti(uint32_t val,uint32_t *flgs)
{

#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    return app_event_flag_set(val,flgs);
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    return app_event_flag_set(val,flgs);
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#error needs implementing
#endif

}
int app_event_flag_clri(uint32_t val,uint32_t *flgs)
{

#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    return app_event_flag_clr(val,flgs);
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    return app_event_flag_clr(val,flgs);
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#error needs implementing
#endif
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
            app_event_flag_seti(SYSFLG_CONSOLE_DATA, NULL);
            break;
        case CONSOLE_EVENT_CHAR:
         /* A single character is returned from the uart */
            /* USER */
            break;
        default:;
    }
}
#endif

