/*
 * console.h
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */

#ifndef CONSOLE_CONSOLE_H_
#define CONSOLE_CONSOLE_H_

#include <stdint.h>
#include <stddef.h>
#include "console_config.h"

/*
 *    DEFINES
 */
#define CONSOLE_FLAG_TX_BUSY  (0x00000001)
#define CONSOLE_FLAG_TX_DONE  (0x00000002)
#define CONSOLE_FLAG_ECHO     (0x00000004)
#define CONSOLE_FLAG_QUOTE    (0x00000008)

#define CONSOLE_SUCCESS        0
#define CONSOLE_FAILURE       -1
/*
 *    TYPE definitions
 */
typedef enum e_console_events
{
  CONSOLE_NULL_EVENT = 0,
  CONSOLE_LF_EVENT = 1
} console_event_t;
typedef enum e_console_data_type_s
{
  CONSOLE_DATA_INT,
  CONSOLE_DATA_FLOAT,
  CONSOLE_DATA_STRING
}console_data_type_t;
typedef struct command_s {
    char* name;
    int (*cmd)(char**);
    char* help;
}command_t;
typedef struct console_data_s {
    uint8_t r;
    uint8_t c;
    uint8_t w;
    console_data_type_t data_type;
    void *data;
}console_data_t;
typedef struct console_pane_s {
    uint8_t row;
    uint8_t col;
    console_data_t *datalist;
}console_pane_t;
typedef struct console_cb_s {
    console_event_t event;
    void *ctx;
}console_cb_t;
typedef struct s_exec_frame {
    char *args[COMMAND_ARG_CNT];
    int arg_cnt;
    volatile uint32_t flags;
}exec_frame_t;
typedef struct console_s {
    char name[CONSOLE_NAME_SIZE];
    volatile uint32_t flags;
    int (*write)(char* s,int len,void *ctx);
    int (*read)(char* s,int len,void *ctx);
    void (*cb)(console_event_t,void*);

    command_t* commands;
    char *rx_buffer;
    int rx_idx;
    int rx_size;
}console_t;
typedef int dds;
/*
 *    API functions
 */
int console_Open(console_t*);
int console_Print(char*);
int console_Exec(char*);
int console_UpdatePane(console_pane_t *);
/*
 *    CALLBACK
 */
int console_CB(char ch);

#endif /* CONSOLE_CONSOLE_H_ */
