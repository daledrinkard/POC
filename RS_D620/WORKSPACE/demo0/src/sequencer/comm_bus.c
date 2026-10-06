/*
            COMM_BUS
*/
#include "comm_bus.h"
#include "POP/pop.h"
//#include "console/console.h" //IWYU pragma: keep (provides console_t for downstream includers)
#include "PMbus_commands.h"
#include "application_common.h"
/* USER */


PB_comm_t PB_Comm;
uint8_t xbuffer[COMM_DATA_SIZE*2]; //@@@ for storing the ASCII hex that is echoed back to the host
int comm_init(void)
{
    R_SCI_UART_Open(&g_comm_uart_ctrl, &g_comm_uart_cfg);
    return 0;
}

/* this function is "buzzed" in the .1mS main loop function when the comm mode is read */
int comm_service(void)
{
    uint8_t t;
    switch (PB_Comm.state)
    {
        case COMM_STATE_IDLE:
            break;
        case COMM_STATE_START:
            break;
        case COMM_STATE_COMMAND:
            break;
        case COMM_STATE_READ:
            if (PB_Comm.flag & COMM_FLAG_TX_RDY)
            {
                /*  Kick off the transmit operation and we'll spin until the completion is detected */
                t = ((PB_Comm.data_len & 0xF0) >> 4);
                xbuffer[0] = ( t > 9) ? (t-10)+'A' : t+'0';
                t = (PB_Comm.data_len & 0x0F);
                xbuffer[1] = ( t > 9) ? (t-10)+'A' : t+'0';
                for(int i=0;i<PB_Comm.data_len;i++)
                {
                    t = ((PB_Comm.data[i] & 0xF0) >> 4);
                    xbuffer[i*2+2] = ( t > 9) ? (t-10)+'A' : t+'0';
                    t = (PB_Comm.data[i] & 0x0F);
                    xbuffer[i*2+3] = ( t > 9) ? (t-10)+'A' : t+'0';
                }
                R_SCI_UART_Write(&g_comm_uart_ctrl,xbuffer,(PB_Comm.data_len*2)+2);
                PB_Comm.flag &= (uint8_t) ~COMM_FLAG_TX_RDY;
            }
            if (PB_Comm.flag & COMM_FLAG_TX_DONE)
            {
                PB_Comm.flag &= ~COMM_FLAG_TX_DONE;
                return 0; /* 0 = we are finished */
            }
            break;
        case COMM_STATE_FAULT:
            break;
            default: break;
    }
    return 1; /* 1 = we are busy */
}
//extern uint8_t *PMbus_write_execute(uint8_t command, uint8_t *data, uint16_t len);
static comm_state_type_t comm_parse_command(void);
static comm_state_type_t comm_parse_command(void)
{
    uint8_t len;
    len = (uint8_t) PMbus_execute(PB_Comm.command,PB_Comm.data,PB_Comm.data_len,(PB_Comm.read_write == 'W') ? 1 : 0);
    switch(PB_Comm.read_write) {
        case 'W': return COMM_STATE_IDLE;
        case 'R':
            PB_Comm.data_len = len;
            PB_Comm.state = COMM_STATE_READ;
            PB_Comm.flag |= COMM_FLAG_TX_RDY;
            app_event_flag_seti(SYSFLG_PWR_READBACK,0); /* this signals the main loop to service the read (transmitting data for us)*/
            return COMM_STATE_READ;
    }
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
            PB_Comm.flag |= COMM_FLAG_TX_DONE;
            PB_Comm.state = COMM_STATE_IDLE;
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
#if 0 //@@@ support for an extended MFG command.                       
//                        PB_Comm.state = (PB_Comm.command == 0xFE) ? COMM_STATE_MFG_CMD : COMM_STATE_DATA ; /* not supported in TI part
#endif
                        PB_Comm.state = COMM_STATE_DATA ;
                    }
                    else
                    {
                        bx--;
                    }
                    break;
#if 0 //@@@ need a #define for whether or not to support MFG CMD extended command function                    
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
#endif
                case COMM_STATE_DATA:
                    switch(data) 
                    {
                        case COMM_EOP:
                            PB_Comm.data_len = cx;
                            PB_Comm.state = comm_parse_command();
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
                case COMM_STATE_READ:
                    while(1); //@@@TRAP   This should never happen as interrupts for "reads" involves TX interrupts
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
