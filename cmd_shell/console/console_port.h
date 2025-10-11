/*
 * console_port.h
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */

#ifndef CONSOLE_CONSOLE_PORT_H_
#define CONSOLE_CONSOLE_PORT_H_


#include "hal_data.h"
#include "console.h"


console_t* RA_console_init(char *name, const command_t *cmd,const uart_instance_t *uart,void* cb);

#endif /* CONSOLE_CONSOLE_PORT_H_ */
