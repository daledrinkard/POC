/*
 * console_api.h
 *
 *  Created on: Aug 20, 2024
 *      Author: daled
 */

#ifndef CONSOLE_CONSOLE_API_H_
#define CONSOLE_CONSOLE_API_H_
#include "console_cfg.h"
typedef enum console12_err_e {C12OK=0,C12Fail} console12_err_t;
typedef struct {
    uint8_t *cmd;
    uint8_t *args[CONSOLE_ARG_COUNT];
    uint32_t len;
    uint32_t flags;
} console12_args_t;
typedef struct console12_s {
    console12_err_t (*open) (uint8_t *,uint32_t,void *);
    uint8_t *rxbuffer;
    uint32_t rxbuffersize;
    uint32_t rxbufferidx;
    uint32_t (*write) (uint8_t*,uint32_t);
    uint32_t (*read)  (uint8_t*,uint32_t);
    uint32_t (*prints) (char*);
    void (*rx_cb) (console12_args_t*);
    void *phy;
}console12_t;

extern console12_t CON12;
void console_warmstart(void);
#endif /* CONSOLE_CONSOLE_API_H_ */
