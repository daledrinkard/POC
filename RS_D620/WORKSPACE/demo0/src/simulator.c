/*
            SIMULATOR
*/
#include<string.h>
#include "simulator.h"
#include "sequencer/CPAN/cpan.h"

/* USER */
extern const simulator_t simulator_initial[2];
simulator_t Simulator[2];


int simulator_init(void)
{
    memcpy((simulator_t*) &Simulator,(simulator_t*) &simulator_initial,sizeof(simulator_t)*2);
    return 0;
}
void simulator_start(int i)
{
    Simulator[i].state = 1;
}
int simulator_run(void)
{
    for(int i=0;i<2;i++)
    {
        simulator_t *sim = &Simulator[i];
        srt_t *srt = sim->p_srt;
        switch(sim->state)
        {
            case 0: /* wait for start */
            break;
            case 1:
                if (srt->op == SIMULATOR_OP_END)
                {
                    sim->state = 0xFF; /* end of script */
                }
                else
                {
                    sim->cnt = srt->delay;
                    sim->div_cnt = srt->div;
                    sim->state = 2;
                }
                break;
            case 2:
                    switch(srt->op)
                    {
                        case SIMULATOR_OP_WAIT:
                            break;
                        case SIMULATOR_OP_RAISE:
                            if((ControlPanel.adc_value[sim->chan] + srt->step) < 0xFFFF)
                                ControlPanel.adc_value[sim->chan] += srt->step;
                            break;
                        case SIMULATOR_OP_LOWER:
                            if((ControlPanel.adc_value[sim->chan] - srt->step) > 0)
                                ControlPanel.adc_value[sim->chan] -= srt->step;
                            break;
                        case SIMULATOR_OP_FORCE:
                            ControlPanel.adc_value[sim->chan] = srt->step;
                            break;
                        case SIMULATOR_OP_REPEAT:
                            sim->p_srt = sim->srt;
                            sim->state = 1;
                            break;
                        default:
                            break;
                    }
                if (sim->div_cnt == 0)
                {
                    sim->div_cnt = srt->div;
                    if (sim->cnt == 0)
                    {
                       sim->p_srt++;
                       sim->state = 0; /* go to next instruction */
                    }
                    else
                    {
                        sim->cnt--;
                    }
                }
                else
                {
                    sim->div_cnt--;
                }
                break;
            default:
                break;
        }
    }
    return 0;
}
