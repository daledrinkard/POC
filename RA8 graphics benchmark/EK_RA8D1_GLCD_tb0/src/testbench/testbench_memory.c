/*
 * testbench_memory.c
 *
 *  Created on: Aug 12, 2024
 *      Author: daled
 */
#include <stdint.h>
#include "testbench_cfg.h"
#include "testbench_memory.h"

extern uint32_t MEM_HEAP[]; //MEM_SIZE_OF_HEAP];

static MEM_errors_t MEM_Allocate(uint32_t **mem,uint32_t size);
static MEM_errors_t MEM_Clear(uint32_t size);
MEM_t MEM = { .ridx = 0, .allocate = MEM_Allocate, .clear = MEM_Clear };
static MEM_errors_t MEM_Allocate(uint32_t **mem,uint32_t size)
{
    *mem = &MEM_HEAP[MEM.ridx];
    MEM.ridx += size;
    if (MEM.ridx > MEM_SIZE_OF_HEAP)
        return MEM_FAIL;
    return MEM_OK;
}
static MEM_errors_t MEM_Clear(uint32_t size)
{
    for(uint32_t i = 0;i<size;i++)
    {
        MEM_HEAP[i] = 0;
    }
    return MEM_OK;
}
