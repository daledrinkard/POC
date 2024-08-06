#include "hal_data.h"
//#include "gui_thread.h"
#include <stdio.h>
#include <sys/stat.h>
#include "printf_redirect.h"

#define SERIAL_TXDONE (0x00000001)
#define SERIAL_RXCHAR (0x00000002)

int _write(int file, char *ptr, int len);
int _close(int file);
int _fstat(int file, struct stat *st);
int _isatty(int file);
int _read(int file, char *ptr, int len);
int _lseek(int file, int ptr, int dir);

static char command_line_buffer[PRINTF_RX_BUFFER_LENGTH] = {0};
static int  command_line_windex = 0;
static int  command_line_rindex = 0;
static int  command_line_length = 0;
static volatile unsigned int flags;

static void process_rxchar(uint32_t);
static void process_rxchar(uint32_t ch)
{
    switch(ch) {
        case '\r':
            command_line_buffer[command_line_windex] = 0;
            command_line_length = command_line_windex;
            command_line_windex = 0;
            command_line_rindex = 0;
            break;
        default:
            command_line_buffer[command_line_windex] = (char) ch;
            // chopped buffer.
            command_line_windex = ((int)command_line_windex < (int)((sizeof(command_line_buffer) - 1)))
                    ? command_line_windex+1 : command_line_windex;
               }
}




int _write(int file, char *ptr, int len)
{
    fsp_err_t err = FSP_SUCCESS;
    FSP_PARAMETER_NOT_USED(file);

    static unsigned char uart_open = 0;

    if (0 == uart_open)
    {
#if PRINTF_USE_SCI_B
        err = R_SCI_B_UART_Open(&g_printf_uart_ctrl, &g_printf_uart_cfg);
#else
        err = R_SCI_UART_Open(&g_printf_uart_ctrl, &g_printf_uart_cfg);

#endif
        if (FSP_SUCCESS == err)
        {
            uart_open = true;
        }
        else
        {
            __BKPT(0); //todo
        }

    }
    flags &= (unsigned int)~SERIAL_TXDONE;
#if PRINTF_USE_SCI_B
    err = R_SCI_B_UART_Write(&g_printf_uart_ctrl, (uint8_t *)ptr, (uint32_t)len);
#else
    err = R_SCI_UART_Write(&g_printf_uart_ctrl, (uint8_t *)ptr, (uint32_t)len);
#endif
    if (FSP_SUCCESS == err)
    {
        while(0x00 == (flags & SERIAL_TXDONE));
    }
    else
    {
        return -1;
    }
    return len;
}

int _close(int file)
{
  FSP_PARAMETER_NOT_USED(file);
  return -1;
}
int _fstat(int file, struct stat *st)
{
    FSP_PARAMETER_NOT_USED(file);
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
    FSP_PARAMETER_NOT_USED(file);
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
    FSP_PARAMETER_NOT_USED(file);
    FSP_PARAMETER_NOT_USED(ptr);
    FSP_PARAMETER_NOT_USED(dir);
  return 0;
}

int _read(int file, char *ptr, int len)
{
    int ret_len;
    FSP_PARAMETER_NOT_USED(file);
//    FSP_PARAMETER_NOT_USED(ptr);
//    FSP_PARAMETER_NOT_USED(len);
    if (command_line_length > 0)
    {
        ret_len = (len < command_line_length) ? len : command_line_length;
        memcpy(ptr,&command_line_buffer[command_line_rindex],(size_t) ret_len);
        command_line_rindex = command_line_rindex + ret_len;
        command_line_length = command_line_length - ret_len;
        return ret_len;
    }
    return -1;
}


/*
 *     CALLBACKS
 */
void printf_uart_callback(uart_callback_args_t *p_args)
{

    switch(p_args->event) {
        case UART_EVENT_RX_COMPLETE:   // = (1UL << 0), ///< Receive complete event
            break;
        case UART_EVENT_TX_COMPLETE:   // = (1UL << 1), ///< Transmit complete event
            flags |= SERIAL_TXDONE;
            break;
        case UART_EVENT_RX_CHAR:       // = (1UL << 2), ///< Character received
            process_rxchar(p_args->data);
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
