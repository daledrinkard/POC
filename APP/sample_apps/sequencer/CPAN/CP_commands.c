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
#include "CPAN/cpan.h"
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

static void exCP_stat_identify(void);
volatile uint32_t peter1;
static int CP_stat(char** args)  /* stat s value | c value | r */
{
    uint32_t v;
    uint32_t flags = ControlPanel.stat;
    if (args[1] == NULL) /*read as a hex value*/
    {
        console_Print("\nstat flags:");
        sprintf(command_buffer,"0x%08X\n",flags);
        console_Print(command_buffer);
    }
    else
    {
        if (NULL == args[2]) /* these options have no argument */
        {
            switch (args[1][0]) {
            case 'i':
            case 'I': exCP_stat_identify(); return 0;
            }
            return -1;
        }
        v = (uint32_t) strtoul(args[2],NULL,16);
        peter1 = v;
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
    bool found;
    uint32_t flags = ControlPanel.stat;
    for(int i=0;i<32;i++) /* look at every bit */
    {
        found = false;
        if (flags & (1 << i))
        {
            console_Print("\n  ");
            int j=0;
            do
            {
                if (CPAN_stat_str[j].val & (1 << i))
                {
                    console_Print((char*) CPAN_stat_str[j].str);
                    found = true;
                }
                else
                {
                    j++;
                }
            } while ( !found && (CPAN_stat_str[j].val > 0));
            if (!found)
            {
                sprintf(command_buffer,"(0x%08X)", (1 << i));
                console_Print(command_buffer);
            }
        }
    }

}
static int CP_creg(char** args)
{
    uint32_t V;
    if (NULL == args[1]) return -1;
    uint32_t R = (uint32_t) strtoul(args[1],NULL,10);
    if (R > CPAN_NUM_OF_REGS) return -1;
    if (NULL == args[2])
    {
        sprintf(command_buffer,"%08X\n",ControlPanel.regs[R]);
        console_Print(command_buffer);
        return 0;
    }
    if ((args[2][1] == 'x') || (args[2][1] == 'X'))
      V = (uint32_t) strtoul(args[2],NULL,16);
    else
      V = strtoul(args[2],NULL,10);
    ControlPanel.regs[R] = V;
    return 0;
}
static int CP_rst(char** args)
{
    FSP_PARAMETER_NOT_USED(args);
    NVIC_SystemReset();
    return 0;
}
#endif // APP_HAS_CMD_SHELL

