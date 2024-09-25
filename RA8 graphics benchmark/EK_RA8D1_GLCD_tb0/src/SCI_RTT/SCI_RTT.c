/*
 * SCI_RTT.c
 *
 *  Created on: May 23, 2023
 *      Author: daled
 */

#include "SCI_RTT.h"
#include "bsp_cfg.h"
#include "hal_data.h"

#define SCI_FLAG_OPENED  (0x00000008)
#define SCI_FLAG_TXDONE  (0x00000001)
#define SCI_FLAG_HASKEY  (0x00000002)
#define SCI_FLAG_ERROR   (0x00000004)
#define SCI_FLAG_TXEMPTY (0x00000010)

static uart_callback_args_t uart_args;
static uart_instance_t *sci;
static volatile uint32_t sci_flags;
static uint8_t sci_rxchar;
static volatile uint8_t sci_rxbuffer[SCI_RXBUFFER_SIZE];
static volatile uint16_t sci_rxidx;
#if (2 == BSP_CFG_RTOS)
static SemaphoreHandle_t sema;
#endif
void SCI_RTT_callback(uart_callback_args_t *p_args);
fsp_err_t SCI_RTT_Open(void *p_sci, void *p_semaphore, void *p_user_callback)
{
    fsp_err_t err;
    sci_flags = 0;
    sci = (uart_instance_t*) p_sci;
    err = sci->p_api->open(sci->p_ctrl,sci->p_cfg);
    if (FSP_SUCCESS != err) return err;
    err = sci->p_api->callbackSet(sci->p_ctrl,SCI_RTT_callback,NULL,&uart_args);
    if (FSP_SUCCESS != err) return err;
    sci_rxidx = 0;
//@@    err = sci->p_api->read(sci->p_ctrl,&sci_rxbuffer[0],SCI_RXBUFFER_SIZE);
//@@    if (FSP_SUCCESS != err) return err;

    sci_flags |= SCI_FLAG_OPENED;
#if (0 == BSP_CFG_RTOS) // Baremetal
#endif
#if (1 == BSP_CFG_RTOS) //Azure
#endif
#if (2 == BSP_CFG_RTOS) // FreeRTOS
    sema = (SemaphoreHandle_t) p_semaphore;
#endif
#if (3 == BSP_CFG_RTOS) // Zyphr
#endif
//err = R_SCI_UART_CallbackSet(&g_SCI_RTT_uart_ctrl, uart_callback_args_t, p_context, p_callback_memory);

    return FSP_SUCCESS;
}

int          SEGGER_RTT_HasKey                  (void)
{
    return (sci_flags & SCI_FLAG_HASKEY) ? 1 : 0;
}
unsigned     SEGGER_RTT_Read  (unsigned BufferIndex,
                               void* pBuffer, unsigned BufferSize)
{
    int i;
    unsigned Size;
    for(i=0;i<(sci_rxidx+1);i++)
    {
        ((uint8_t*) pBuffer)[i] = sci_rxbuffer[i];
        Size = sci_rxidx;
    }
    sci_flags &= (uint32_t) ~SCI_FLAG_HASKEY;
    sci_rxidx = 0;
  return Size;

}
unsigned     SEGGER_RTT_Write (unsigned BufferIndex,
                               const void* pBuffer, unsigned NumBytes)
{
    fsp_err_t err;
    sci_flags &= ~SCI_FLAG_TXDONE;
    err = sci->p_api->write(sci->p_ctrl,pBuffer,NumBytes);
    if (FSP_SUCCESS != err) return 0;
#if   (0 == BSP_CFG_RTOS)
#elif (1 == BSP_CFG_RTOS)
#elif (2 == BSP_CFG_RTOS)
    if (NULL != sema)
    {
        xSemaphoreTake(sema, portMAX_DELAY);
    }
#elif (3 == BSP_CFG_RTOS)
#endif
    while(0 == (sci_flags & SCI_FLAG_TXDONE));
    return NumBytes;
}






// callback
void SCI_RTT_callback(uart_callback_args_t *p_args)
{
    uart_callback_args_t v;
    uart_event_t g;
#if (2 == BSP_CFG_RTOS)
    BaseType_t ctx = false;
#endif
    switch (p_args->event) {
        case UART_EVENT_RX_COMPLETE: //   = (1UL << 0), ///< Receive complete event
            break;
        case UART_EVENT_TX_COMPLETE: //   = (1UL << 1), ///< Transmit complete event
            sci_flags |= SCI_FLAG_TXDONE;
            break;
        case UART_EVENT_RX_CHAR: //       = (1UL << 2), ///< Character received
#if SCI_UART_B
            sci_rxchar = ((sci_b_uart_instance_ctrl_t *) sci->p_ctrl)->p_reg->RDR;
#else
            sci_rxchar = ((sci_uart_ instance_ctrl_t *) sci->p_ctrl)->p_reg->RDR;
#endif
#if (SCI_ECHO_RX)
#if SCI_UART_B
            ((sci_b_uart_instance_ctrl_t *) sci->p_ctrl)->p_reg->TDR = sci_rxchar;
#else
            ((sci_uart_instance_ctrl_t *) sci->p_ctrl)->p_reg->TDR = sci_rxchar;
#endif
#endif
            switch (sci_rxchar)
            {
                case '\r' : // carriage return
                    sci_rxbuffer[sci_rxidx] = 0;
                    sci_flags |= SCI_FLAG_HASKEY;
                    break;
                case 27 : // escape
                    sci_rxbuffer[sci_rxidx] = 27;
                    sci_flags |= SCI_FLAG_HASKEY;
                    break;
                default:
                    sci_rxbuffer[sci_rxidx] = sci_rxchar;
                    sci_rxidx = (sci_rxidx < SCI_RXBUFFER_SIZE) ? sci_rxidx + 1 : sci_rxidx;
            }
            break;
        case UART_EVENT_ERR_PARITY: //    = (1UL << 3), ///< Parity error event
            sci_flags |= SCI_FLAG_ERROR;
            break;
        case UART_EVENT_ERR_FRAMING: //   = (1UL << 4), ///< Mode fault error event
            sci_flags |= SCI_FLAG_ERROR;
            break;
        case UART_EVENT_ERR_OVERFLOW: //  = (1UL << 5), ///< FIFO Overflow error event
            sci_flags |= SCI_FLAG_ERROR;
            break;
        case UART_EVENT_BREAK_DETECT: //  = (1UL << 6), ///< Break detect error event
            sci_flags |= SCI_FLAG_ERROR;
            break;
        case UART_EVENT_TX_DATA_EMPTY: // = (1UL << 7), ///< Last byte is transmitting, ready for more data
            sci_flags |= SCI_FLAG_TXEMPTY;
            break;
        default: while(1); //@@TRAP
    }
#if (2 == BSP_CFG_RTOS)
    if (NULL != sema)
    {
        xSemaphoreGiveFromISR(sema, &ctx);
    }
    portYIELD_FROM_ISR(ctx); // @suppress("5.3a Multiple single statements") @suppress("3.6a Cast comment")
#endif
}



