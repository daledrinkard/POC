/*
 *     DBG_PRINT.H
 *
 * Human-readable ANSI-colored dump of the PowerController object
 * (power_module.h) to the debug console, for use over an ASCII terminal.
 *
 */

#ifndef DBG_PRINT_H_
#define DBG_PRINT_H_
#include "application_common.h"
/* USER INCLUDE */

/* PUBLISHED functions */
void print_controller(int num_rails);
/* x is one of the pmbus_command_t codes (PMbus_commands.h); prints "0x%02X  <NAME> (c)",
 * followed by " XX XX ..." hex bytes from data[0..len-1] if data is non-NULL */
void print_pmbus_cmd(uint8_t x, char c, const uint8_t *data, uint8_t len);

/* USER ADDITIONAL PUBLIC FUNCTIONS */

#endif /* DBG_PRINT_H_ */
