/*
 * pop.h
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */

#ifndef POP_POP_H_
#define POP_POP_H_

#include "hal_data.h"
#include "pop_config.h"



void POP0(void);
void DROP0(void);
void TOG0(void);
void POP1(void);
void DROP1(void);
void TOG1(void);
void POP2(void);
void DROP2(void);
void TOG2(void);
void POP3(void);
void DROP3(void);
void TOG3(void);
void POP4(void);
void DROP4(void);
void TOG4(void);
void POP5(void);
void DROP5(void);
void TOG5(void);
void POP6(void);
void DROP6(void);
void TOG6(void);
void POP7(void);
void DROP7(void);
void TOG7(void);

void POPR(void);
void POPG(void);
void POPB(void);

void DROPR(void);
void DROPG(void);
void DROPB(void);

void TOGR(void);
void TOGG(void);
void TOGB(void);

void POP_TEST(void);

void T1I(void);
void T1S(void);
uint32_t T1E(void);

#endif /* POP_POP_H_ */
