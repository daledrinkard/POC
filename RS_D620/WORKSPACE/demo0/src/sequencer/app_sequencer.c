/*
            SEQUENCER app
*/
#include "application_common.h"
#include "cpan.h"
#include "app_sequencer.h"
#include "hal_data.h"
#include "power_module.h"
#include "r_gpt.h"
#include "r_ioport.h"       // IWYU pragma: keep
#include "POP/pop.h"
#include "simulator.h"
#include "comm_bus.h"
extern bsp_leds_t g_bsp_leds;
extern cpan_t *CP;
extern const power_rail_cfg_t power_analog_3300;
extern const power_rail_cfg_t power_analog_5000;
extern const power_rail_cfg_t power_analog_1200;
extern const power_rail_cfg_t power_digital_2400;
extern const power_rail_cfg_t power_digital_OFFLINE;
extern const power_controller_cfg_t power_controller_basic;
extern const power_rail_map_t power_rail_maps[PWR_MAX_RAILS];
//extern const power_rail_t power_rail_initial;
const cpan_t control_panel_initial = { 
        .stat = 0,
        .regs = {0},
             /* USER */
        .leds = &g_bsp_leds,  /* add the led structure */
        .p_SW1 = &g_SW1, /* add the SW1 external interrupt */
        .led_state = 0,
        .port_base =   {R_PORT1}, //@@@
        .port_shadow = {0},
        .port_enable = {0},
        .adc_value =   {0}
};

power_rail_cfg_t         rail_cfg_scratch[PWR_MAX_RAILS];
power_rail_ctrl_t        rail_ctrl_scratch[PWR_MAX_RAILS];
power_controller_ctrl_t  controller_ctrl_scratch;
extern const power_rail_t power_rails[PWR_MAX_RAILS];
power_controller_cfg_t controller_cfg_scratch;
extern const power_sequencer_CONST_t power_sequencer_CONST;

const power_controller_t PowerController = {    
    .rails =     (power_rail_t*)              &power_rails[0], 
    .cfg =       (power_controller_cfg_t *)   &controller_cfg_scratch,
    .cfg_store = (power_controller_cfg_t *)   DF_SEQUENCER_CONFIG_ADDR,
    .ctrl =      (power_controller_ctrl_t *)  &controller_ctrl_scratch,
    .map =       (power_rail_map_t *)         DF_POWER_RAIL_PINMAP_ADDR,
//@@@    .faults =    (power_fault_output_t *)     DF_POWER_RAIL_FLTMAP_ADDR,  /* moved to cfg */
   /*
        The following are const in Flash
   */
    .GPIO =      (power_sequencer_IO_t *)     &power_sequencer_IO,
    .CONSTANTS = (power_sequencer_CONST_t *)  &power_sequencer_CONST
};
int app_func_reset   (void)
{
   power_rail_t *p_rail;// = PowerController.rails;
    APP_INFO_PRINT("\nSEQUENCER RESET\n");
    POP0();


    #if 1 // only do this once, on first run, if the addresses of the dataflash segments change.
    APP_INFO_PRINT("\n ******* \n      UPDATING DATAFLASH  \n    ***** \n");
     pwr_rail_store_config(0,(uint8_t*) &power_analog_3300,sizeof(power_rail_cfg_t));
     pwr_rail_store_config(1,(uint8_t*) &power_analog_5000,sizeof(power_rail_cfg_t));
     pwr_rail_store_config(2,(uint8_t*) &power_analog_1200,sizeof(power_rail_cfg_t));
     pwr_rail_store_config(3,(uint8_t*) &power_digital_2400,sizeof(power_rail_cfg_t));
     pwr_rail_store_config(4,(uint8_t*) &power_digital_OFFLINE,sizeof(power_rail_cfg_t));
    for(int i=5;i<PWR_MAX_RAILS;i++)
    {
         pwr_rail_store_config(i,(uint8_t*)&power_digital_OFFLINE,sizeof(power_rail_cfg_t));
    }
    pwr_seq_store_config((uint8_t *)&power_controller_basic,sizeof(power_controller_cfg_t));
    pwr_seq_store_map((uint8_t *)&power_rail_maps,sizeof(power_rail_maps)); //@@@ hard constant
    #endif

    pwr_seq_restore_all(); /* cfg_store --> cfg */
    pwr_seq_configure();
    PowerController.ctrl->event = 0;
    PowerController.ctrl->rails_ready = 0;
    PowerController.ctrl->rails_enabled = 0;
    p_rail = PowerController.rails;
    for(int i=0;i<PWR_MAX_RAILS;i++) //@@@DWR code only handles one port at the moment
    {
        memset(p_rail->ctrl,0,sizeof(power_rail_ctrl_t));
        p_rail++;
    }
    PowerController.ctrl->state = PWR_SEQ_IDLE;
    POPB(); //@@@ turn on the blue led

    CP = CPAN_open(&control_panel_initial);  /* open the control panel */
    comm_init();
    R_PORT1->PCNTR3 = 0x00000000;
    R_PORT1->PCNTR4 = 0x00000000; 
    return (CP == NULL) ? -1 : 0;
}
int app_func_startup (void)
{
    APP_INFO_PRINT("\nSEQUENCER STARTUP\n");
/*  T0 sets the cadence for the system.  it is always running.*/
    R_GPT_Open  (&g_T0_ctrl, &g_T0_cfg);
    R_GPT_Enable(&g_T0_ctrl);
    R_GPT_Start (&g_T0_ctrl);
/*  SW1 starts the sequencer */    
    R_ICU_ExternalIrqOpen(&g_SW1_ctrl, &g_SW1_cfg);
    R_ICU_ExternalIrqEnable(&g_SW1_ctrl);    
    DROP0();
    return 0;
}

int app_func_restart (void)
{
    /* example: persist rail 0's configuration record to data flash on restart */
    //data_flash_write_record(BSP_FEATURE_FLASH_DATA_FLASH_START, (const uint8_t *) &power_rail_cfgs[0],
      //                       sizeof(power_rail_cfg_t));
    return 0;
}
/*

    The GPIO->pin[] are pointers to the pin's PFS register.  These set in the initial condition 

*/

int app_func_run     (void)
{
    /*
        The ADC has been scanned and values are stored in ???????
    */
   //power_rail_t *p_rail;// = PowerController.rails;
   //power_rail_map_t  *p_map;// = PowerController.map;
   bool loop_again = false;
   if (app_event_flag_get(SYSFLG_PWR_READBACK,APP_FLAG_OR_CLEAR,0,NULL))
   {
      PowerController.ctrl->event |= PWR_FLAG_READBACK;
   }
   if (PowerController.ctrl->event & PWR_FLAG_READBACK)
   {
        if (0 == comm_service())
        {
            PowerController.ctrl->event &= (uint32_t) ~PWR_FLAG_READBACK;
        }
   }
   switch(PowerController.ctrl->state)
   {    
        case PWR_SEQ_RESET:
            // Fall through to IDLE case
        case PWR_SEQ_IDLE:
            if (PowerController.ctrl->event & PWR_FLAG_ENABLE)
            {
                PowerController.ctrl->state = PWR_SEQ_RUN;
                PowerController.ctrl->rails_enabled = 0;
                DROPB();
            }
            break;
        case PWR_SEQ_RUN:
            loop_again = false;
            do {
            for(int i=0;i<PWR_MAX_RAILS;i++)
            {
                uint32_t GPI      = PowerController.ctrl->GPI;
                uint32_t Page     = PowerController.ctrl->Page;
                uint32_t GPI_on   = PowerController.rails[i].cfg->SEQ_config.GPI_seq_mask_on;
                uint32_t GPI_off  = PowerController.rails[i].cfg->SEQ_config.GPI_seq_mask_off;
                uint32_t Page_on  = PowerController.rails[i].cfg->SEQ_config.page_seq_on_dep_msk;
                uint32_t Page_off = PowerController.rails[i].cfg->SEQ_config.page_seq_off_dep_msk;
                uint8_t ID        = PowerController.rails[i].cfg->SEQ_config.ID;
                uint8_t conf      = PowerController.rails[i].cfg->SEQ_config.ID_other;
                power_rail_t *rail = &PowerController.rails[i];
                switch (PowerController.rails[i].ctrl->state) {
                    case PWR_RAIL_OFF:
                         if ( ((GPI & GPI_on) == GPI_on  ) && ((Page & Page_on) == Page_on) )
                         {
                             PowerController.rails[i].ctrl->state = PWR_RAIL_ON;
                             rail->ctrl->state = PWR_RAIL_ON;
                             PowerController.rails[i].ctrl->en_out = 1; //@@@ TIMER BABY
                         }
                         break;
                    case PWR_RAIL_ON:
                         if ( ((GPI & GPI_off) == GPI_off  ) && ((Page & Page_off) == Page_off) )
                         {
                             PowerController.rails[i].ctrl->state = PWR_RAIL_OFF; //@@@ not taking into account delays yet
                             PowerController.rails[i].ctrl->dis_out = 1; //@@@ TIMER BABY
                         }
                         break;
                    default: break;
                }
                if (PowerController.rails[i].ctrl->en_out)
                {
                    PowerController.ctrl->Page |= (1 << i);  /* lets everybody know you're enabled */
                    PowerController.rails[i].ctrl->en_out = 0;
                    if (ID > 0)
                    {
                        switch(conf) {
                            /* Active low */
                            case (2): /* Active */
                            case (3): /* open drain */
                               PWR_PIN_ASSERT_LOW(ID-1);
//                               *((uint32_t*) PowerController.GPIO->pin[ID-1]) |= (uint32_t)  0x00000004;  /* Set PDR bit */
//                               *((uint32_t*) PowerController.GPIO->pin[ID-1]) &= (uint32_t) ~0x00000001;  /* clear PODR bit */
                               break;
                            /* Active High */
                            case (6):
                                PWR_PIN_ASSERT_HI(ID-1);
//                               *((uint32_t*) PowerController.GPIO->pin[ID-1]) |= (uint32_t)  0x00000004;  /* Set PDR bit */
//                               *((uint32_t*) PowerController.GPIO->pin[ID-1]) |= (uint32_t) ~0x00000001;  /* clear PODR bit */
                               break;
                            case (7):
                                PWR_PIN_ASSERT_OPEN(ID-1);
//                               *((uint32_t*) PowerController.GPIO->pin[ID-1]) |= (uint32_t)  ~0x00000004;  /* Clr PDR bit (make it an input)*/
                               //@@@ what to do about the pullup resistor???
                               break;
                        }
                    }
                }
                if (PowerController.rails[i].ctrl->dis_out)
                {
                    PowerController.ctrl->Page &= (uint32_t) ~(1 << i);  /* lets everybody know you're not enabled */
                    PowerController.rails[i].ctrl->dis_out = 0;
                    if (ID > 0)
                    {
                        switch(conf) {
                            /* Active low */
                            case (2): /* Active */
                               PWR_PIN_ASSERT_HI(ID-1);
                               break;
                            case (3): /* open drain */
                               PWR_PIN_ASSERT_OPEN(ID-1);
                               break;
                            /* Active High */
                            case (6):
                                PWR_PIN_ASSERT_HI(ID-1);
                               break;
                            case (7):
                                PWR_PIN_ASSERT_LOW(ID-1);
                               //@@@ what to do about the pullup resistor???
                               break;
                        }
                    }
                }
                if (PowerController.rails[i].ctrl->en_out || PowerController.rails[i].ctrl->dis_out)
                {
                    PowerController.rails[i].ctrl->en_out = 0;
                    PowerController.rails[i].ctrl->dis_out = 0;
                    loop_again = true;
                }
            }
        }        while(loop_again);

            break;
        case PWR_SEQ_SEQUENCING_DOWN:
            break;
        case PWR_SEQ_FAULT_SHUTDOWN:
            break;
            default: break;
    }
    return 0;
}

/* once the ADCs are integrated, this will become the ADC callback */
void T0_cb(timer_callback_args_t *p_args)
{
    /* USER CODE: handle timer callback */
    POP5();
    pwr_mod_poll((uint16_t*) &ControlPanel.adc_value[0]); /* poll the ADC and update monitor_mv for each rail */
    pwr_seq_poll_GPI();
    pwr_seq_poll_Page();
    DROP5();
    app_event_flag_seti(SYSFLG_PWR_SERVICE,0);
}

/* Callback function */
void SW1_cb(external_irq_callback_args_t *p_args)
{
    /* TODO: add your own code here */
    if (PowerController.ctrl->event & PWR_FLAG_ENABLE)
    {
        PowerController.ctrl->event &= (uint32_t) ~PWR_FLAG_ENABLE;
    }
    else
    {
        PowerController.ctrl->event |= PWR_FLAG_ENABLE;
    }
}

/****  DATA FLASH MAP *****
0x0800_0000: power_sequencer_cfg_t
0x0800_0100: power_rail_0  cfg   (128 bytes)
0x0800_0180: power_rail_1  cfg   (128 bytes)
****************************/