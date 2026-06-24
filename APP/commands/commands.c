/*
 * commands.c
 *
 *  Created on: Aug 6, 2025
 *      Author: daled
 */


#include "hal_data.h"
#include"commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../CPAN/cpan.h"

extern command_t* P1_cmds;
extern command_t* CP_cmds;
extern command_t* CAM_cmds;
extern command_t* AP_cmds;

/* ----------------------------------------------------------

      Basic commands

 ---------------------------------------------------------- */
static int BS_cls(char** args);  /* clear an ANSI terminal screen esc[2J */
static int BS_echo(char** args);
static int BS_help(char** args); /* display help for all commands */


const command_t BS_cmds[] =
{
  {"echo",&BS_echo,"echo argument"},
  {"cls", &BS_cls, "clear screen"},

  {"help",&BS_help,"get help"},
  {0,0,0}
};

console_t *p_Console;
char command_buffer[COMMAND_TEMP_BUF_SIZE]; /* */
/* command_set is referenced in the console open function */
const command_t* command_set[] = {(command_t*) &BS_cmds,
                                  (command_t*) &P1_cmds,
                                  (command_t*) &CP_cmds,
                                  (command_t*) &CAM_cmds,
                                  (command_t*) &AP_cmds,
								  NULL};
static int BS_cls(char** args)
{
    FSP_PARAMETER_NOT_USED(args);
    console_Print("\x1b[2J\x1b[H");
    return 0;
}
static int BS_echo(char** args)
{
    console_Print(args[1]);
    console_Print("\n");
    return 0;
}
static int BS_help(char** args)
{
    FSP_PARAMETER_NOT_USED(args);
    int i=0;
	command_t* p;
	command_t* q;
    p = command_set[i];
    while(NULL != p)
    {
        q = p;
        while(NULL != q->name)
        {
            sprintf(command_buffer,"%s: %s\n",q->name, q->help);
            console_Print(command_buffer);
            q++;
        }
        i++;
        p = command_set[i];
    }
    //@@@ needs implementing again
    return 0;
}





