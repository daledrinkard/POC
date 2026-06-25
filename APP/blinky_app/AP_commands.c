/*
 * AP_commands.c
 *
 *  Commands specific to the application
 *
 *
 */

#include "application_common.h"
#if APP_HAS_CMD_SHELL
#include "hal_data.h" //@@@ will this work for RTOS???
#include"commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern char command_buffer[];
//!!extern volatile cpan_t ControlPanel;
static int AP_app(char** args);
const char AP_cmds_name[] = "AP Commands";
const command_t AP_cmds[] =
{
  {"app",&AP_app,"manipulate app object"},
  {0,0,0}
};
static int AP_app(char** args)
{
    FSP_PARAMETER_NOT_USED(args);
	console_Print("Hello World\n");
    return 0;
}
#endif




