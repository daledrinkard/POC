/*
 * FS_commands.c
 *
 *  File System commands
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
static int FS_dir(char** args);
static int FS_cpy(char** args);
static int FS_del(char** args);
static int FS_cat(char** args);
static int FS_cd(char** args);

const char FS_cmds_name[] = "File Commands";
const command_t AP_cmds[] =
{
      {"dir",&FS_dir,"display directory"},
	  {"cpy",&FS_cpy,"copy src dest"},
      {"del",&FS_del,"delete file"},
	  {"cat",&FS_cat,"display the contents of a file"},
	  {"cd",&FS_cd,"change directory"},
  {0,0,0}
};

static int FS_dir(char** args)
{
	return 0;
}
static int FS_cpy(char** args)
{
	return 0;
}
static int FS_del(char** args)
{
	return 0;
}
static int FS_cat(char** args)
{
	return 0;
}
static int FS_cd(char** args)
{
	return 0;
}
#endif




