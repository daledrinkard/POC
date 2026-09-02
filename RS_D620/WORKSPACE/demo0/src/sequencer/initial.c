#include "application_common.h"
#include "power_module.h"
/*_____       _ _    _____             __ _                       _   _                 
 |  __ \     (_) |  / ____|           / _(_)                     | | (_)                
 | |__) |__ _ _| | | |     ___  _ __ | |_ _  __ _ _   _ _ __ __ _| |_ _  ___  _ __  ___ 
 |  _  // _` | | | | |    / _ \| '_ \|  _| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \/ __|
 | | \ \ (_| | | | | |___| (_) | | | | | | | (_| | |_| | | | (_| | |_| | (_) | | | \__ \
 |_|  \_\__,_|_|_|  \_____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|___/
                                             __/ |                                      
                                            |___/                                       
 These are used to initialize the dataflash
                                            */
const power_rail_cfg_t power_analog_3300 = {
    .nominal_mv = 3300,
    .ov_threshold_mv = 3400,
    .uv_threshold_mv = 3000,
    .nominal_c = 1000,
    .oc_threshold = 1100,
    .uc_threshold = 900,
    .on_delay_ms = 5,
    .off_delay_ms = 5,
    .timeout_ms = 10,
    .dependency_mask = 0,
	.enabled = true,
    .rail_group_mask = 0,
    .monitor_type = PWR_MON_ANALOG,
    .margin_capable = false,
    .margin_high_pct = 0,
    .margin_low_pct = 0,
    .enable_active_high = true
};
const power_rail_cfg_t power_analog_5000 = {
    .nominal_mv = 5000,
    .ov_threshold_mv = 5100,
    .uv_threshold_mv = 4000,
    .nominal_c = 1000,
    .oc_threshold = 1100,
    .uc_threshold = 900,
    .on_delay_ms = 10,
    .off_delay_ms = 20,
    .timeout_ms = 100,
    .dependency_mask = 0,
	.enabled = true,
    .rail_group_mask = 0,
    .monitor_type = PWR_MON_ANALOG,
    .margin_capable = false,
    .margin_high_pct = 0,
    .margin_low_pct = 0,
    .enable_active_high = true
};
const power_rail_cfg_t power_analog_1200 = {
    .nominal_mv = 12000,
    .ov_threshold_mv = 12010,
    .uv_threshold_mv = 11890,
    .nominal_c = 1000,
    .oc_threshold = 1100,
    .uc_threshold = 900,
    .on_delay_ms = 2,
    .off_delay_ms = 30,
    .timeout_ms = 10,
    .dependency_mask = 0,
	.enabled = true,
    .rail_group_mask = 0,
    .monitor_type = PWR_MON_ANALOG,
    .margin_capable = false,
    .margin_high_pct = 0,
    .margin_low_pct = 0,
    .enable_active_high = true
};
const power_rail_cfg_t power_digital_2400 = {
    .nominal_mv = 2400,
    .ov_threshold_mv = 2405,
    .uv_threshold_mv = 2395,
    .on_delay_ms = 60,
    .off_delay_ms = 0,
    .timeout_ms = 0,
    .dependency_mask = 0,
	.enabled = true,
    .rail_group_mask = 0,
    .monitor_type = PWR_MON_DIGITAL,
    .margin_capable = false,
    .margin_high_pct = 0,
    .margin_low_pct = 0,
    .enable_active_high = true
};
const power_rail_cfg_t power_digital_OFFLINE = {
    .nominal_mv = 2400,
    .ov_threshold_mv = 2405,
    .uv_threshold_mv = 2395,
    .nominal_c = 1000,
    .oc_threshold = 1100,
    .uc_threshold = 900,
    .on_delay_ms = 60,
    .off_delay_ms = 0,
    .timeout_ms = 0,
    .dependency_mask = 0,
	.enabled = false,
    .rail_group_mask = 0,
    .monitor_type = PWR_MON_NONE,
    .margin_capable = false,
    .margin_high_pct = 0,
    .margin_low_pct = 0,
    .enable_active_high = true
};
/*
   _____                                              _____             __ _                       _   _             
  / ____|                                            / ____|           / _(_)                     | | (_)            
 | (___   ___  __ _ _   _  ___ _ __   ___ ___ _ __  | |     ___  _ __ | |_ _  __ _ _   _ _ __ __ _| |_ _  ___  _ __  
  \___ \ / _ \/ _` | | | |/ _ \ '_ \ / __/ _ \ '__| | |    / _ \| '_ \|  _| |/ _` | | | | '__/ _` | __| |/ _ \| '_ \ 
  ____) |  __/ (_| | |_| |  __/ | | | (_|  __/ |    | |___| (_) | | | | | | | (_| | |_| | | | (_| | |_| | (_) | | | |
 |_____/ \___|\__, |\__,_|\___|_| |_|\___\___|_|     \_____\___/|_| |_|_| |_|\__, |\__,_|_|  \__,_|\__|_|\___/|_| |_|
                 | |                                                          __/ |                                  
                 |_|                                                         |___/                                   
                 
                 This structure is in dataflash    */
const power_controller_cfg_t power_controller_basic = {
    .watchdog_enabled = false,
    .watchdog_timeout_ms = 0,
    .active_rail_group = 0,
    .gpi_mask = 0,
    .gpo_mask = 0,
    .pmbus_address = 0,
    .cascade_id = 0,
    .cascade_count = 1,
    .sync_clock_enabled = false,
    .fault_pin_asserted = false
};

/* 
  _____       _ _   __  __                   _             
 |  __ \     (_) | |  \/  |                 (_)            
 | |__) |__ _ _| | | \  / | __ _ _ __  _ __  _ _ __   __ _ 
 |  _  // _` | | | | |\/| |/ _` | '_ \| '_ \| | '_ \ / _` |
 | | \ \ (_| | | | | |  | | (_| | |_) | |_) | | | | | (_| |
 |_|  \_\__,_|_|_| |_|  |_|\__,_| .__/| .__/|_|_| |_|\__, |
                                | |   | |             __/ |
                                |_|   |_|            |___/ 
this structure is in dataflash*/
const power_rail_map_t power_rail_maps[8] = {
    {
        .en_port =    1,
        .en_pin_bit = 11,
        .ADC0_index = 0,
        .ADC1_index = 1
    },
    {
        .en_port =    1,
        .en_pin_bit = 12,
        .ADC0_index = 2,
        .ADC1_index = 3
    },
    {
        .en_port =    1,
        .en_pin_bit = 15,
        .ADC0_index = 0,
        .ADC1_index = 0
    },
    {
        .en_port = 0,
        .en_pin_bit = 3,
        .ADC0_index = 3,
        .ADC1_index = 3
    },
    {
        .en_port = 0,
        .en_pin_bit = 4,
        .ADC0_index = 4,
        .ADC1_index = 4
    },
    {
        .en_port = 0,
        .en_pin_bit = 5,
        .ADC0_index = 5,
        .ADC1_index = 5
    },
    {
        .en_port = 0,
        .en_pin_bit = 6,
        .ADC0_index = 6,
        .ADC1_index = 6
    },
    {
        .en_port = 0,
        .en_pin_bit = 7,
        .ADC0_index = 7,
        .ADC1_index = 7
    }
};



/*
  _____                           _____       _ _     
 |  __ \                         |  __ \     (_) |    
 | |__) |____      _____ _ __    | |__) |__ _ _| |___ 
 |  ___/ _ \ \ /\ / / _ \ '__|   |  _  // _` | | / __|
 | |  | (_) \ V  V /  __/ |      | | \ \ (_| | | \__ \
 |_|   \___/ \_/\_/ \___|_|      |_|  \_\__,_|_|_|___/
                                                      
                                                      
                                                                     
                                                                     */
extern power_rail_ctrl_t rail_ctrl_scratch[8];
const power_rail_t power_rails[8] = {
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (0 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[0]
    },
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (1 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[1]
    },
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (2 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[2]
    },
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (3 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[3]         
    },
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (4 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[4]
    },
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (5 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[5]
    },
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (6 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[6]
    },
    {
    .cfg = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + (7 * DF_POWER_RAIL_RECORD_SIZE)), /* will be initialized in app_func_reset() */
    .ctrl = (power_rail_ctrl_t *) &rail_ctrl_scratch[7]
    }
};
