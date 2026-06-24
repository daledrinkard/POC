/*
 * CP_commands.c
 *
 *  Created on: Jun 1, 2026
 *      Author: daled
 */
#include "hal_data.h"
#include"commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CPAN/cpan.h"
extern char command_buffer[];
extern volatile cpan_t ControlPanel;

#if 1 // Control panel projects
static int CP_stat(char** args);
static int CP_creg(char** args);
static int CP_ofs(char** args);
static int CP_dg(char** args);
static int CP_sau(char** args);
static int CP_swap(char** args);
static int CP_f(char** args);
static int CP_rst(char** args);
static int CP_tmr(char** args);
#endif
const command_t CP_cmds[] =
{
  {"ofs", &CP_ofs,"dump OFS registers"}, //@@@ these commands are not necessarily tied to the control panel
  {"dg",  &CP_dg,"dump memory grid"},    //@@@ this could be a P1 command
  {"sau", &CP_sau,"dump memory grid"},
  {"swap",&CP_swap,"execute a bank swap request"},
  {"f",   &CP_f,"flash utility"},
  {"stat",&CP_stat,"status flag"},
  {"rst", &CP_rst,"restart app"},
  {"creg",&CP_creg,"creg reg value"},
  {"tmr",&CP_tmr,"tmr Timer [T|P] [value]"},  //@@@ needs flushing out
  //{"adc",&CP_adc,"adc command stub"},
  {0,0,0}
};




#if 1 // CP commands
/*
 *   CP_dg   Dump a grid of the specified memory
 */
#define RAMFUNC __attribute__((section(".ram_code_from_flash")))

static void ex_print_grid(uint32_t *p,uint32_t sector_size,uint32_t sectors,uint32_t rows);
static int CP_dg(char** args)
{
    uint32_t exargs[4];
    for(uint32_t a=0;a<4;a++)
    {
        exargs[a] = (uint32_t) strtol(args[a+1],NULL,16);  // all args base 16
    }
    sprintf(command_buffer,"\nstart: 0x%08lX sector: %ldK\n",exargs[0],(exargs[1])/1024);
    console_Print(command_buffer);
    ex_print_grid((uint32_t*) exargs[0],exargs[1],exargs[2],exargs[3]);

    return 0;
}
volatile uint32_t* ppy;
static void ex_print_grid(uint32_t *p,uint32_t sector_size,uint32_t sectors,uint32_t rows)
{
    volatile uint32_t i,j,k,B1;
    volatile uint32_t *q;
    volatile uint32_t w_size = sector_size/4;
    ppy = p;
    char c;
    char *s;
    B1 = w_size-1;
    for(k=0;k<rows;k++)
    {
        s = command_buffer;
        q = p+(k*w_size*sectors);
        sprintf(command_buffer,"[0x%08lX] ",(uint32_t) q);
        console_Print(command_buffer);
        for(i=0;i<sectors;i++)
        {
            c='.';
            for(j=0;j<w_size;j++)
            {
                if(0xFFFFFFFF != *q)
                {
                    if ('.' == c)
                    {
                        c = (*q == 0x96F3B83D) ? 'M' : '*';
                    }
                    if ((j & B1) == (B1 & 0xFFFFFFC0))  // first word of last 256 bytes in a sector-size segment...
                    {
                        if (*q == 0xFFFFFF01)
                        {
                            c = 'C';
                        }
                    }
                    //q = q + (w_size-j); // look no further
                    //break;
                }
                q++;
            }
            *s = c;
            s++;
        }
        *s = '\n';
        s++;
        *s = 0;
        console_Print(command_buffer);
    }
}

/*
 *     CP_ofs  dump OFS register info
 */
static void ex_DUALSEL(uint32_t x);
static void ex_BANKSEL_SEL(uint32_t x);
static void ex_BANKSEL(uint32_t x);
static void s_ex_BANKSEL_SEL(uint32_t x,char a,char b,char c);

static void ex_DUALSEL(uint32_t x)
{
    console_SetColor(CONSOLE_COLOR_NORMAL);
     switch (x & 0x00000007) {
         case 0:
             console_Print("DUAL mode");
             break;
         case 7:
             console_Print("LINEAR mode");
             break;
         default:
             console_SetColor(CONSOLE_COLOR_END);
             console_Print("INVALID");
             console_SetColor(CONSOLE_COLOR_NORMAL);
     }
}
static void s_ex_BANKSEL_SEL(uint32_t x,char a,char b,char c)
{
    char *q;
    char *p;
    console_SetColor(CONSOLE_COLOR_NORMAL);
    q = command_buffer;
    sprintf(q," --> BLCKSWP: ");
    while(*q)q++;
    for(int i=22;i>15;i--)
    {
            p = (x & (1 << i)) ? (char*) ansi_estr[CONSOLE_COLOR_YELLOW] : (char*) ansi_estr[CONSOLE_COLOR_GREEN];
            while(*p)
            {
                *q = *p;
                q++;p++;
            }
            *q = (x & (1 << i)) ? a : b;
            q++;
    }
    *q=0;
    console_Print(command_buffer);
    console_SetColor(CONSOLE_COLOR_NORMAL);
    console_Print("  BANKSWP: ");
    if (c == 0)
    switch(x & 0x0000007) {
        case 0:
            console_SetColor(CONSOLE_COLOR_GREEN);
            console_Print("BANKSEL_SEC");
            break;
        case 7:
            console_SetColor(CONSOLE_COLOR_YELLOW);
            console_Print("BANKSEL");
            break;
        default:
            console_SetColor(CONSOLE_COLOR_RED);
            console_Print("BAD VALUE");
            break;
    }
    if (c == 1)
    switch(x & 0x0000007) {
        case 0:
            console_SetColor(CONSOLE_COLOR_YELLOW);
            console_Print("SWAPPED");
            break;
        case 7:
            console_SetColor(CONSOLE_COLOR_GREEN);
            console_Print("NORMAL");
            break;
        default:
            console_SetColor(CONSOLE_COLOR_RED);
            console_Print("BAD VALUE");
            break;
    }
    console_SetColor(CONSOLE_COLOR_NORMAL);
}
static void ex_BANKSEL_SEL(uint32_t x)
{
    s_ex_BANKSEL_SEL(x,'N','S',0);
}
static void ex_BANKSEL(uint32_t x)
{
    s_ex_BANKSEL_SEL(x,'D','E',1);
}

typedef struct s_bh {
    char* n;
    uint32_t *p;
    uint32_t b;
    void (*e)(uint32_t);
} bh_t;
const bh_t XC[] = {
             {"OFS0",       (uint32_t*) 0x0300A100,1,NULL},
             {"OFS2",       (uint32_t*) 0x0300A104,1,NULL},
             {"DUALSEL",    (uint32_t*) 0x0300A110,1,&ex_DUALSEL},
             {"SAS",        (uint32_t*) 0x0300A134,1,NULL},
             {"OFS1",       (uint32_t*) 0x1300A180,1,NULL},     // non-secure
             {"OFS1_SEC",   (uint32_t*) 0x0300A200,1,NULL},     // secure
             {"OFS1_SEL"   ,(uint32_t*) 0x0300A280,1,NULL},
             {"BANKSEL",    (uint32_t*) 0x1300A190,1,&ex_BANKSEL},     //  non-secure
             {"BANKSEL_SEC",(uint32_t*) 0x0300A210,1,&ex_BANKSEL}, // secure
             {"BANKSEL_SEL",(uint32_t*) 0x0300A290,1,&ex_BANKSEL_SEL},
             {"BPS0"       ,(uint32_t*) 0x130001C0,4,NULL}, // non secure
             {"PBPS0"      ,(uint32_t*) 0x130001E0,4,NULL}, // non secure
             {"BPS0_SEC"   ,(uint32_t*) 0x0300A240,4,NULL}, // secure
             {"PBPS0_SEC"  ,(uint32_t*) 0x0300A260,4,NULL}, // secure
             {"BPS_SEL"    ,(uint32_t*) 0x0300A2C0,4,NULL},
             {NULL,NULL,0,NULL}
};
#define CP_OFNS (12)
static int CP_ofs(char** args)
{
    (void) ((args));
    uint32_t i,k;
    i=0;
    char *q;
    do
    {
        k = strlen(XC[i].n);
        q = (((uint32_t) XC[i].p & (uint32_t) 0x10000000) > 0) ?
            (char*) ansi_estr[CONSOLE_COLOR_YELLOW] : // non secure alias
            (char*) ansi_estr[CONSOLE_COLOR_GREEN];   //     secure alias
        console_Print(q);
        sprintf(command_buffer,"%s",XC[i].n);
        while(k<CP_OFNS)
        {
            command_buffer[k] = ' ';
            k++;
        }
        command_buffer[k] = 0;

        console_Print(command_buffer);
        for(uint32_t j=0;j<XC[i].b;j++)
        {
            sprintf(command_buffer,"%08X ",(unsigned int) *(XC[i].p+(j*1)));
            console_Print(command_buffer);
        }
        if (XC[i].e != NULL)
        {
            XC[i].e((uint32_t) *XC[i].p);
            //XC[i].e(0x000A0007);
        }
        console_Print("\n");
        i++;
    } while(XC[i].b > 0);
    console_SetColor(CONSOLE_COLOR_NORMAL);
    return 0;
}
/*
 *   CP_sau   dump SAU info
 */
volatile uint32_t R;
volatile uint32_t* pSAU_CTRL = (uint32_t*)  0xe000edd0; /* bit0 : enabled if set to 1, bit1 : non defined regions are 0: secure  1: non-secure */
volatile uint32_t* pSAU_TYPE = (uint32_t*)  0xe000edd4;
volatile uint32_t* pSAU_RNR  = (uint32_t*)  0xe000edd8;
volatile uint32_t* pSAU_RBAR = (uint32_t*)  0xe000eddC;
volatile uint32_t* pSAU_RLAR = (uint32_t*)  0xe000ede0;


static int CP_sau(char** args)
{
    FSP_PARAMETER_NOT_USED(args);
    uint32_t regions = *pSAU_TYPE;
    sprintf(command_buffer,"SAU_CTRL: %08X\n",(unsigned int) *pSAU_CTRL);
    console_Print(command_buffer);
    sprintf(command_buffer,"SAU_TYPE: %08X\n",(unsigned int) *pSAU_TYPE);
    console_Print(command_buffer);
    sprintf(command_buffer,"SAU_RNR:  %08X\n",(unsigned int) *pSAU_RNR);
    console_Print(command_buffer);
    for(uint32_t i=0;i<regions;i++)
    {
        *pSAU_RNR = i;
        sprintf(command_buffer,"R[%i] %08X %08X\n",(int) i,(unsigned int) *pSAU_RBAR,(unsigned int)  *pSAU_RLAR);
        console_Print(command_buffer);
    }
    return 0;
}
static int CP_swap(char** args)
{
#if 0 //@@@ no flash on P1
    fsp_err_t err;
    FSP_PARAMETER_NOT_USED(args);
    flash_instance_t *f = ControlPanel.flash;
    err = f->p_api->open(f->p_ctrl,f->p_cfg);
    if (FSP_SUCCESS != err) return -1;
    __disable_irq();
    err = f->p_api->bankSwap(f->p_ctrl);
    __enable_irq();
    __NVIC_SystemReset();
    return (FSP_SUCCESS == err) ? 0 : -1;
#endif
    return 0; //@@@
}


const char* CPAN_STAT_ansi[] = {
  "UPDATE ",
  "CAPTURE ",
  "JPEG0_CAPTURED ",
  "JPEG1_CAPTURED ",
  "RUN_DISPLAY ",
  "RUN_CONVERT ",
  "RUN_CAMERA ",
  "RUN_AI ",
  "RESTART_APP ",
  "THROTTLE_USB ",
  "RUN_UVC ",
  "HALT_ON_PCAP ",
  "RESTART_CAM ",
  "CHK_PKT_SIZE ",
  "BLOCK_USB ",
  "TEST_AI ",
  "RUN_MONITOR ",
  NULL
  };
static void exCP_stat_identify(void);
static int CP_stat(char** args)  /* stat s value | c value | r */
{
    uint32_t v;
    uint32_t flags = ControlPanel.stat;
    if (args[1] == NULL) /*read*/
    {
        console_Print("\nstat flags:");
        int i=0;
        do
        {
            if ((1<<i) & flags)
            {
                console_Print("\n  ");
                console_Print((char*) CPAN_STAT_ansi[i]);
            }
            i++;
        } while (CPAN_STAT_ansi[i] != NULL);
    }
    else /* write*/
    {
        if (NULL == args[2]) /* these options have no argument */
        {
            switch (args[1][0]) {
            case 'i':
            case 'I': exCP_stat_identify(); return 0;
            }
            return -1;
        }
        v = (uint32_t) strtol(args[2],NULL,16);
        switch (args[1][0]) {
        case 's':
        case 'S': ControlPanel.stat |= v; break;
        case 'c':
        case 'C': ControlPanel.stat &= ~v; break;
        }
    }
    return 0;
}
static void exCP_stat_identify(void)
{
    int i=0;
    do
    {
        sprintf(command_buffer,"(%08X) %s\n",(1 << i),CPAN_STAT_ansi[i]);
        console_Print(command_buffer);
        i++;
    } while (CPAN_STAT_ansi[i] != NULL);
}




static int CP_creg(char** args)
{
    uint32_t V;
    if (NULL == args[1]) return -1;
    uint32_t R = strtol(args[1],NULL,10);
    if (R > CPAN_NUM_OF_REGS) return -1;
    if (NULL == args[2])
    {
        sprintf(command_buffer,"%08X\n",ControlPanel.regs[R]);
        console_Print(command_buffer);
        return 0;
    }
    if ((args[2][1] == 'x') || (args[2][1] == 'X'))
      V = strtol(args[2],NULL,16);
    else
      V = strtol(args[2],NULL,10);
    ControlPanel.regs[R] = V;
    return 0;
}

static int CP_rst(char** args)
{
    FSP_PARAMETER_NOT_USED(args);
    ControlPanel.stat |= CPAN_STAT_RESTART_APP;
    return 0;
}
/**
 *  tmr 0 c value
 * @param args
 * @return
 */
static void ex_tmr_set_cycle(char,uint32_t);
static void ex_tmr_set_priority(char,uint32_t);
static int CP_tmr(char** args)
{
    uint32_t V;
    char C;
    if (NULL == args[1]) return -1;
    if (NULL == args[2]) return -1;
    if (NULL == args[3]) return -1;

    C = args[1][0];
    if ((args[3][1] == 'x') || (args[2][1] == 'X'))
      V = strtol(args[3],NULL,16);
    else
      V = strtol(args[3],NULL,10);
    switch (args[2][0]) {
        case 'c': case 'C':
            ex_tmr_set_cycle(C,V); break;
        case 'p': case 'P':
            ex_tmr_set_priority(C,V); break;
    }
    return 0;
}
static void ex_tmr_set_cycle(char C,uint32_t V)
{
    switch(C) {
        case '0' : ControlPanel.T0_cfg->duty_cycle_counts = V;
                   ControlPanel.T0_cfg->period_counts = V*2;
                   break;
    }

}
static void ex_tmr_set_priority(char C,uint32_t V)
{

}

/*
 *      f   flash utilities
 */
static void ex_flash_d(void);
static fsp_err_t ex_flash_e(uint32_t, uint32_t);
static fsp_err_t ex_flash_b(uint32_t,uint32_t,uint32_t);

static int CP_f(char** args)
{
#if 0 //@@@ comment out for P1
    /*  f e b# cnt   erase block(s)
     *
     */
    fsp_err_t err;
    uint32_t exargs[3];
    for(uint32_t a=0;a<3;a++)
    {
        exargs[a] = (uint32_t) strtol(args[a+2],NULL,16);  // all args base 16
    }
    flash_instance_t *f = ControlPanel.flash;
    err = f->p_api->open(f->p_ctrl,f->p_cfg);
    char c = (char) *args[1];
    switch (c) {
        case 'd' :  ex_flash_d(); break;
        case 'e' :  err = ex_flash_e(exargs[0],exargs[1]); break;
        case 'b' :  err = ex_flash_b(exargs[0],exargs[1],exargs[2]); break;
    }
    if (FSP_SUCCESS != err) console_Print("\nERROR\n");
    f->p_api->close(f->p_ctrl);
    return (FSP_SUCCESS == err) ? 0 : -1;
#endif
    return 0;
}
#if 0 //@@ comment this out for P1
static void ex_flash_d(void)
{
#define zROW 8
#define zCOL 2
    uint32_t *p,c;

//    q = (((uint32_t) XC[i].p & (uint32_t) 0x10000000) > 0) ?
//        (char*) ansi_estr[CONSOLE_COLOR_YELLOW] : // non secure alias
//        (char*) ansi_estr[CONSOLE_COLOR_GREEN];   //     secure alias
//    console_Print(q);
//    sprintf(command_buffer,"%s",XC[i].n);

    c=0;
    p = (uint32_t*) 0x02000000;
    for( int i=0;i<69;i++)
    {
        if (*p != 0xFFFFFFFF)
        {
            sprintf(command_buffer,"(%2i) [0x%08X] : %s0x%08X%s ",i,(unsigned int)p,ansi_estr[CONSOLE_COLOR_MAGENTA],(unsigned int)*p,ansi_estr[CONSOLE_COLOR_NORMAL]);
        }
        else
        {
            sprintf(command_buffer,"(%2i) [0x%08X] : .......... ",i,(unsigned int)p);
        }
        console_Print(command_buffer);
        p = p + ((i<8) ? (8*1024)/4 : (32*1024)/4);
        c++;
        if (c == zCOL)
        {
            console_Print("\n");
            c = 0;
        }
    }
    console_Print("\n");

}
static fsp_err_t ex_flash_b(uint32_t dest,uint32_t src,uint32_t cnt)
{
    /*
     *    copies a block into RAM and then programs it into flash
     *    f b dest source number of 32k blocks.
     */
    fsp_err_t err;
    flash_instance_t *f = ControlPanel.flash;
    for(uint32_t i=0;i<cnt;i++)
    {
        memcpy(block_buffer,(uint8_t*) src + (i*(32*1024)),(32*1024));
        err = f->p_api->write(f->p_ctrl,
                              (uint32_t) &block_buffer[0],
                              dest,
                              (cnt*32*1024));
        if (FSP_SUCCESS != err) return err;
    }
    return err;

}
#define x FLASH_CF_BLOCK_0
#endif //@@@
static fsp_err_t ex_flash_e(uint32_t block_address,uint32_t len)
{
#if 0 //@@@ this needs attention for P1
    volatile fsp_err_t err;
    flash_instance_t *f = ControlPanel.flash;
    err = f->p_api->open(f->p_ctrl,f->p_cfg);
    if (FSP_SUCCESS != err) return err;
    for(uint32_t i=0;i<len;i++)
    {

        err = f->p_api->erase(f->p_ctrl,block_address,len);
        if (FSP_SUCCESS != err) return err;
        block_address = block_address + (32*1024);
    }
    return err;
#endif //@@@
    return 0;
}
#if 0 //@@@  comment out for P1
RAMFUNC void flash_callback(flash_callback_args_t * p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if 0 // LPR
static int CP_mloop(char** args)
{
    ControlPanel.model_loops = strtol(args[1],NULL,10);
    ControlPanel.sync = 1;
    return 0;
}
static int CP_aiy(char** args)
{
    ControlPanel.ai_yield = strtol(args[1],NULL,10);
    return 0;
}
static int CP_aid(char** args)
{
    ControlPanel.ai_taskdelay = strtol(args[1],NULL,10);
    return 0;

}
static int CP_rf(char** args)
{

    sprintf(command_buffer,"%02X \n",ControlPanel.flags);
    console_Print(command_buffer);
    return 0;
}
static int CP_wf(char** args)
{
    ControlPanel.flags = (uint8_t)  strtol(args[1],NULL,16);
    return 0;
}
static int CP_dsy(char** args)
{
    ControlPanel.ds_yield = strtol(args[1],NULL,10);
    return 0;

}
static int CP_sts(char** args)
{
    sprintf(command_buffer,"GLCD  fps %d.%d\n",(ControlPanel.gfps/10),(ControlPanel.gfps%10));
    console_Print(command_buffer);
    sprintf(command_buffer,"VIDEO fps %d.%d\n",(ControlPanel.dfps/10),(ControlPanel.dfps%10));
    console_Print(command_buffer);
    sprintf(command_buffer,"CAM   fps %d.%d\n",(ControlPanel.cfps/10),(ControlPanel.cfps%10));
    console_Print(command_buffer);
    sprintf(command_buffer,"CAPT  fps %d.%d\n",(ControlPanel.vfps/10),(ControlPanel.vfps%10));
    console_Print(command_buffer);
    return 0;
}
static int CP_img(char** args)
{

    ControlPanel.camera_mux = strtol(args[1],NULL,16);
    return 0;
}


#endif


#if 0 // ADC_interleave
static int CP_adc(char** args)
{
    uint32_t idx = strtol(args[2],NULL,16);
    int cx = (strcmp(args[1],"st") == 0) ? 1 : 0;
    switch (cx) {
        case 1:
            ControlPanel.adc_ext_configuration.start_trigger_delay_table[idx] = (uint16_t) strtol(args[3],NULL,16);
            break;
    }
    ControlPanel.sync = 0x00000001; // update adc
    return 0;
}
#endif
#endif // CP commands

