/*
 *     SIMULATOR.H
 *
 * Simulator module header file
 *
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_
#include <stdint.h>
/* USER INCLUDE */
#define SIMULATOR_OP_WAIT    (0)
#define SIMULATOR_OP_RAISE   (1)
#define SIMULATOR_OP_LOWER   (2)
#define SIMULATOR_OP_REPEAT  (3)
#define SIMULATOR_OP_FORCE   (4)
#define SIMULATOR_OP_END     (0xFF)

typedef struct s_srt {
    uint8_t op; 
    uint8_t div;
    uint16_t step;
    uint32_t delay; /* in 100uS */
}   srt_t;
typedef struct s_simulator {
    uint8_t chan;
    uint8_t state;
    uint16_t flags;
    uint32_t cnt; /* in 100uS */
    uint32_t div_cnt;
    srt_t *srt;
    srt_t *p_srt;
} simulator_t;
extern simulator_t Simulator[2];
/* PUBLISHED functions */
int simulator_init  (void);
int simulator_run   (void);
void simulator_start(int);

/* USER ADDITIONAL PUBLIC FUNCTIONS */

#endif /* SIMULATOR_H_ */
