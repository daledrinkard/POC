/*
 * console_port.c
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */

#include "hal_data.h"
#include "console_port.h"

/*
 *   STATIC declarations
 */
static console_t RA_console;
static uart_instance_t *Uart;
static uart_callback_args_t RA_CB_args;
static void RA_console_cb(uart_callback_args_t*);

static int RA_uart_write(char* data,int len, void *ctx);
static int RA_uart_read(char* data,int len, void *ctx);
/*
 *    Port functions for RA
 */

console_t* RA_console_init(char *name, const command_t *cmd,const uart_instance_t *uart,void* cb)
{
    fsp_err_t err;
    err = uart->p_api->open(uart->p_ctrl,uart->p_cfg);
    err |= uart->p_api->callbackSet(uart->p_ctrl,&RA_console_cb,NULL,&RA_CB_args);
    if (FSP_SUCCESS == err)
    {
        strncpy(&RA_console.name[0],name,CONSOLE_NAME_SIZE);
        Uart = uart;
        RA_console.write = &RA_uart_write;
        RA_console.read = &RA_uart_read;
        RA_console.commands = (command_t *) cmd;
        RA_console.cb = cb;
        console_Open(&RA_console);
    }
    else return NULL;

    return &RA_console;
}


/*
 *     STATIC function definitions
 */
static int RA_uart_write(char* data,int len, void *ctx)
{
    RA_console.flags &= ~CONSOLE_FLAG_TX_DONE;
    RA_console.flags |= CONSOLE_FLAG_TX_BUSY;
    Uart->p_api->write(Uart->p_ctrl,data,len);
    if (ctx == NULL)
    {
        while(RA_console.flags & CONSOLE_FLAG_TX_BUSY);
    }
    return len;
}
static int RA_uart_read(char* data,int len, void *ctx)
{

    return 0;
}

/*
 *   CALLBACK functions
 */

void RA_console_cb(uart_callback_args_t *p_args)
{

    switch(p_args->event) {
        case UART_EVENT_RX_COMPLETE:   // = (1UL << 0), ///< Receive complete event
            break;
        case UART_EVENT_TX_COMPLETE:   // = (1UL << 1), ///< Transmit complete event
            RA_console.flags &= ~CONSOLE_FLAG_TX_BUSY;
            break;
        case UART_EVENT_RX_CHAR:       // = (1UL << 2), ///< Character received
            //process_rxchar(p_args->data);
            break;
        case UART_EVENT_ERR_PARITY:    // = (1UL << 3), ///< Parity error event
            break;
        case UART_EVENT_ERR_FRAMING:   // = (1UL << 4), ///< Mode fault error event
            break;
        case UART_EVENT_ERR_OVERFLOW:  // = (1UL << 5), ///< FIFO Overflow error event
            break;
        case UART_EVENT_BREAK_DETECT:  // = (1UL << 6), ///< Break detect error event
            break;
        case UART_EVENT_TX_DATA_EMPTY: // = (1UL << 7), ///< Last byte is transmitting, ready for more data
            break;

    }
}
