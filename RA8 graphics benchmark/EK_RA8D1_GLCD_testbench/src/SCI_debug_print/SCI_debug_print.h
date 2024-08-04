/*
 * SCI_debug_print.h
 *
 *  Created on: Jun 13, 2024
 *      Author: daled
 */

#ifndef SCI_DEBUG_PRINT_SCI_DEBUG_PRINT_H_
#define SCI_DEBUG_PRINT_SCI_DEBUG_PRINT_H_

#include "SCI_debug_print_cfg.h"
// API
int SCI_write(int file, char *ptr, int len);
int SCI_close(int file);
int SCI_fstat(int file, struct stat *st);
int SCI_isatty(int file);
int SCI_read(int file, char *ptr, int len);
int SCI_lseek(int file, int ptr, int dir);

// published
void Printf(char *s);
#endif /* SCI_DEBUG_PRINT_SCI_DEBUG_PRINT_H_ */
