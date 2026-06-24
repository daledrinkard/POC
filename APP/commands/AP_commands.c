/*
 * AP_commands.c
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
#include "application_common.h"


extern char command_buffer[];

static int AP_app(char** args);

const char AP_cmds_name[] = "AP Commands";
const command_t AP_cmds[] =
{
  {"app",&AP_app,"manipulate app object"},
  {0,0,0}
};
extern char command_buffer[]; /* */
extern volatile cpan_t ControlPanel;


const char* SYSFLG_asci[] = {
"USB_POLL \n",
"DECODE \n",
"CONSOLE \n",
"CONTINUE \n",
"DISPLAY \n",
"GLCDC_RDY \n",
"spare0 \n",
"spare1 \n",
"BUF0_FULL \n",
"BUF1_FULL \n",
"RGB0_RDY \n",
"RGB1_RDY \n",
"USB_START \n",
"USB_ATTCH \n",
"USB_STOP \n",
"CAMERA_RDY \n",
"PACKET_RDY \n",
"UVC_START \n",
"UVC_STOP \n",
"PACKET_EOF \n",
"PACKET_DATA \n",
"PACKET_SOF \n",
"UVC_ERROR  \n",
"FRAME_TICK \n",
"AI_BUF_RDY \n",
"AI_INF_DONE \n",
"AI_RDY \n",
"MON_POLL \n",
NULL

};
static int AP_app(char** args) //@@@  CPU usage should be attached to the control panel, not the application
{
    FSP_PARAMETER_NOT_USED(args);
    ULONG flags;
    int a,b;
    a = App.CPU_usage / 100;
    b = App.CPU_usage % 100;
    sprintf(command_buffer,"\n App: CPU usage %d.%d \n",a,b);
    console_Print(command_buffer);
    console_Print(" SYSFLGs:\n");
    _tx_event_flags_get(&g_system_event,0,TX_OR,&flags,TX_NO_WAIT);
    int i=0;
    do
    {
        if ((1<<i) & flags)
        {
            console_Print("  ");
            console_Print((char*) SYSFLG_asci[i]);
        }
        i++;
    } while (SYSFLG_asci[i] != NULL);
    return 0;
}





