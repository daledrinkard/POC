/*
            COMM_BUS
*/
#include "comm_bus.h"
#include "POP/pop.h"
#include "console/console.h" //IWYU pragma: keep (provides console_t for downstream includers)
#include "PMbus_commands.h"
/* USER */


PB_comm_t PB_Comm;

int comm_init(void)
{
    return 0;
}

int comm_service(void)
{
    switch (PB_Comm.state)
    {
        case COMM_STATE_IDLE:
            break;
        case COMM_STATE_START:
            break;
        case COMM_STATE_COMMAND:
            break;
        case COMM_STATE_FAULT:
            break;
    }
    return 0;
}
//extern uint8_t *PMbus_write_execute(uint8_t command, uint8_t *data, uint16_t len);
static uint32_t comm_parse_command(void);
static uint32_t comm_parse_command(void)
{
    if (PB_Comm.read_write == 'W')
    {
      PMbus_write_execute(PB_Comm.command,PB_Comm.data,PB_Comm.data_len);
    }
    //@@@ what do you do to read?????
}

/**
 *    Callback functions
 */
void T2_cb(timer_callback_args_t *p_args)
{
    /* USER CODE: handle timer callback */
    
}
/**
 *   Callback functions
 *   The ingress data stream is ASCII encoded, with the following format:
 *  <SOP><RW><ADDR><CMD><DATA><EOP>
 *  whitespace characters are ignored, and the data is hex encoded.
 *  where:
 *  SOP = Start of Packet, 1 byte, 'S'
 *  RW = Read/Write, 1 byte, 'R' or 'W'
 *  ADDR = Address, 2 bytes, hex encoded
 *  CMD = Command, 2 bytes, hex encoded
 *  DATA = Data, variable length, hex encoded
 *  EOP = End of Packet, 1 byte, 'P'
 *   an example would be: SW3C010203P
 */
void comm_cb(uart_callback_args_t *p_args)
{
    uint8_t data = (uint8_t) p_args->data;
    volatile static uint8_t bx,cx; //@@@ volatile just for debugging
    switch(p_args->event) {
        case UART_EVENT_RX_COMPLETE:   // = (1UL << 0), ///< Receive complete event
            break;
        case UART_EVENT_TX_COMPLETE:   // = (1UL << 1), ///< Transmit complete event
            break;
        case UART_EVENT_RX_CHAR:       // = (1UL << 2), ///< Character received
            if ((data == ' ') || (data == '\n') || (data == '\r')) {
                /* ignore whitespace characters */
                break;
            }
            switch (PB_Comm.state) {
                case COMM_STATE_IDLE:
                    if (data == COMM_SOP) {
                        PB_Comm.address = 0;
                        PB_Comm.state = COMM_STATE_START;
                    } else {
                        POPR();
                        PB_Comm.state = COMM_STATE_FAULT;
                    }
                    break;
                case COMM_STATE_START:
                    PB_Comm.read_write = data;
                    PB_Comm.state = COMM_STATE_ADDRESS;
                    bx = 1;
                    break;
                case COMM_STATE_ADDRESS:
                    PB_Comm.address |= (data > '9') ? (data - 'A' + 10) : (data - '0');
                    PB_Comm.address = PB_Comm.address << (4*bx);
                    if (bx == 0)
                    {
                        bx = 1;
                        PB_Comm.command = 0;
                        PB_Comm.state = COMM_STATE_COMMAND;
                    }
                    else
                    {
                        bx--;
                    }
                    break;
                case COMM_STATE_COMMAND:
                    PB_Comm.command |= (data > '9') ? (data - 'A' + 10) : (data - '0');
                    PB_Comm.command = PB_Comm.command << (4*bx);
                    if (bx == 0)
                    {
                        bx = 1;
                        cx = 0;
//                        PB_Comm.state = (PB_Comm.command == 0xFE) ? COMM_STATE_MFG_CMD : COMM_STATE_DATA ; /* not supported in TI part
                        PB_Comm.state = COMM_STATE_DATA ;
                    }
                    else
                    {
                        bx--;
                    }
                    break;
//                case COMM_STATE_MFG_CMD:
//                    PB_Comm.mfg_cmd |= (data > '9') ? (data - 'A' + 10) : (data - '0');
//                    PB_Comm.mfg_cmd = PB_Comm.command << (4*bx);
//                    if (bx == 0)
//                    {
//                        bx = 1;
//                        cx = 0;
//                        PB_Comm.state = COMM_STATE_DATA;
//                    }
//                    else
//                    {
//                        bx--;
//                    }
//                    break;
                case COMM_STATE_DATA:
                    switch(data) 
                    {
                        case COMM_EOP:
                            PB_Comm.data_len = cx;
                            comm_parse_command();
                            PB_Comm.state = COMM_STATE_IDLE;
                            break;
                        default:
                        if (bx == 1)
                        {
                            PB_Comm.data[cx] = (uint8_t) ((data > '9') ? (data - 'A' + 10) : (data - '0')) << 4;
                            bx--;
                        }
                        else
                        {
                            PB_Comm.data[cx] |= (data > '9') ? (data - 'A' + 10) : (data - '0');
                            bx = 1;
                            cx++;
                        }
                    }
                        break;
                    /* USER CODE: handle data */
                    break;
                case COMM_STATE_FAULT:
                    /* USER CODE: handle fault */
                    break;
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
 //@@@           Console.flags |= (uint32_t) ((p_args->event & 0x000000FF) << 24) | CONSOLE_FLAG_ERROR; /* set an error flag */

#if (APPCFG_RTOS_AZURE == BSP_CFG_RTOS) /* Azure */
            tx_semaphore_ceiling_put(Console.tx_done_sema,1); //@@@ this needs to be to local sema
#elif (APPCFG_RTOS_FREERTOS == BSP_CFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (APPCFG_RTOS_ZEPHYR == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif
    }
#if (APPCFG_RTOS_FREERTOS == BSP_CFG_RTOS)
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
