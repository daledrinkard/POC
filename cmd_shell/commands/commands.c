/*
 * commands.c
 *
 *  Created on: Aug 6, 2025
 *      Author: daled
 */


#include "hal_data.h"
#include"commands.h"
#include <stdlib.h>

console_t *Console;

static int P1_echo(char** args);
static int P1_pin(char** args);


const command_t P1[3] =
{
  {"echo",&P1_echo,"echo\n"},
  {"pin",&P1_pin,"pin Pt Pn val"},
  {0,0,0}
};
int P1_Open(console_t *c)
{
    if (c != NULL)
    {
      Console = c;
      return 0;
    }
    return -1;
}

/*  template
static int P1_(char* args)
{
    return 0;
}
*/
static int P1_echo(char** args)
{
    console_Print(args[1]);
    return 0;
}
static int P1_pin(char** args)
{
    int pin = atoi(args[1] );
    return 0;
}

