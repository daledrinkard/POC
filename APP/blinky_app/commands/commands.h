/*
 * commands.h
 *
 *  Created on: Aug 6, 2025
 *      Author: daled
 */

#ifndef COMMANDS_COMMANDS_H_
#define COMMANDS_COMMANDS_H_

#include "../console/console.h"
#include "../console/console_port.h"


extern const command_t* command_set[];
#define COMMANDS_NUMBER_OF_CMD_SETS (sizeof(command_set/4))

#endif /* COMMANDS_COMMANDS_H_ */
