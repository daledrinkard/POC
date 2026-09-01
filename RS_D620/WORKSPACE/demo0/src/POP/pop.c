/*
 * POP.c
 *
 *  Created on: Aug 5, 2025
 *      Author: daled
 */


#include "POP.h"
#define PIN_LO 0x00000804
#define PIN_HI 0x00000805
#define PIN_IN(_a_,_b_) ((R_PFS->PORT[_a_].PIN[_b_].PmnPFS & 0x00000002) >> 1)

timer_status_t status_end;
void POP0(void)
{
    R_PFS->PORT[PP0].PIN[PN0].PmnPFS = PIN_HI;
}
void POP1(void)
{
    R_PFS->PORT[PP1].PIN[PN1].PmnPFS  = PIN_HI;
}
void POP2(void)
{
    R_PFS->PORT[PP2].PIN[PN2].PmnPFS = PIN_HI;
}
void POP3(void)
{
    R_PFS->PORT[PP3].PIN[PN3].PmnPFS = PIN_HI;
}
void POP4(void)
{
    R_PFS->PORT[PP4].PIN[PN4].PmnPFS = PIN_HI;
}
void POP5(void)
{
    R_PFS->PORT[PP5].PIN[PN5].PmnPFS = PIN_HI;
}
void POP6(void)
{
    R_PFS->PORT[PP6].PIN[PN6].PmnPFS = PIN_HI;
}
void POP7(void)
{
    R_PFS->PORT[PP7].PIN[PN7].PmnPFS = PIN_HI;
}
void POPR(void)
{
    R_PFS->PORT[PPR].PIN[PNR].PmnPFS = PIN_HI;
}
void POPG(void)
{
    R_PFS->PORT[PPG].PIN[PNG].PmnPFS = PIN_HI;
}
void POPB(void)
{
    R_PFS->PORT[PPB].PIN[PNB].PmnPFS = PIN_HI;
}

void TOG0(void)
{
    R_PFS->PORT[PP0].PIN[PN0].PmnPFS = (PIN_IN(PP0,PN0)) ? PIN_LO : PIN_HI;
}
void TOG1(void)
{
    R_PFS->PORT[PP1].PIN[PN1].PmnPFS = (PIN_IN(PP1,PN1)) ? PIN_LO : PIN_HI;
}
void TOG2(void)
{
    R_PFS->PORT[PP2].PIN[PN2].PmnPFS = (PIN_IN(PP2,PN2)) ? PIN_LO : PIN_HI;
}
void TOG3(void)
{
    R_PFS->PORT[PP3].PIN[PN3].PmnPFS = (PIN_IN(PP3,PN3)) ? PIN_LO : PIN_HI;
}
void TOG4(void)
{
    R_PFS->PORT[PP4].PIN[PN4].PmnPFS = (PIN_IN(PP4,PN4)) ? PIN_LO : PIN_HI;
}
void TOG5(void)
{
    R_PFS->PORT[PP5].PIN[PN5].PmnPFS = (PIN_IN(PP5,PN5)) ? PIN_LO : PIN_HI;
}
void TOG6(void)
{
    R_PFS->PORT[PP6].PIN[PN6].PmnPFS = (PIN_IN(PP6,PN6)) ? PIN_LO : PIN_HI;
}
void TOG7(void)
{
    R_PFS->PORT[PP7].PIN[PN7].PmnPFS = (PIN_IN(PP7,PN7)) ? PIN_LO : PIN_HI;
}
void TOGR(void)
{
    R_PFS->PORT[PPR].PIN[PNR].PmnPFS = (PIN_IN(PPR,PNR)) ? PIN_LO : PIN_HI;
}
void TOGG(void)
{
    R_PFS->PORT[PPG].PIN[PNG].PmnPFS = (PIN_IN(PPG,PNG)) ? PIN_LO : PIN_HI;
}
void TOGB(void)
{
    R_PFS->PORT[PPB].PIN[PNB].PmnPFS = (PIN_IN(PPB,PNB)) ? PIN_LO : PIN_HI;
}



void DROP0(void)
{
  R_PFS->PORT[PP0].PIN[PN0].PmnPFS = PIN_LO;
}
void DROP1(void)
{
  R_PFS->PORT[PP1].PIN[PN1].PmnPFS = PIN_LO;
}
void DROP2(void)
{
  R_PFS->PORT[PP2].PIN[PN2].PmnPFS = PIN_LO;
}
void DROP3(void)
{
  R_PFS->PORT[PP3].PIN[PN3].PmnPFS = PIN_LO;
}
void DROP4(void)
{
  R_PFS->PORT[PP4].PIN[PN4].PmnPFS = PIN_LO;
}
void DROP5(void)
{
  R_PFS->PORT[PP5].PIN[PN5].PmnPFS = PIN_LO;
}
void DROP6(void)
{
  R_PFS->PORT[PP6].PIN[PN6].PmnPFS = PIN_LO;
}
void DROP7(void)
{
  R_PFS->PORT[PP7].PIN[PN7].PmnPFS = PIN_LO;
}
void DROPR(void)
{
    R_PFS->PORT[PPR].PIN[PNR].PmnPFS = PIN_LO;
}
void DROPG(void)
{
    R_PFS->PORT[PPG].PIN[PNG].PmnPFS = PIN_LO;
}
void DROPB(void)
{
    R_PFS->PORT[PPB].PIN[PNB].PmnPFS = PIN_LO;
}

void POP_TEST(void)
{
    uint32_t rxr[8];  // save the state of all the pins
    rxr[0] = R_PFS->PORT[PP0].PIN[PN0].PmnPFS;
    rxr[1] = R_PFS->PORT[PP1].PIN[PN1].PmnPFS;
    rxr[2] = R_PFS->PORT[PP2].PIN[PN2].PmnPFS;
    rxr[3] = R_PFS->PORT[PP3].PIN[PN3].PmnPFS;
    rxr[4] = R_PFS->PORT[PP4].PIN[PN4].PmnPFS;
    rxr[5] = R_PFS->PORT[PP5].PIN[PN5].PmnPFS;
    rxr[6] = R_PFS->PORT[PP6].PIN[PN6].PmnPFS;
    rxr[7] = R_PFS->PORT[PP7].PIN[PN7].PmnPFS;

    DROP0();
    DROP1();
    DROP2();
    DROP3();
    DROP4();
    DROP5();
    DROP6();
    DROP7();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP0();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP1();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP2();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP3();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP4();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP5();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP6();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    POP7();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG0();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG1();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG2();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG3();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG4();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG5();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG6();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    TOG7();
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    R_PFS->PORT[PP0].PIN[PN0].PmnPFS = rxr[0];
    R_PFS->PORT[PP1].PIN[PN1].PmnPFS = rxr[1];
    R_PFS->PORT[PP2].PIN[PN2].PmnPFS = rxr[2];
    R_PFS->PORT[PP3].PIN[PN3].PmnPFS = rxr[3];
    R_PFS->PORT[PP4].PIN[PN4].PmnPFS = rxr[4];
    R_PFS->PORT[PP5].PIN[PN5].PmnPFS = rxr[5];
    R_PFS->PORT[PP6].PIN[PN6].PmnPFS = rxr[6];
    R_PFS->PORT[PP7].PIN[PN7].PmnPFS = rxr[7];
}
void T1I(void)
{
    R_GPT_Open(&g_T1_ctrl, &g_T1_cfg);
    R_GPT_Enable(&g_T1_ctrl);
}
void T1S(void)
{
    R_GPT_Reset(&g_T1_ctrl);
    R_GPT_Start(&g_T1_ctrl);

}
uint32_t T1E(void)
{
    R_GPT_StatusGet(&g_T1_ctrl, &status_end);
    R_GPT_Stop(&g_T1_ctrl);
    return (status_end.counter / GPT_CLK_MHZ);

}
