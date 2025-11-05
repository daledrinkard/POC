/*
 * dbg_util.h
 *
 *  Created on: May 16, 2025
 *      Author: daled
 */

#ifndef DBG_UTIL_H_
#define DBG_UTIL_H_

#include "hal_data.h"
#include "flash_map_backend/flash_map_backend.h"

typedef struct flash_area fa_t;

uint8_t mem_blank(uint8_t* p,uint32_t len);
void dbg_print_map(void*);
void dbg_print_data(uint32_t *p,uint32_t len);
void dbg_print_grid(uint32_t *p,uint32_t sector_size,uint32_t sectors,uint32_t rows);
void dbg_print_fmap(fa_t *p_f);
void dbg_print_rsp_header(struct boot_rsp *p_rsp);
char *dbg_swap_type(int i);

#endif /* DBG_UTIL_H_ */
