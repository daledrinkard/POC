/*
 * console_port.h
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */

#ifndef CONSOLE_CONSOLE_PORT_H_
#define CONSOLE_CONSOLE_PORT_H_




#include "console.h"

#if   (0 == BSP_CFG_RTOS) /* Bare METAL */
    #include "hal_data.h"
#elif (1 == BSP_CFG_RTOS) /* Azure */
#include <app_thread.h>
#elif (2 == BSP_CFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (3 == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif
   /* USER FIELDS */

#define CONSOLE_TX_WAIT (10000)

console_t* RA_console_init(char *name,const uart_instance_t *uart,void* cb,void *sema);

#endif /* CONSOLE_CONSOLE_PORT_H_ */
