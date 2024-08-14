/*
 * testbench_memory.h
 *
 *  Created on: Aug 12, 2024
 *      Author: daled
 */

#ifndef TESTBENCH_TESTBENCH_MEMORY_H_
#define TESTBENCH_TESTBENCH_MEMORY_H_

#include "dave_driver.h"
typedef enum MEM_errors_e {MEM_OK,MEM_FAIL} MEM_errors_t;
typedef struct MEM_s {
    uint32_t ridx;
    MEM_errors_t (*allocate) (d2_point **,uint32_t size);
    MEM_errors_t (*clear) (uint32_t size);
}MEM_t;

extern MEM_t MEM;


#endif /* TESTBENCH_TESTBENCH_MEMORY_H_ */
