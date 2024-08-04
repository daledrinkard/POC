/*
 * test_core.h
 *
 *  Created on: Jun 17, 2024
 *      Author: daled
 */

#ifndef TEST_CORE_TEST_CORE_H_
#define TEST_CORE_TEST_CORE_H_

#define RED_PIXELS (0xf800f800)
#define GREEN_PIXELS (0x07E007E0)
#define BLUE_PIXELS (0x001F001F)
#define QRED_PIXELS (0xf800f800f800f800)
#define QGREEN_PIXELS (0x07E007E00x07E007E0)
#define QBLUE_PIXELS (0x001F001F0x001F001F)

#include "test_core_cfg.h"
#include "bsp_api.h"
#include "r_transfer_api.h"

typedef struct core_ctrl_s {
    volatile uint32_t code_timer_done;
    volatile uint32_t dma_done;
    uint32_t capture32[32];
    uint32_t capture32_idx;
    uint32_t code_timer_counts;
    uint32_t loop_counter;
    uint32_t current_test;
    uint32_t current_pixel_idx;
    uint32_t image_size;
    uint32_t core_mode;
    uint16_t *p_fb;
    uint32_t *xp_fb;
    uint8_t *bp_fb;
    uint64_t *qxp_fb;
    transfer_info_t tilkx;
}core_ctrl_t;
void test_core_init(void);

uint32_t test_run(uint32_t) BSP_PLACE_IN_SECTION(".itcm_data");
extern core_ctrl_t Core;
#endif /* TEST_CORE_TEST_CORE_H_ */
