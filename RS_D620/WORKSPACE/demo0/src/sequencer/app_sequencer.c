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
extern bsp_leds_t g_bsp_leds;
extern cpan_t *CP;
extern const power_rail_cfg_t power_analog_3300;
extern const power_rail_cfg_t power_analog_5000;
extern const power_rail_cfg_t power_analog_1200;
extern const power_rail_cfg_t power_digital_2400;
extern const power_rail_cfg_t power_digital_OFFLINE;
extern const power_controller_cfg_t power_controller_basic;
extern const power_rail_map_t power_rail_maps[8];
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
/*power_rail_cfg_t power_rail_cfg_array[PWR_MAX_RAILS] = {
    [0] = { .nominal_mv = 3300, .ov_threshold_mv = 3600, .uv_threshold_mv = 3000, .on_delay_ms = 10, .off_delay_ms = 10, .timeout_ms = 100, .dependency_mask = 0x00000000, .rail_group_mask = 0x01, .monitor_type = PWR_MON_ANALOG, .margin_capable = true, .margin_high_pct = 10, .margin_low_pct = -10, .enable_active_high = true },
    [1] = { .nominal_mv = 5000, .ov_threshold_mv = 5500, .uv_threshold_mv = 4500, .on_delay_ms = 20, .off_delay_ms = 20, .timeout_ms = 200, .dependency_mask = 0x00000001, .rail_group_mask = 0x02, .monitor_type = PWR_MON_DIGITAL, .margin_capable = false, .margin_high_pct = 0, .margin_low_pct = 0, .enable_active_high = false },
    [2] = { .nominal_mv = 12000, .ov_threshold_mv = 13000, .uv_threshold_mv = 11000, .on_delay_ms = 30, .off_delay_ms = 30, .timeout_ms = 300, .dependency_mask = 0x00000003, .rail_group_mask = 0x04, .monitor_type = PWR_MON_NONE, .margin_capable = false, .margin_high_pct = 0, .margin_low_pct = 0, .enable_active_high = true },
    [3] = { .nominal_mv = 18000, .ov_threshold_mv = 20000, .uv_threshold_mv = 16000, .on_delay_ms = 40, .off_delay_ms = 40, .timeout_ms = 400, .dependency_mask = 0x00000007, .rail_group_mask = 0x08, .monitor_type = PWR_MON_ANALOG, .margin_capable = true, .margin_high_pct = -5, .margin_low_pct = -15, .enable_active_high = false },
    [4] ... [PWR_MAX_RAILS-1] ... // Add more configurations as needed
};*/
power_rail_ctrl_t        rail_ctrl_scratch[8];
power_controller_ctrl_t  controller_ctrl_scratch;
extern const power_rail_t power_rails[8];
const power_controller_t PowerController = {    
    .rails = (power_rail_t*)            &power_rails[0], 
    .cfg = (power_controller_cfg_t *)   DF_SEQUENCER_CONFIG_ADDR,
    .ctrl = (power_controller_ctrl_t *) &controller_ctrl_scratch,
    .map = (power_rail_map_t *)         DF_POWER_RAIL_PINMAP_ADDR,
    .faults = (power_fault_output_t *)  DF_POWER_RAIL_FLTMAP_ADDR
};
int app_func_reset   (void)
{
    APP_INFO_PRINT("\nSEQUENCER RESET\n");
    POP0();


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
    pwr_seq_update_map((uint8_t *)&power_rail_maps,sizeof(power_rail_maps)); //@@@ hard constant
    #endif
    PowerController.ctrl->event = 0;


    CP = CPAN_open(&control_panel_initial);  /* open the control panel */
    R_SCI_UART_Open(&g_comm_uart_ctrl, &g_comm_uart_cfg); //@@@ should be in comm_init();
    R_PORT1->PCNTR3 = 0x00000000;
    R_PORT1->PCNTR4 = 0x00000000; 
    return (CP == NULL) ? -1 : 0;
}
int app_func_startup (void)
{
    APP_INFO_PRINT("\nSEQUENCER STARTUP\n");
    /**
     *   Start the timer to generate a periodic interrupt for the sequencer to poll the ADC and update the power rail states.
     *   The timer is configured to generate an interrupt every 100ms, which is suitable for the sequencer's needs. The timer callback function will handle the ADC polling and state updates.
     *   The timer is opened, enabled, and started. The callback function T0_cb will be called on each timer interrupt.
     *   The timer is configured in the RA configuration files (ra_gen/hal_data.h and ra_gen/hal_entry.c) with the appropriate settings for the desired period and callback function.
     *   The timer is a GPT (General Purpose Timer) instance, and the R_GPT_Open, R_GPT_Enable, and R_GPT_Start functions are used to control the timer.
     */
    R_GPT_Open  (&g_T0_ctrl, &g_T0_cfg);
    R_GPT_Enable(&g_T0_ctrl);
    R_GPT_Start (&g_T0_ctrl);
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
int app_func_run     (void)
{
    /*
        The ADC has been scanned and values are stored in ???????
    */
   power_rail_t *p_rail;// = PowerController.rails;
   power_rail_map_t  *p_map;// = PowerController.map;
   switch(PowerController.ctrl->state)
   {    
        case PWR_SEQ_RESET:
            PowerController.ctrl->event = 0;
            PowerController.ctrl->rails_ready = 0;
            PowerController.ctrl->rails_enabled = 0;
            p_rail = PowerController.rails;
            p_map = PowerController.map;
            for(int i=0;i<PWR_MAX_RAILS;i++) //@@@DWR code only handles one port at the moment
            {
                PowerController.ctrl->rails_enabled |= p_rail->cfg->enabled ? (1 << i) : 0;
                CP->port_enable[0] |= p_rail->cfg->enabled ? (1 << p_map->en_pin_bit) : 0; 
                p_rail->ctrl->state = PWR_RAIL_OFF;
                p_map++;    
            }
            PowerController.ctrl->state = PWR_SEQ_IDLE;
            POPB(); //@@@ turn on the blue led
            // Fall through to IDLE case
        case PWR_SEQ_IDLE:
            if (PowerController.ctrl->event & PWR_FLAG_ENABLE)
            {
                PowerController.ctrl->state = PWR_SEQ_SEQUENCING_UP;
                PowerController.ctrl->rails_enabled = 0;
                DROPB();
            }
            break;
        case PWR_SEQ_SEQUENCING_UP:
            if (PowerController.ctrl->rails_enabled == PowerController.ctrl->rails_ready)
            {
                PowerController.ctrl->state = PWR_SEQ_RUN;
            }
            break;
        case PWR_SEQ_RUN:
            CP->port_shadow[0] = 0;
            CP->port_enable[0] = 0;
            for(int i=0;i<PWR_MAX_RAILS;i++) //DWR code only handles one port at the moment //@@@ hard coded 8
            {
                /* USER code for running each rail */
//                CP->port_enable[0] |= p_rail->cfg->enabled ? (1 << p_map->en_pin_bit) : 0; //@@@note that the configured rails 
                                                                                           //@@@ should really only need set in the startup.
                switch(p_rail->ctrl->state)
                {
                    case PWR_RAIL_ON:
                    case PWR_RAIL_MARGIN_HIGH:
                    case PWR_RAIL_MARGIN_LOW:
                    case PWR_RAIL_SEQ_OFF:
                         CP->port_shadow[0] |= (1 << p_map->en_pin_bit);
                         break;
                    default:
                    break;
                }
                p_rail++;
                p_map++;    
            }
            TOG2();
            //@@@ this is where the outputs are updated as a port write.
            if (CP->port_last  != CP->port_shadow)
            {
                CP->port_base[0]->PCNTR1 = (CP->port_shadow[0] << 16) | (CP->port_enable[0]);
                CP->port_last[0] = CP->port_shadow[0];
            //pwr_mod_poll(NULL); /* poll the ADC and update monitor_mv for each rail */
            }
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
    POP5();
    pwr_mod_poll((uint16_t*) &ControlPanel.adc_value[0]); /* poll the ADC and update monitor_mv for each rail */
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