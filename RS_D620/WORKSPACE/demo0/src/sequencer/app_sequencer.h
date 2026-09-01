/*
 *     APP_SEQUENCER.H
 *
 * Sequencer application header file
 *
 */

#ifndef APP_SEQUENCER_H_
#define APP_SEQUENCER_H_
#include "application_common.h"
#if !APP_HAS_CONTROLPANEL
#error This app requires a control panel be defined.
#endif
/* USER INCLUDE */
typedef struct s_power_control {
uint8_t power_on;
} power_control_t;
/* PUBLISHED functions (weak overrides defined in src/base_app/application.c) */
int app_func_reset   (void);
int app_func_startup (void);
int app_func_restart (void);
int app_func_run     (void);
/* USER ADDITIONAL PUBLIC FUNCTIONS */

#endif /* APP_SEQUENCER_H_ */
