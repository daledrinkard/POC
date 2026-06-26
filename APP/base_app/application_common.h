#ifndef APPLICATION_COMMON_H_
#define APPLICATION_COMMON_H_

#include <stdint.h>

#define APP_HAS_DEBUG_IO     (0)  /* adds the ability to POP, DROP and TOG gpio pins for logic analyzer debugging */
#define APP_HAS_CONTROLPANEL (0)  /* adds a control panel for runtime manipulation of components */
#define APP_HAS_CONSOLE      (0)  /* adds a console to support prints to UART and input command parsing */
#define APP_HAS_CMD_SHELL    (0)  /* adds a command shell to support runtime commands */


#if APP_HAS_CONSOLE
#include "console/console_port.h"
#if APP_HAS_CMD_SHELL
#include "commands/commands.h"
#endif
#define APP_ASSERT(_a_,_b_,_c_) if(_a_) {console_Print(_b_); console_Print("\n");} else {console_Print(_c_); console_Print("\n"); assert(false);}
#define APP_INFO_PRINT(_a_) console_Print(_a_)
#define APP_ERR_PRINT(_a_) { console_SetColor(CONSOLE_COLOR_RED); console_Print(_a_); console_SetColor(CONSOLE_COLOR_NORMAL); }
#else
#define APP_ASSERT(_a_,_b_,_c_) assert(_a_)
#define APP_INFO_PRINT(_a_)
#define APP_ERR_PRINT(_a_)
#endif



/* BUILD MODULE OPTIONS */
//-->#define APPCFG_BUILD_xxx       (1)

/* BUILD CONFIGURATION OPTIONS */
#define APPCFG_RTOS_NONE     (0)
#define APPCFG_RTOS_AZURE    (1)
#define APPCFG_RTOS_FREERTOS (2)
#define APPCFG_RTOS_ZEPHYR   (3)
#define APPCFG_RTOS          ( BSP_CFG_RTOS)
//-->#define APPCFG_xxx             (1)

/* define buffer placements */
//-->#define APPCFG_ATTRIB_xxx     BSP_ALIGN_VARIABLE(16) BSP_PLACE_IN_SECTION(".sdram")          //@@@ BSP_PLACE_IN_SECTION(".ram_nocache")

/* define attributes of the system */
//-->#define APPCFG_USB_IDLE_SLEEP  (100)  /* how many ticks to sleep whilst waiting to be told to start or check for detach */

/*  SYSTEM FLAG DEFINITIONS */
#define SYSFLG_APP_RESTART (0x00000001)
#if APP_HAS_CONSOLE
#define SYSFLG_CONSOLE_DATA (0x00000002)
#endif

//-->#define SYSFLG_xxx    (0x00000001)
#if APP_HAS_CMD_SHELL
const char* APP_SYSFLG_ansi[] = {
	"APP RESTART",
#if APP_HAS_CONSOLE
    "CONSOLE DATA",
#endif
/* USER */
    NULL
};
#endif

/* APPLICATION STATE MACHINE */
typedef enum app_state_e {
    APP_STATE_RESET,
    APP_STATE_STARTUP,
    APP_STATE_RUNNING,
    APP_STATE_RESTART,
    APP_STATE_ERROR,
	APP_STATE_SLEEP
} app_state_t;



#if APP_HAS_CONTROLPANEL
#define CPAN_POLL {	if (ControlPanel.stat & CPAN_STAT_UPDATE) CPAN_update();}
#else
#define CPAN_POLL {	}

#endif

/* application context */
typedef struct s_app {
    app_state_t state;
#if   (APPCFG_RTOS_NONE == APPCFG_RTOS) /* Bare METAL */
    uint32_t events;
#elif (APPCFG_RTOS_AZURE == APPCFG_RTOS) /* Azure */
#error needs implementing
#elif (APPCFG_RTOS_FREERTOS == APPCFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == APPCFG_RTOS) /* Zephyr */
#error needs implementing
#endif	
   /* USER FIELDS */
} app_t;
extern app_t App;

#if APP_HAS_DEBUG_IO
#include "POP/pop.h"
#endif
#if APP_HAS_CONTROLPANEL
#include "CPAN/cpan.h"
#endif


#endif /* APPLICATION_COMMON_H_ */










/*
 *    Some notes about some stuff:
 *
 *    To put the stack into DTCM, add a section in the Linker sections tab:
 *    *(.bss_g_main_stack)   DTCM Uninitialized
 *    *(,stack,new_thread0)  DTCM Zeroed
 *
 *    Then, OFS1_SEC  Enable,
 *    Tightly Coupled Memory (TCM)/Cache ECC      Disable ECC for TCM and Cache
 *
 *    To add the DMA to the USB stack:
 *    USB properties DMA/DTC support =  Enabled
 *    add the DMA element on the left
 *    transfer size = 4-bytes,
 *    Activation source = USBHS FIFO 1 (DMA transfer request 1)
 *    DMA on the right
 *    transfer size = 4 bytes
 *
 *    Priority 0 (Highest)   <--- Can PREEMPT this thread
      ...
Priority 14            <--- Can PREEMPT this thread
----------------------- PREEMPTION THRESHOLD (Set to 15)
Priority 15            <--- BLOCKED from preempting
      ...
Priority 19            <--- BLOCKED from preempting
Priority 20 (Actual)   <--- CURRENT RUNNING THREAD
Priority 21+ (Lowest)  <--- Cannot run anyway
 *
 *
 *
 *
 *
 *
 *
 *
 */
