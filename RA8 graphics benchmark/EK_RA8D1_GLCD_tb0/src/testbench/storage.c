/*
 * storage.c
 *
 *  Created on: Aug 15, 2024
 *      Author: daled
 */

#include <stdint.h>
#include "storage.h"
#include "draw_core/draw_core_cfg.h"
#include "Console/console_cfg.h"
/*
 *    These buffers collect timing samples
 */
uint32_t Sample_Buffer_Animate[CAPTURE_BUFFER_SIZE] BSP_PLACE_IN_SECTION(".dtcm_data");
uint32_t Sample_Buffer_Render[CAPTURE_BUFFER_SIZE]  BSP_PLACE_IN_SECTION(".dtcm_data");
uint32_t Sample_Buffer_VSYNC[CAPTURE_BUFFER_SIZE]   BSP_PLACE_IN_SECTION(".dtcm_data");


/*
 *   Animate and Render lists.
 */
dr_animate_t Animate[DRAW_CORE_NUMBER_OF_ANIMATIONS] BSP_PLACE_IN_SECTION(".dtcm_data");
dr_render_t Render[DRAW_CORE_NUMBER_OF_RENDERS] BSP_PLACE_IN_SECTION(".dtcm_data");

/*
 *   Heap used for dr_points
 */
uint32_t MEM_HEAP[MEM_SIZE_OF_HEAP] BSP_PLACE_IN_SECTION(".dtcm_data");

/*
 *    Console
 */
uint8_t Console_RX_Buffer[CONSOLE_BUFFER_SIZE];

/*
 *   Testbench buffers.  This is the command buffer copied from the console (maybe)
 */
char TBRXBuffer[TB_RX_BUFFER_SIZE];// BSP_PLACE_IN_SECTION(".dtcm_data");
char TBTXBuffer[TB_TX_BUFFER_SIZE];// BSP_PLACE_IN_SECTION(".dtcm_data");
