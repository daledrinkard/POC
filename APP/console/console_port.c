/*
 * console_port.c
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */
/*
 *   CONSOLE_PORT.C
 *
 *   This is a port for the App console on RA using FSP HAL drivers for communications
 */


#include "console_port.h"
#if   (0 == BSP_CFG_RTOS) /* Bare METAL */
#include "hal_data.h"
#elif (CONSOLE_CONFIG_RTOS_AZURE    == BSP_CFG_RTOS)
#include <app_thread.h>
#elif (CONSOLE_CONFIG_RTOS_FREERTOS == BSP_CFG_RTOS)
#error needs implementing
#elif (CONSOLE_CONFIG_RTOS_ZEPHYR   == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif
   /* USER FIELDS */
/*
 *   PUBLIC
 */
console_t Console;
/*
 *   STATIC declarations
 */
static uart_instance_t *Uart;
static uart_callback_args_t RA_CB_args;
static void RA_console_cb(uart_callback_args_t*);

static int RA_uart_write(char* data,int len, void *ctx);
static int RA_uart_read(char* data,int len, void *ctx);
#if CONSOLE_CONFIG_COMMAND_MODE
static char command_buffer[CONSOLE_RX_BUFFER_LEN];
#endif

/*
 *    Port functions for RA
 */

console_t* RA_console_init(char *name, const uart_instance_t *uart,void* cb,void *sema)
{
    fsp_err_t err;
    err = uart->p_api->open(uart->p_ctrl,uart->p_cfg);
    err |= uart->p_api->callbackSet(uart->p_ctrl,&RA_console_cb,NULL,&RA_CB_args);
    if (FSP_SUCCESS == err)
    {
        strncpy(&Console.name[0],name,CONSOLE_NAME_SIZE);
        Uart = (uart_instance_t*) uart;
        Console.write = &RA_uart_write;
        Console.read = &RA_uart_read;
        // Console.commands = (command_t *) cmd;
        Console.cb = cb;
        Console.rx_idx = 0;
#if CONSOLE_CONFIG_COMMAND_MODE
        Console.rx_buffer = &command_buffer[0];
        Console.rx_size = sizeof(command_buffer);
#else
        Console.rx_buffer = 0;
        Console.rx_size = 0;
#endif
        if (NULL == sema)
        {
        	//@@@ create a semaphore
#if (CONSOLE_CONFIG_RTOS_FREERTOS == BSP_CFG_RTOS)
#elif (CONSOLE_CONFIG_RTOS_AZURE    == BSP_CFG_RTOS)
#elif (CONSOLE_CONFIG_RTOS_ZEPHYR   == BSP_CFG_RTOS)
#endif
        }
        else
        {
            Console.tx_done_sema = sema; //@@ where o
        }
        console_Open(&Console);
    }
    else return NULL;

    return &Console;
}


/*
 *     STATIC function definitions
 */
static int RA_uart_write(char* data,int len, void *ctx)
{
    FSP_PARAMETER_NOT_USED(ctx);
    return (FSP_SUCCESS == Uart->p_api->write(Uart->p_ctrl,(uint8_t*)  data,(uint32_t) len)) ? len : -1;
}
static int RA_uart_read(char* data,int len, void *ctx)
{
    //@@@ needs implementing
    FSP_PARAMETER_NOT_USED(data);
    FSP_PARAMETER_NOT_USED(len);
    FSP_PARAMETER_NOT_USED(ctx);
    return 0;
}

/*
 *   CALLBACK functions
 */
//@@@
//@@@    We really need to support RTOSes
//@@@
void RA_console_cb(uart_callback_args_t *p_args)
{
#if (BSP_CFG_RTOS == CONSOLE_CONFIG_RTOS_FREERTOS)
    BaseType_t xHigherPriorityTaskWoken, xResult;
    xHigherPriorityTaskWoken = pdFALSE;
#endif
    console_event_t h;

    switch(p_args->event) {
        case UART_EVENT_RX_COMPLETE:   // = (1UL << 0), ///< Receive complete event
            break;
        case UART_EVENT_TX_COMPLETE:   // = (1UL << 1), ///< Transmit complete event
#if   (CONSOLE_CONFIG_RTOS_NONE == BSP_CFG_RTOS) /* Bare METAL */
            Console.flags &= (uint32_t) ~CONSOLE_FLAG_TX_BUSY;
#elif (CONSOLE_CONFIG_RTOS_AZURE == BSP_CFG_RTOS) /* Azure */
    tx_semaphore_ceiling_put(&g_console_sema,1);
#elif (CONSOLE_CONFIG_RTOS_FREERTOS == BSP_CFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (CONSOLE_CONFIG_RTOS_ZEPHYR == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif
            break;
        case UART_EVENT_RX_CHAR:       // = (1UL << 2), ///< Character received
            //process_rxchar(p_args->data);
            switch ((char) p_args->data) {
                case '\n': h=CONSOLE_LF_EVENT;
#if CONSOLE_CONFIG_COMMAND_MODE
                Console.rx_buffer[Console.rx_idx] = 0;
                Console.cb(h,(void*) Console.rx_buffer);           /* pass the completed input buffer */
                Console.rx_idx = 0;
#else
                         RA_console.cb(h,(void*) &p_args->data);   /* pass just the received character */
#endif
                         break;
                case '\b':
                    if(Console.rx_idx > 0)
                    {
                        Console.rx_idx--;
                    }
                    break;
                default: h=CONSOLE_CHAR_EVENT;
                if (Console.flags & CONSOLE_FLAG_ECHO)
                {

                }
#if CONSOLE_CONFIG_COMMAND_MODE
                Console.rx_buffer[Console.rx_idx] = (char) p_args->data;
                Console.rx_idx = (Console.rx_idx < (Console.rx_size - 1)) ? Console.rx_idx + 1 : Console.rx_idx;
#else
                         RA_console.cb(h,(void*) &p_args->data);
#endif
            }
            break;
        case UART_EVENT_TX_DATA_EMPTY: // = (1UL << 7), ///< Last byte is transmitting, ready for more data
            /* not used in this implementation */
            break;

        default: /* anything else is considered an error.  DWR  Depending on the error bits to be present in the lower 8 bits */
        // = (1UL << 3), ///< Parity error event
        // = (1UL << 4), ///< Mode fault error event
        // = (1UL << 5), ///< FIFO Overflow error event
        // = (1UL << 6), ///< Break detect error event
            Console.flags |= (uint32_t) ((p_args->event & 0x000000FF) << 24) | CONSOLE_FLAG_ERROR; /* set an error flag */

#if (CONSOLE_CONFIG_RTOS_AZURE == BSP_CFG_RTOS) /* Azure */
            tx_semaphore_ceiling_put(Console.tx_done_sema,1); //@@@ this needs to be to local sema
#elif (CONSOLE_CONFIG_RTOS_FREERTOS == BSP_CFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (CONSOLE_CONFIG_RTOS_ZEPHYR == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif
    }
#if (CONSOLE_CONFIG_RTOS_FREERTOS == BSP_CFG_RTOS)
    xResult = xEventGroupClearBitsFromISR or something...
    if( xResult != pdFAIL )
    {
      /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
      switch should be requested.  The macro used is port specific and will
      be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
      the documentation page for the port being used. */
      portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
#endif

}
