/*
 * CAM_commands.c
 *
 *  Created on: Jun 1, 2026
 *      Author: daled
 */
#include "hal_data.h"
#include"commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "usbcamera_thread.h"
#include "CPAN/cpan.h"
#include "application_common.h"

extern char command_buffer[];
#if 1 // Camera commands

static int CAM_stat(char** args);
#endif

const char CAM_cmds_name[] = "CAM Commands";
const command_t CAM_cmds[] =
{
 {"cam", &CAM_stat,"camera status and control"},
 {0,0,0}
};

const char* CAM_FLAG_ansi[] = {
"LAST_FID",
"FID_CHANGE",
"EOF",
"DATA",
"TOGGLE",
"AI_BUSY",
"ATTACHED",
"_STREAMING",
"ERROR",
NULL
};
#if 1 // CAM commands
static void exCAM_report(void);
static void exCAM_modify(void);
static int CAM_stat(char** args)
{
    uint32_t exargs[1];
    if (NULL == args[1])
    {
        exCAM_report();
        return 0;
    }
    do {
        switch (args[1][0]) {
            case 's': case 'S': tx_event_flags_set(&g_system_event,SYSFLG_USB_START,TX_OR);  continue;
            case 'h': case 'H': tx_event_flags_set(&g_system_event,SYSFLG_USB_STOP,TX_OR);   continue;
            case 'r': case 'R':
            if (Camera.state != CAM_STATE_ATTACH) /* then the camera is running */
            {
                console_Print("\nThe camera is streaming.\n");
                return -1;
            }
            ControlPanel.stat |= CPAN_STAT_RESTART_CAM;
            continue;
            case 'f': case 'F':
                if (NULL == args[2])
                {
                    console_Print("\nmust provide a value\n");
                    return -1;
                }
                exargs[0] = (uint32_t) strtol(args[2],NULL,10);
                Camera.request_fps = exargs[0];
                continue;
        }
        return -1;
    } while(0);
    return 0;
}
static void exCAM_report(void)
{
    sprintf(command_buffer,"Camera:\n  size= ( %d X %d x %dbpp )\n",
            Camera.width,Camera.height,Camera.bpp);
    console_Print(command_buffer);
    sprintf(command_buffer,"  state:     %d\n  uvc_index: %d\n  uvc_max_packet: %d\n  stream_state: %d\n",
            Camera.state,Camera.uvc_index,Camera.max_packet_buffer_size,Camera.stream_state);
    console_Print(command_buffer);
    sprintf(command_buffer,"  request_fps: %d\n  error_code:  %d\n  flag:        0x%04X\n",
            Camera.request_fps,Camera.error_code,Camera.flag);
    console_Print(command_buffer);


    int i=0;
    do
    {
        if ((1<<i) & Camera.flag)
        {
            console_Print("\n  ");
            console_Print((char*) CAM_FLAG_ansi[i]);
        }
        i++;
    } while (CAM_FLAG_ansi[i] != NULL);









    sprintf(command_buffer,"\n  frame_count: %d\n  field_count: %d\n  poll_rate:   %d\n  frame_size:  %d\n",
            Camera.frame_count,Camera.field_count,Camera.poll_rate,Camera.frame_size);
    console_Print(command_buffer);
    if (Camera.frame_timer_ticks > 1000)
    {
        sprintf(command_buffer," frame time: %duS (~%d fps)",Camera.frame_timer_ticks,1000000/Camera.frame_timer_ticks);
    }
    else
    {
        sprintf(command_buffer," frame time: <TOO LOW>\n");
    }
    console_Print(command_buffer);

    /*
    ULONG frame_format;
    ULONG max_packet_buffer_size;
    ULONG event_flag;
    UX_HOST_CLASS_VIDEO  * volatile video_host_class;
    UCHAR                *video_data_ptrs[JPEG_BUFFER_NUMBER];
    UINT                 completed_transfer_length[JPEG_BUFFER_NUMBER];
    void                 (*uvc_transfer_request_done_callback)(UX_TRANSFER *transfer_request);
     */
}
static void exCAM_report_flags(void)
{

}
static void exCAM_modify(void)
{

}

#endif
