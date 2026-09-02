/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_sci_uart.h"
            #include "r_uart_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_flash_hp.h"
#include "r_flash_api.h"
FSP_HEADER
/** UART on SCI Instance. */
            extern const uart_instance_t      g_console_uart;

            /** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
            extern sci_uart_instance_ctrl_t     g_console_uart_ctrl;
            extern const uart_cfg_t g_console_uart_cfg;
            extern const sci_uart_extended_cfg_t g_console_uart_cfg_extend;

            #ifndef NULL
            void NULL(uart_callback_args_t * p_args);
            #endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_T0;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_T0_ctrl;
extern const timer_cfg_t g_T0_cfg;

#ifndef T0_cb
void T0_cb(timer_callback_args_t * p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_T1;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_T1_ctrl;
extern const timer_cfg_t g_T1_cfg;

#ifndef NULL
void NULL(timer_callback_args_t * p_args);
#endif
/* Flash on Flash HP Instance */
extern const flash_instance_t g_dataflash;

/** Access the Flash HP instance using these structures when calling API functions directly (::p_api is not used). */
extern flash_hp_instance_ctrl_t g_dataflash_ctrl;
extern const flash_cfg_t g_dataflash_cfg;

#ifndef dataflash_cb
void dataflash_cb(flash_callback_args_t * p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
