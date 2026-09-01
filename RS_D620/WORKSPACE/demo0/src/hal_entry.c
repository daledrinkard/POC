/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"
#include "r_ioport.h"

/* base_app state-machine entry point (see src/base_app/application.c) */
extern void app_entry(void);

/*******************************************************************************************************************//**
 * @brief  base_app example application
 *
 * Hands control to the base_app state machine (app_entry) provided by src/base_app/application.c.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif

    /* Wake up 2nd core if this is first core and we are inside a multicore project. */
#if (0 == _RA_CORE) && (1 == BSP_MULTICORE_PROJECT) && !BSP_TZ_NONSECURE_BUILD
    R_BSP_SecondaryCoreStart();
#endif
    R_PORT1->PODR = 0x00000000; /* set all pins low */
    R_IOPORT_PortDirectionSet(&g_ioport_ctrl, BSP_IO_PORT_01, 0xFFFF, 0xFFFF); /* set all PORT1 pins to output */
    /* no whole-port "drive strength" API exists in r_ioport; it's a per-pin PFS field set via pin config */
    /*
        PCNTR1 (RW) [GPIO DIRECTION 1=output] 31:16
                    [GPIO state] 15:0
        PCNTR2 (R)  [read only current state of pin] 31:16
                    [read only latched inputs via ELC event] 15:0
        PCNTR3  (W) [write a 1 to set the pin state HIGH] 31:16  
                    [write a 1 to set the pin state LOW] 15:0
                    [  0=no effect on output  ]
        PCNTR4  (W) [write a 1 to set this pin HIGH on an ELC event] 31:16
                    [write a 1 to set this pin LOW on an ELC event ] 15:0
                    [  0=no effect on output  ]
    
    */
   R_PORT1->PCNTR3 = 0x00000000;
   R_PORT1->PCNTR4 = 0x00000000;

    app_entry();
}
