/*
 *     COMM_BUS.H
 *
 * Communication bus module header file
 *
 */

#ifndef COMM_BUS_H_
#define COMM_BUS_H_
#include "application_common.h"

#define COMM_BUS_CFG_ADDR (0x3C) //@@@ needs to be configurable?????

#define COMM_SOP ('S')
#define COMM_EOP ('P')

#define COMM_DATA_SIZE 256
typedef enum 
{
    COMM_STATE_IDLE,
    COMM_STATE_START,
    COMM_STATE_ADDRESS,
    COMM_STATE_COMMAND,
//    COMM_STATE_MFG_CMD,  /* not supported in TI part */
    COMM_STATE_DATA,
    COMM_STATE_FAULT
} comm_state_type_t;

/* USER INCLUDE */
typedef struct PB_comm_s
{
    comm_state_type_t state;
    uint8_t address;
    uint8_t command;
//    uint8_t mfg_cmd;    /* not supported in TI device */
    uint8_t data_len;
    uint8_t read_write;
    uint8_t data[COMM_DATA_SIZE];
} PB_comm_t;

/* USER ADDITIONAL PUBLIC FUNCTIONS */

#endif /* COMM_BUS_H_ */
