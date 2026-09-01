/*
            SEQUENCER app
*/
#include "cpan.h"
#include "app_sequencer.h"
#include "hal_data.h"
#include "power_module.h"
#include "r_gpt.h"
#include "r_ioport.h"       // IWYU pragma: keep
#include "POP/pop.h"
extern bsp_leds_t g_bsp_leds;
extern cpan_t *CP;












extern const power_rail_cfg_t power_analog_3300;
extern const power_rail_cfg_t power_analog_5000;
extern const power_rail_cfg_t power_analog_1200;
extern const power_rail_cfg_t power_digital_2400;
extern const power_rail_cfg_t power_digital_OFFLINE;
extern const power_controller_cfg_t power_controller_basic;




const cpan_t control_panel_initial = { 
        .stat = 0,
        .regs = {0},
             /* USER */
        .leds = &g_bsp_leds,  /* add the led structure */
        .led_state = 0,
        .port_base = {R_PORT1}, //@@@
        .port_shadow = {0}

};


/*power_rail_cfg_t power_rail_cfg_array[PWR_MAX_RAILS] = {
    [0] = { .nominal_mv = 3300, .ov_threshold_mv = 3600, .uv_threshold_mv = 3000, .on_delay_ms = 10, .off_delay_ms = 10, .timeout_ms = 100, .dependency_mask = 0x00000000, .rail_group_mask = 0x01, .monitor_type = PWR_MON_ANALOG, .margin_capable = true, .margin_high_pct = 10, .margin_low_pct = -10, .enable_active_high = true },
    [1] = { .nominal_mv = 5000, .ov_threshold_mv = 5500, .uv_threshold_mv = 4500, .on_delay_ms = 20, .off_delay_ms = 20, .timeout_ms = 200, .dependency_mask = 0x00000001, .rail_group_mask = 0x02, .monitor_type = PWR_MON_DIGITAL, .margin_capable = false, .margin_high_pct = 0, .margin_low_pct = 0, .enable_active_high = false },
    [2] = { .nominal_mv = 12000, .ov_threshold_mv = 13000, .uv_threshold_mv = 11000, .on_delay_ms = 30, .off_delay_ms = 30, .timeout_ms = 300, .dependency_mask = 0x00000003, .rail_group_mask = 0x04, .monitor_type = PWR_MON_NONE, .margin_capable = false, .margin_high_pct = 0, .margin_low_pct = 0, .enable_active_high = true },
    [3] = { .nominal_mv = 18000, .ov_threshold_mv = 20000, .uv_threshold_mv = 16000, .on_delay_ms = 40, .off_delay_ms = 40, .timeout_ms = 400, .dependency_mask = 0x00000007, .rail_group_mask = 0x08, .monitor_type = PWR_MON_ANALOG, .margin_capable = true, .margin_high_pct = -5, .margin_low_pct = -15, .enable_active_high = false },
    [4] ... [PWR_MAX_RAILS-1] ... // Add more configurations as needed
};*/






const power_rail_t power_rail_initial = {
    .cfg = NULL, /* will be initialized in app_func_reset() */
    .state = PWR_RAIL_OFF,
    .last_fault = PWR_FAULT_NONE,
    .monitor_mv = 0,
    .cnt = 0,
    .adc_raw_lo = 0,
    .adc_raw_hi = 0,
    .adc_convert = 0
};
power_rail_t rail_scratch;
power_rail_cfg_t rail_cfg_scratch;

const power_controller_t PowerController_initial = {    
    .rails = NULL, /* will be initialized in app_func_reset() */
    .num_rails = PWR_MAX_RAILS,
    .state = PWR_SEQ_IDLE,
    .fault_log = {0},
    .cfg = (power_controller_cfg_t *) DF_POWER_RAIL_CONFIG_ADDR
};
power_controller_t PowerController;
int app_func_reset   (void)
{
    APP_INFO_PRINT("\nSEQUENCER RESET\n");
    POP0();
    int r = sizeof(power_controller_cfg_t);
    #if 1 // only do this once, on first run, to initialize the data flash with default values
     pwr_mod_update_config(0,(uint8_t*) &power_analog_3300,sizeof(power_rail_cfg_t));
     pwr_mod_update_config(1,(uint8_t*) &power_analog_5000,sizeof(power_rail_cfg_t));
     pwr_mod_update_config(2,(uint8_t*) &power_analog_1200,sizeof(power_rail_cfg_t));
     pwr_mod_update_config(3,(uint8_t*) &power_digital_2400,sizeof(power_rail_cfg_t));
     pwr_mod_update_config(4,(uint8_t*) &power_digital_OFFLINE,sizeof(power_rail_cfg_t));
    for(int i=5;i<PWR_MAX_RAILS;i++)
    {
         pwr_mod_update_config(i,(uint8_t*)&power_digital_OFFLINE,sizeof(power_rail_cfg_t));
    }
    pwr_seq_update_config((uint8_t *)&power_controller_basic,sizeof(power_controller_cfg_t));
    #endif
    memcpy(&PowerController, &PowerController_initial, sizeof(PowerController));
    for(int i=0;i<PowerController.num_rails;i++)
    {
        PowerController.rails[i].cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (DF_POWER_RAIL_RECORD_SIZE * i));
    }
    CP = CPAN_open(&control_panel_initial);  /* open the control panel */
    R_PORT1->PCNTR3 = 0x00000000;
    R_PORT1->PCNTR4 = 0x00000000; 
    R_GPT_Open(&g_T0_ctrl, &g_T0_cfg);
    R_GPT_Enable(&g_T0_ctrl);
    R_GPT_Start(&g_T0_ctrl);
    DROP0();
    return (CP == NULL) ? -1 : 0;
}
int app_func_startup (void)
{
    APP_INFO_PRINT("\nSEQUENCER STARTUP\n");
    return 0;
}

int app_func_restart (void)
{
    /* example: persist rail 0's configuration record to data flash on restart */
    //data_flash_write_record(BSP_FEATURE_FLASH_DATA_FLASH_START, (const uint8_t *) &power_rail_cfgs[0],
      //                       sizeof(power_rail_cfg_t));
    return 0;
}
int app_func_run     (void)
{
    /*
        The ADC has been scanned and values are stored in ???????
    */
   switch(PowerController.state)
   {    
        case PWR_SEQ_IDLE:
        PowerController.state = PWR_SEQ_SEQUENCING_UP;
            break;
        case PWR_SEQ_SEQUENCING_UP: // fall thru
        case PWR_SEQ_RUN:
            CP->port_shadow[0] = 0;
            CP->port_enable[0] = 0;
        for(int i=0;i<PowerController.num_rails;i++) //DWR code only handles one port at the moment
        {
            /* USER code for running each rail */
            CP->port_enable[0] |= PowerController.rails[i].cfg->enabled ? (1 << PowerController.rails[i].cfg->en_pin_bit) : 0;
            switch(PowerController.rails[i].state)
            {
                case PWR_RAIL_ON:
                case PWR_RAIL_MARGIN_HIGH:
                case PWR_RAIL_MARGIN_LOW:
                case PWR_RAIL_SEQ_OFF:
                     CP->port_shadow[0] |= (1 << PowerController.rails[i].cfg->en_pin_bit) & CP->port_enable[0];
                    break;
            }
        }
        TOG2();
        //@@@ this is where the outputs are updated as a port write.
        CP->port_base[0]->PCNTR1 = CP->port_shadow[0];
        //pwr_mod_poll(NULL); /* poll the ADC and update monitor_mv for each rail */
            break;
        case PWR_SEQ_SEQUENCING_DOWN:
            break;
        case PWR_SEQ_FAULT_SHUTDOWN:
            break;
    }
    return 0;
}

void T0_cb(timer_callback_args_t *p_args)
{
    /* USER CODE: handle timer callback */
    pwr_mod_poll(NULL); /* poll the ADC and update monitor_mv for each rail */
    app_event_flag_seti(SYSFLG_PWR_SERVICE,0);
}
/****  DATA FLASH MAP *****
0x0800_0000: power_sequencer_cfg_t
0x0800_0100: power_rail_0  cfg
0x0800_0080: power_rail_1  cfg
****************************/