/*
 * console.c
 */
#include "application_common.h"
#include "console.h"
#include <stdio.h>
#include <string.h>

#if APP_HAS_CMD_SHELL //@@@ using a command set
#include "commands/commands.h"
#endif

const char* ansi_estr[] =
{
 "\x1b[2;37m", // dim white
 "\x1b[2;31m", // red
 "\x1b[2;32m", // green
 "\x1b[2;33m", // yellow
 "\x1b[2;34m", // blue
 "\x1b[2;35m", // magenta

};
#if APP_HAS_CMD_SHELL
static char cm[COMMAND_NAME_SIZE];
static exec_frame_t ExeFrame;
static int parse_command(char*);
#endif
static char tmb[64];

static console_t *p_console;

/*
 *   API
 */
int console_Open(console_t* con)
{
    if (NULL == con) return -1;
    p_console = con;
    p_console->flags = CONSOLE_START_UP;
#if APP_HAS_CMD_SHELL
        p_console->commands = (command_t**) command_set; /* the command_set is defined in the commands interface */
#else
        p_console->commands = NULL;
#endif
    return 0;
}
int console_Print(char* s)
{
    int len;
    if(NULL == s) return 0;
    len = (int) strlen(s);

    /* Wait for the uart transmitter to be available */
#if   (0 == BSP_CFG_RTOS) /* Bare METAL */
    while(Console.flags & CONSOLE_FLAG_TX_BUSY);
#elif (1 == BSP_CFG_RTOS) /* Azure */
    tx_mutex_get(&g_console_mutex,TX_WAIT_FOREVER);
#elif (2 == BSP_CFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (3 == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif
    Console.flags |= CONSOLE_FLAG_TX_BUSY;
    Console.write(s,len,NULL);
    /* Wait for the uart transmitter to finish */
#if   (0 == BSP_CFG_RTOS) /* Bare METAL */
    while(Console.flags & CONSOLE_FLAG_TX_BUSY);
#elif (1 == BSP_CFG_RTOS) /* Azure */
    tx_semaphore_get(&g_console_sema,TX_WAIT_FOREVER);
    Console.flags &= ~CONSOLE_FLAG_TX_BUSY;
    tx_mutex_put(&g_console_mutex);
#elif (2 == BSP_CFG_RTOS) /* Fee RTOS */
#error needs implementing
#elif (3 == BSP_CFG_RTOS) /* Zephyr */
#error needs implementing
#endif
    Console.flags |= CONSOLE_FLAG_TX_DONE;
    return len;
}
//@@DWR this function must be called with *s being in RAM.  It does not make a copy
int console_Exec(char* s)
{
#if APP_HAS_CMD_SHELL
    switch (parse_command(s)) {
        case -1: return -1;  //@@ ERROR
        case 0: return 0;
    }
    strncpy(&cm[0],ExeFrame.args[0],COMMAND_NAME_SIZE);
    int i,j;
    j=0;
    command_t *pcmd;
    do
    {
        i=0;
        pcmd = p_console->commands[j];
        if (pcmd != NULL) do
        {
            if (strcmp(pcmd[i].name,cm) == 0)
            {
                pcmd[i].cmd(ExeFrame.args);
                console_Print("\n>");
                return i;
            }
            i++;
        } while(pcmd[i].name != NULL);
        j++;
    } while(NULL != p_console->commands[j]);
#if CONSOLE_CONFIG_COMMAND_MODE
            console_Print("\ninvalid command\n>");
#endif
#else
    FSP_PARAMETER_NOT_USED(s);
    console_SetColor(CONSOLE_COLOR_RED);
    console_Print("No commands installed!\n");
    console_SetColor(CONSOLE_COLOR_NORMAL);
#endif
    return -1;
}
int console_UpdatePane(console_pane_t *pane)
{
    console_data_t *d = pane->datalist;
    char *q;
    q = &tmb[16];

    while(d->w != 0)
    {
        sprintf(tmb,"\x1B[%d;%dH",d->r,d->c);
        console_Print(tmb);
        switch (d->data_type) {
            case CONSOLE_DATA_INT:

                sprintf(tmb,"%%%dd",d->w);
                sprintf(q,tmb,*((uint32_t*) (d->data)));
                console_Print(q);
                break;
            case CONSOLE_DATA_FLOAT:
                break;
            case CONSOLE_DATA_STRING:
                break;
        }
        d++;
    }
    return 0;
}
int console_SetColor(console_color_t c)
{
    if (c >= CONSOLE_COLOR_END)
        return -1;
    console_Print((char*) ansi_estr[c]);
    return 0;
}
/*
 *   STATIC functions
 */
volatile int tommy;
#if APP_HAS_CMD_SHELL
static int parse_command(char* s)
{
    char *p,*q;
    int i;
    p = q = s;
    ExeFrame.arg_cnt = 0;
    ExeFrame.flags &= (uint32_t) ~0x00000001;
    for (i=0;i<COMMAND_ARG_CNT;i++)
        ExeFrame.args[i] = 0;
    i=0;
    do {
        if (ExeFrame.arg_cnt == COMMAND_ARG_CNT) return -1;
        tommy = ExeFrame.arg_cnt;
        ExeFrame.args[tommy] = q;
        ExeFrame.arg_cnt++;
        do {
            while( (*p != ' ' ) &&
                    (*p != '\n') &&
                    (*p !=  0  ) &&
                    (*p != '\"'))
            {i++;p++;}
            switch (*p) {
                case 0:
                    if (ExeFrame.flags & 0x00000001)
                    {
                        tommy = tommy + 5;
                        return -1;
                    } else
                    {
                        tommy = tommy + 2;
                        return ExeFrame.arg_cnt;
                    }
                case '\n': *p = 0; return ExeFrame.arg_cnt;
                case ' ':
                    while(*p == ' ') {*p=0;p++;}

                    q = p;
                    break;
                case '\"': // arg separator
                    if (ExeFrame.flags & 0x00000001) // currently quoted.
                    {
                        ExeFrame.flags &= (uint32_t) ~0x00000001;
                    }
                    else
                    {
                        ExeFrame.flags |= 0x00000001;

                    }
                    //while(*p == ' ') {*p=0;p++;}
                    break;

            }
        } while(ExeFrame.flags & 0x00000001);
        tommy--;
    } while(*p);
    tommy++;
    return ExeFrame.arg_cnt;
}
#endif
/*
 *   CALLBACK
 */
int console_CB(char ch) // return: 0 normally, 1 if lf, -1 if error.
{

    switch(ch) {
        case '\n':
            break;
        case '\b':
            break;
        case 0:
            return -1;
    }
#if CONSOLE_CONFIG_COMMAND_MODE
    if (p_console->rx_idx == (p_console->rx_size-1))
    {
        p_console->rx_buffer[p_console->rx_idx] = 0;
        return -1;
    }
#endif
    return 0;
}
