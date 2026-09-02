#include "simulator.h"
#define DIV_MS (10)
#define DIV_100US (1)
const srt_t srt_table_0[] = 
{
    {SIMULATOR_OP_WAIT,  DIV_100US,     0, 10},
    {SIMULATOR_OP_RAISE, DIV_100US,    64, 500},
    {SIMULATOR_OP_WAIT,  DIV_MS,        0, 1000},
    {SIMULATOR_OP_FORCE, DIV_MS,    40000, 1000},
    {SIMULATOR_OP_END, 0, 0, 0}
};
const srt_t srt_table_1[] = 
{
    {SIMULATOR_OP_WAIT,  DIV_MS,        0, 1000},
    {SIMULATOR_OP_RAISE, DIV_100US,    10, 1000},
    {SIMULATOR_OP_FORCE, DIV_MS,    32768, 1000},
    {SIMULATOR_OP_END, 0, 0, 0}
};

const simulator_t simulator_initial[2] = {
    {
    .chan = 0,
    .state = 0,
    .flags = 0,
    .cnt = 0,
    .div_cnt = 0,
    .srt = (srt_t*) srt_table_0,
    .p_srt = (srt_t*) srt_table_0
    },
    {
    .chan = 1,
    .state = 0,
    .flags = 0,
    .cnt = 0,
    .div_cnt = 0,
    .srt = (srt_t*) srt_table_1,
    .p_srt = (srt_t*) srt_table_1
    }
};


