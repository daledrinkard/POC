/*
 * console.c
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */
#include "console.h"
#include <stdio.h>
#include <string.h>


static char cm[COMMAND_NAME_SIZE];
static char tmb[64];
static exec_frame_t ExeFrame;
static int parse_command(char*);

static console_t *p_console;

/*
 *   API
 */
int console_Open(console_t* con)
{
    if (NULL == con) return -1;
    p_console = con;
    p_console->flags = CONSOLE_START_UP;
    return 0;
}
int console_Print(char* s)
{
    int len;
    len = (int) strlen(s);
    p_console->write(s,len,NULL);
    return len;
}
//@@DWR
int console_Exec(char* s)
{
    int i;
    switch (parse_command(s)) {
        case -1: return -1;  //@@ ERROR
        case 0: return 0;
    }
    strncpy(&cm[0],ExeFrame.args[0],COMMAND_NAME_SIZE);
    i=0;
    do
    {
        if (strcmp(p_console->commands[i].name,cm) == 0)
        {
            p_console->commands[i].cmd(ExeFrame.args);
            return i;
        }
        i++;
    } while(p_console->commands[i].name != NULL);
    return -1;
}
int console_UpdatePane(console_pane_t *pane)
{
    console_pane_t *p = pane;
    console_data_t *d = pane->datalist;
    char *q;
    int i;
    uint32_t xi;
    i=0;
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
        //sprintf(sprintf_buffer,"")
        d++;
    }
    return 0;
}
/*
 *   STATIC functions
 */
volatile int tommy;
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

/*
 *   CALLBACK
 */
int console_CB(char ch) // return: 0 normally, 1 if lf, -1 if error.
{
    int i;
    switch(ch) {
        case '\n':
            break;
        case '\b':
            break;
        case 0:
            return -1;
    }
    if (p_console->rx_idx == (p_console->rx_size-1))
    {
        p_console->rx_buffer[p_console->rx_idx] = 0;
        return -1;
    }
    return 0;
}
