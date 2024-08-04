/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_sci_b_uart.h"
#include "r_uart_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t g_aux_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_aux_timer_ctrl;
extern const timer_cfg_t g_aux_timer_cfg;

#ifndef aux_timer_cb
void aux_timer_cb(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_freerun_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_freerun_timer_ctrl;
extern const timer_cfg_t g_freerun_timer_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_sample_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_sample_timer_ctrl;
extern const timer_cfg_t g_sample_timer_cfg;

#ifndef sample_timer_cb
void sample_timer_cb(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_one_second_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_one_second_timer_ctrl;
extern const timer_cfg_t g_one_second_timer_cfg;

#ifndef code_timer_callback
void code_timer_callback(timer_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t g_printf_uart;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_b_uart_instance_ctrl_t g_printf_uart_ctrl;
extern const uart_cfg_t g_printf_uart_cfg;
extern const sci_b_uart_extended_cfg_t g_printf_uart_cfg_extend;

#ifndef printf_uart_callback
void printf_uart_callback(uart_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
