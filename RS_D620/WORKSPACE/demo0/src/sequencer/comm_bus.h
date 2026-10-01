/*
 *     COMM_BUS.H
 *
 * Communication bus module header file
 *
 */

#ifndef COMM_BUS_H_
#define COMM_BUS_H_
#include "application_common.h"  // IWYU 

#define COMM_BUS_CFG_ADDR (0x3C) //@@@ needs to be configurable?????

#define COMM_SOP ('S')
#define COMM_EOP ('P')

#define COMM_DATA_SIZE 256

#define COMM_FLAG_TX_RDY  (0x01)
#define COMM_FLAG_TX_DONE (0x02)

typedef enum 
{
    COMM_STATE_IDLE,
    COMM_STATE_START,
    COMM_STATE_ADDRESS,
    COMM_STATE_COMMAND,
#if 0 //@@@ need #define for mfg_cmd    
//    COMM_STATE_MFG_CMD,  /* not supported in TI part */
#endif
    COMM_STATE_DATA,
    COMM_STATE_FAULT,
    COMM_STATE_READ
} comm_state_type_t;

/* USER INCLUDE */
typedef struct PB_comm_s
{
    comm_state_type_t state;
    uint8_t address;
    uint8_t command;
#if 0 //@@@ support of mfg_cmd needs #define    
//    uint8_t mfg_cmd;    /* not supported in TI device */
#endif
    uint8_t data_len;
    uint8_t read_write;
    uint8_t flag;
    uint8_t data[COMM_DATA_SIZE];
} PB_comm_t;

/* USER ADDITIONAL PUBLIC FUNCTIONS */
int comm_init(void);
int comm_service(void);
#endif /* COMM_BUS_H_ */
