/*
 * P1_commands.c
 *
 *  Created on: Jun 1, 2026
 *      Author: daled
 */
/*

      Basic processor command set for "pokes" and "peeks"


 */


#include "hal_data.h"
#include"commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern char command_buffer[];

static int P1_pin(char** args);
static int P1_r32(char** args);
static int P1_w32(char** args);
static int P1_r16(char** args);
static int P1_w16(char** args);
static int P1_r8(char** args);
static int P1_w8(char** args);

const char P1_cmds_name[] = "P1 Commands";
const command_t P1_cmds[] =
{
  {"pin", &P1_pin,"read or set port,pin"},
  {"r32", &P1_r32,"read add [cnt] 32bit"},
  {"w32", &P1_w32,"write add val 32bit"},
  {"r16", &P1_r16,"read add [cnt] 16bit"},
  {"w16", &P1_w16,"write add val 16bit"},
  {"r8",  &P1_r8,"read add [cnt] 32bit"},
  {"w8",  &P1_w8,"write add val 8bit"},
  {0,0,0}
};


static int P1_pin(char** args)
{
    int port = atoi(args[1] );
    int pin = atoi(args[2]);
    if (NULL == args[3])
    {
        sprintf(command_buffer,"%08X\n",(unsigned int) R_PFS->PORT[port].PIN[pin].PmnPFS);
        console_Print(command_buffer);
    }
    else
    {
        uint32_t val = (uint32_t) strtol(args[3],NULL,16);
        R_PFS->PORT[port].PIN[pin].PmnPFS = val;
    }
    return 0;
}

static int P1_r32(char** args)
{
    uint32_t *p = (uint32_t*) strtol(args[1],NULL,16);
    uint32_t count = (uint32_t) strtol(args[2],NULL,10);
    while(count)
    {
        sprintf(command_buffer,"%08X ",(unsigned int)*p);
        console_Print(command_buffer);
        p++;
        count--;
        if (0 == (count & 0x0000007))
            console_Print("\n");
    }
    return 0;
}
static int P1_w32(char** args)
{
    uint32_t *p    = (uint32_t*) strtol(args[1],NULL,16);
    uint32_t value = (uint32_t)  strtol(args[2],NULL,16);
    *p = value;
    return 0;

}
static int P1_r16(char** args)
{
    uint16_t *p = (uint16_t*) strtol(args[1],NULL,16);
    uint32_t count = (uint32_t) strtol(args[2],NULL,10);
    while(count)
    {
        sprintf(command_buffer,"%04X ",(unsigned short)*p);
        console_Print(command_buffer);
        p++;
        count--;
        if (0 == (count & 0x0000007))
            console_Print("\n");
    }   return 0;

}
static int P1_w16(char** args)
{
    uint16_t *p    = (uint16_t*) strtol(args[1],NULL,16);
    uint16_t value = (uint16_t)  strtol(args[2],NULL,16);
    *p = value;
    return 0;

}
static int P1_r8(char** args)
{
    uint8_t *p = (uint8_t*) strtol(args[1],NULL,16);
    uint32_t count = (uint32_t) strtol(args[2],NULL,10);
    while(count)
    {
        sprintf(command_buffer,"%02X ",(unsigned char)*p);
        console_Print(command_buffer);
        p++;
        count--;
        if (0 == (count & 0x0000007))
            console_Print("\n");
    }
    return 0;

}
static int P1_w8(char** args)
{
    uint8_t *p    = (uint8_t*) strtol(args[1],NULL,16);
    uint8_t value = (uint8_t)  strtol(args[2],NULL,16);
    *p = value;
    return 0;

}




