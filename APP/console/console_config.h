/*
 * console_config.h
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */

#ifndef CONSOLE_CONSOLE_CONFIG_H_
#define CONSOLE_CONSOLE_CONFIG_H_

#define CONSOLE_RX_BUFFER_LEN (256)
#define CONSOLE_NAME_SIZE     (8)

#define COMMAND_NAME_SIZE     (7+1)  // room for the terminator character.
#define COMMAND_ARG_CNT       (16)   // max number of arguments
#define COMMAND_TEMP_BUF_SIZE (256)  // DWR..  Used in lots of places.

#define CONSOLE_START_UP (CONSOLE_FLAG_ECHO)

#define CONSOLE_CONFIG_COMMAND_MODE (1)


/*
#define CONSOLE_CONFIG_RTOS_NONE     (0)
#define CONSOLE_CONFIG_RTOS_AZURE    (1)
#define CONSOLE_CONFIG_RTOS_FREERTOS (2)
#define CONSOLE_CONFIG_RTOS_ZEPHYR   (3)
#define CONSOLE_CONFIG_RTOS          ( BSP_CFG_RTOS)
*/

#endif /* CONSOLE_CONSOLE_CONFIG_H_ */
