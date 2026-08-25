/*
  BASE application.
  This is the base application driver for the APP examples framework.
  
*/
#include "application_common.h"
#if   (APPCFG_RTOS == APPCFG_RTOS_NONE) /* Bare METAL */
#elif (APPCFG_RTOS == APPCFG_RTOS_AZURE) /* Azure */
#include "app_thread.h"
#elif (APPCFG_RTOS == APPCFG_RTOS_FREERTOS) /* Fee RTOS */
#include "app_thread.h"
#elif (APPCFG_RTOS == APPCFG_RTOS_ZEPHYR) /* Zephyr */
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
/* weak functions */
int app_func_reset   (void);
int app_func_startup (void);
int app_func_restart (void);
int app_func_run     (void);
void app_func_error   (int);
/* public functions */


/* application initializer */
const app_t app_initial = {
        .state = APP_STATE_RESET,
        .events = {0}
};

/**
      Application entry function.
	  Call this from hal_entry or the application thread entry function.
	  
	  This function implements the application state machine.
**/
void app_entry(void) {
    uint32_t event_flag;
    R_BSP_PinAccessEnable();
    memcpy(&App,&app_initial,sizeof(app_t)); /* initialize the app */
#if   (APPCFG_RTOS == APPCFG_RTOS_NONE) /* Bare METAL */
    App.events = 0x00000000;
#elif (APPCFG_RTOS == APPCFG_RTOS_AZURE) /* Azure */
    UINT err;
    err = tx_event_flags_create(&App.events,(CHAR*) "app_event");
    while(TX_SUCCESS != err); //@@@ needs fixin...
#elif (APPCFG_RTOS == APPCFG_RTOS_FREERTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS == APPCFG_RTOS_ZEPHYR) /* Zephyr */
#error needs implementing
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
                do { /* hang in this state */
                    /* USER code */
                    app_func_run();
#if APP_HAS_CONTROLPANEL
                    CPAN_POLL  /* service the control panel */
#endif
#if APP_HAS_CONSOLE
                    if (0 == app_event_flag_get(SYSFLG_CONSOLE_DATA,APP_FLAG_OR_CLEAR,0,&event_flag))
                    {
                        /* at this point, data has been input through the console and is accessed by CP->p_console_string */
                        /* the default action is to execute this string a a command */
                        console_Exec(CP->p_console_string);
                    }
#endif
                    /* USER code end */
                    if (0 == app_event_flag_get(SYSFLG_APP_RESTART,APP_FLAG_OR_CLEAR,0,&event_flag))
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

BSP_WEAK_REFERENCE int app_func_startup(void)
{
    APP_INFO_PRINT("\nAPP STARTUP\n");
    return 0;
}
BSP_WEAK_REFERENCE int app_func_reset(void)
{
    APP_INFO_PRINT("\nAPP RESET\n");
    return 0;
}
BSP_WEAK_REFERENCE int app_func_restart(void)
{
    APP_INFO_PRINT("\nAPP RESTART\n");
    return 0;
}
BSP_WEAK_REFERENCE int app_func_run(void)
{
    return 0;
}
BSP_WEAK_REFERENCE void app_func_error(int err)
{
    FSP_PARAMETER_NOT_USED(err);
    /** **/
}
int app_event_flag_get(uint32_t msk,app_flag_ctl_t ctl, uint32_t timeout,uint32_t *flgs)
{
#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    uint32_t flg;
    uint32_t mf;
    bool clear = ((ctl == APP_FLAG_OR_CLEAR) || (ctl == APP_FLAG_AND_CLEAR)) ? true : false;
    bool match = false;
    do {
        flg = App.events;
        if (NULL != flgs)
        {
            *flgs = flg;
        }
        mf = (flg & msk);
        switch(ctl) {
            case APP_FLAG_OR:
            case APP_FLAG_OR_CLEAR:
                match = (0 != mf) ? true : false;
                break;
            case APP_FLAG_AND:
            case APP_FLAG_AND_CLEAR:
                match = (msk == mf) ? true : false;
                break;
        }
        if (match && clear)
        {
            App.events &= ~mf;  /*note flg returns the bit that is cleared here */
        }
        if (match) return 0;
        if (0 == timeout) return -1;
        if (0 != timeout) timeout--;
        R_BSP_SoftwareDelay(1,BSP_DELAY_UNITS_MILLISECONDS);
    } while(1);

#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    ULONG flg;
    ULONG err;
    switch(ctl) {
        case APP_FLAG_OR:
            err = tx_event_flags_get(&App.events, msk, TX_OR,     (ULONG*)  &flg, timeout);
            break;
        case APP_FLAG_OR_CLEAR:
            err = tx_event_flags_get(&App.events, msk, TX_OR_CLEAR, (ULONG*)  &flg, timeout);
            break;
        case APP_FLAG_AND:
            err = tx_event_flags_get(&App.events, msk, TX_AND,       (ULONG*)  &flg, timeout);
            break;
        case APP_FLAG_AND_CLEAR:
            err = tx_event_flags_get(&App.events, msk, TX_AND_CLEAR, (ULONG*)  &flg, timeout);
            break;
        default: {while(1);}//@@TRAP
    }
    if (TX_SUCCESS != err) return -1;
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
void app_delay_ms(uint32_t t)
{
#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    R_BSP_SoftwareDelay(t, BSP_DELAY_UNITS_MILLISECONDS);
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
    tx_thread_sleep(t);
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

