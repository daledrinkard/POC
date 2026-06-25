/*
 * CP_commands.c
 *
 *  Commands for Control Panel
 * 
 */
#include "application_common.h"
#if APP_HAS_CMD_SHELL
#include "hal_data.h" //@@@ good for RTOS??
#include"commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpan.h"
extern char command_buffer[];
extern volatile cpan_t ControlPanel;
static int CP_stat(char** args);
static int CP_creg(char** args);
static int CP_rst(char** args);
const command_t CP_cmds[] =
{
  {"stat",&CP_stat,"status flag"},
  {"rst", &CP_rst,"restart app"},
  {"creg",&CP_creg,"creg reg value"},
  {0,0,0}
};
const char* CPAN_STAT_ansi[] = {
  "UPDATE ",
  "RESTART ",
  NULL
  };
static void exCP_stat_identify(void);
static int CP_stat(char** args)  /* stat s value | c value | r */
{
    uint32_t v;
    uint32_t flags = ControlPanel.stat;
    if (args[1] == NULL) /*read*/
    {
        console_Print("\nstat flags:");
        int i=0;
        do
        {
            if ((1<<i) & flags)
            {
                console_Print("\n  ");
                console_Print((char*) CPAN_STAT_ansi[i]);
            }
            i++;
        } while (CPAN_STAT_ansi[i] != NULL);
    }
    else /* write*/
    {
        if (NULL == args[2]) /* these options have no argument */
        {
            switch (args[1][0]) {
            case 'i':
            case 'I': exCP_stat_identify(); return 0;
            }
            return -1;
        }
        v = (uint32_t) strtol(args[2],NULL,16);
        switch (args[1][0]) {
        case 's':
        case 'S': ControlPanel.stat |= v; break;
        case 'c':
        case 'C': ControlPanel.stat &= ~v; break;
        }
    }
    return 0;
}
static void exCP_stat_identify(void)
{
    int i=0;
    do
    {
        sprintf(command_buffer,"(%08X) %s\n",(1 << i),CPAN_STAT_ansi[i]);
        console_Print(command_buffer);
        i++;
    } while (CPAN_STAT_ansi[i] != NULL);
}
static int CP_creg(char** args)
{
    uint32_t V;
    if (NULL == args[1]) return -1;
    uint32_t R = strtol(args[1],NULL,10);
    if (R > CPAN_NUM_OF_REGS) return -1;
    if (NULL == args[2])
    {
        sprintf(command_buffer,"%08X\n",ControlPanel.regs[R]);
        console_Print(command_buffer);
        return 0;
    }
    if ((args[2][1] == 'x') || (args[2][1] == 'X'))
      V = strtol(args[2],NULL,16);
    else
      V = strtol(args[2],NULL,10);
    ControlPanel.regs[R] = V;
    return 0;
}
static int CP_rst(char** args)
{
    FSP_PARAMETER_NOT_USED(args);
    ControlPanel.stat |= CPAN_STAT_RESTART_APP;
    return 0;
}
#endif // APP_HAS_CMD_SHELL 

