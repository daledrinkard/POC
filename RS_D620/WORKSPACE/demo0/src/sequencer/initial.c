#include "application_common.h"
#include "power_module.h"
#include "ra/fsp/src/bsp/mcu/all/bsp_io.h"
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
*/
const power_controller_cfg_t power_controller_basic = {
    .monitor = {
        (PWR_MON_TYPE_ANALOG  | 0), /* Tie MON[0] to the voltage monitoring of rail[0]*/
        (PWR_MON_TYPE_CURRENT | 0), /* MON[1] to the current monitoring of rail[0]*/  
        (PWR_MON_TYPE_ANALOG  | 1), /* MON[2] to the voltage monitoring of rail[1]*/  
        (PWR_MON_TYPE_ANALOG  | 2), /* MON[3] to the voltage monitoring of rail[0]*/  
        (PWR_MON_TYPE_DIGITAL | 3), /* physically connected to SW2 on EK boards */
        (PWR_MON_TYPE_NONE),        /* physically connected to SW1 on EK boards */
        (PWR_MON_TYPE_NONE),        /* BLUE LED */
        (PWR_MON_TYPE_NONE),        /* GREEN LED */
        (PWR_MON_TYPE_NONE),        /* RED LED */
        (PWR_MON_TYPE_ANALOG  | 4), /* MON[9]*/
        (PWR_MON_TYPE_CURRENT | 4), /* MON[10]*/
        0xff,/* MON[11]*/
        0xff,/* MON[12]*/
        0xff,/* MON[13]*/
        0xff,/* MON[14]*/
        0xff,/* MON[15]*/
        0xff,/* MON[16]*/
        0xff,/* MON[17]*/
        0xff,/* MON[18]*/
        0xff,/* MON[19]*/
        0xff,/* MON[20]*/
        0xff,/* MON[21]*/
        0xff,/* MON[22]*/
        0xff,/* MON[23]*/
        0xff,/* MON[24]*/
        0xff,/* MON[25]*/
        0xff,/* MON[26]*/
        0xff,/* MON[27]*/
        0xff,/* MON[28]*/
        0xff,/* MON[29]*/
        0xff, /* MON[30]*/
        0xff /* MON[31]*/
    },
    .GPI_config = {
        .GPI = {
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[0]*/
            { .id = 1,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[1]*/
            { .id = 2,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[2]*/
            { .id = 3,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[3]*/
            { .id = 4,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[4]*/
            { .id = 5,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[5]*/
            { .id = 6,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[6]*/
            { .id = 7,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[7]*/
            { .id = 8,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[8]*/
            { .id = 9,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[9]*/
            { .id = 10, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[10]*/
            { .id = 11, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[11]*/
            { .id = 12, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[12]*/
            { .id = 13, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[13]*/
            { .id = 14, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[14]*/
            { .id = 15, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[15]*/
            { .id = 16, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[16]*/
            { .id = 17, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[17]*/
            { .id = 18, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[18]*/
            { .id = 19, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[19]*/
            { .id = 20, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[20]*/
            { .id = 21, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[21]*/
            { .id = 22, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[22]*/
            { .id = 23, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[23]*/
            { .id = 24, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[24]*/
            { .id = 25, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[25]*/
            { .id = 26, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[26]*/
            { .id = 27, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[27]*/
            { .id = 28, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[28]*/
            { .id = 29, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[29]*/
            { .id = 30, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[30]*/
            { .id = 31, .conf = PWR_MON_GPI_MODE_UNUSED  } /* GPI[31]*/
        },
        .fault_en = 0x00000000,
        .LSCP = 0x00,
        .MRG_EN = 0x00,
        .MRG_LOW = 0x00,
        .rsv1 = 0xff,
        .debug_pin = 0x00
    },
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
const power_rail_map_t power_rail_maps[PWR_MAX_RAILS] = {
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
    },
    /* rails 8-31: not wired on this POC board yet - placeholder mapping,
       continuing the en_pin_bit/ADC0_index/ADC1_index = i pattern from
       rails 4-7 until real pin assignments are known */
    { .en_port = 0, .en_pin_bit = 8,  .ADC0_index = 8,  .ADC1_index = 8  },
    { .en_port = 0, .en_pin_bit = 9,  .ADC0_index = 9,  .ADC1_index = 9  },
    { .en_port = 0, .en_pin_bit = 10, .ADC0_index = 10, .ADC1_index = 10 },
    { .en_port = 0, .en_pin_bit = 11, .ADC0_index = 11, .ADC1_index = 11 },
    { .en_port = 0, .en_pin_bit = 12, .ADC0_index = 12, .ADC1_index = 12 },
    { .en_port = 0, .en_pin_bit = 13, .ADC0_index = 13, .ADC1_index = 13 },
    { .en_port = 0, .en_pin_bit = 14, .ADC0_index = 14, .ADC1_index = 14 },
    { .en_port = 0, .en_pin_bit = 15, .ADC0_index = 15, .ADC1_index = 15 },
    { .en_port = 0, .en_pin_bit = 16, .ADC0_index = 16, .ADC1_index = 16 },
    { .en_port = 0, .en_pin_bit = 17, .ADC0_index = 17, .ADC1_index = 17 },
    { .en_port = 0, .en_pin_bit = 18, .ADC0_index = 18, .ADC1_index = 18 },
    { .en_port = 0, .en_pin_bit = 19, .ADC0_index = 19, .ADC1_index = 19 },
    { .en_port = 0, .en_pin_bit = 20, .ADC0_index = 20, .ADC1_index = 20 },
    { .en_port = 0, .en_pin_bit = 21, .ADC0_index = 21, .ADC1_index = 21 },
    { .en_port = 0, .en_pin_bit = 22, .ADC0_index = 22, .ADC1_index = 22 },
    { .en_port = 0, .en_pin_bit = 23, .ADC0_index = 23, .ADC1_index = 23 },
    { .en_port = 0, .en_pin_bit = 24, .ADC0_index = 24, .ADC1_index = 24 },
    { .en_port = 0, .en_pin_bit = 25, .ADC0_index = 25, .ADC1_index = 25 },
    { .en_port = 0, .en_pin_bit = 26, .ADC0_index = 26, .ADC1_index = 26 },
    { .en_port = 0, .en_pin_bit = 27, .ADC0_index = 27, .ADC1_index = 27 },
    { .en_port = 0, .en_pin_bit = 28, .ADC0_index = 28, .ADC1_index = 28 },
    { .en_port = 0, .en_pin_bit = 29, .ADC0_index = 29, .ADC1_index = 29 },
    { .en_port = 0, .en_pin_bit = 30, .ADC0_index = 30, .ADC1_index = 30 },
    { .en_port = 0, .en_pin_bit = 31, .ADC0_index = 31, .ADC1_index = 31 }
};



/*
  _____                           _____       _ _     
 |  __ \                         |  __ \     (_) |    
 | |__) |____      _____ _ __    | |__) |__ _ _| |___ 
 |  ___/ _ \ \ /\ / / _ \ '__|   |  _  // _` | | / __|
 | |  | (_) \ V  V /  __/ |      | | \ \ (_| | | \__ \
 |_|   \___/ \_/\_/ \___|_|      |_|  \_\__,_|_|_|___/
                                                      
                                                      
                                                                     
                                                                     */
extern power_rail_cfg_t  rail_cfg_scratch[PWR_MAX_RAILS];
extern power_rail_ctrl_t rail_ctrl_scratch[PWR_MAX_RAILS];

/* one power_rail_t initializer for rail index n: cfg_store points at that
   rail's DF_POWER_RAIL_CONFIG_ADDR slot in dataflash; cfg/ctrl point at
   rail n's slot in the matching SRAM scratch arrays */
#define RAIL_ENTRY(n) { \
    .cfg_store = (power_rail_cfg_t *) (DF_POWER_RAIL_CONFIG_ADDR + ((n) * DF_POWER_RAIL_RECORD_SIZE)), \
    .cfg       = (power_rail_cfg_t *) &rail_cfg_scratch[n], \
    .ctrl      = (power_rail_ctrl_t *) &rail_ctrl_scratch[n] }

const power_rail_t power_rails[PWR_MAX_RAILS] = {
    RAIL_ENTRY(0),  RAIL_ENTRY(1),  RAIL_ENTRY(2),  RAIL_ENTRY(3),
    RAIL_ENTRY(4),  RAIL_ENTRY(5),  RAIL_ENTRY(6),  RAIL_ENTRY(7),
#if PWR_MAX_RAILS > 8    //@@@ for debug and development
    RAIL_ENTRY(8),  RAIL_ENTRY(9),  RAIL_ENTRY(10), RAIL_ENTRY(11),
    RAIL_ENTRY(12), RAIL_ENTRY(13), RAIL_ENTRY(14), RAIL_ENTRY(15),
    RAIL_ENTRY(16), RAIL_ENTRY(17), RAIL_ENTRY(18), RAIL_ENTRY(19),
    RAIL_ENTRY(20), RAIL_ENTRY(21), RAIL_ENTRY(22), RAIL_ENTRY(23),
    RAIL_ENTRY(24), RAIL_ENTRY(25), RAIL_ENTRY(26), RAIL_ENTRY(27),
    RAIL_ENTRY(28), RAIL_ENTRY(29), RAIL_ENTRY(30), RAIL_ENTRY(31)
#endif
};
#undef RAIL_ENTRY
//@@@ this stuff below belongs in its own domain
/*
    This is an initialization structure for the POC based on the EK which has pins
    that cannot be used due to other stuff...

*/
const power_sequencer_IO_t power_sequencer_IO = {
.MON = {
BSP_IO_PORT_00_PIN_00,
BSP_IO_PORT_00_PIN_01,
BSP_IO_PORT_00_PIN_02,
BSP_IO_PORT_00_PIN_03,
BSP_IO_PORT_00_PIN_04,
BSP_IO_PORT_00_PIN_05,
BSP_IO_PORT_00_PIN_06,
BSP_IO_PORT_00_PIN_07,
BSP_IO_PORT_00_PIN_08,
BSP_IO_PORT_00_PIN_09,
BSP_IO_PORT_00_PIN_10,
BSP_IO_PORT_00_PIN_14,
BSP_IO_PORT_00_PIN_15,
BSP_IO_PORT_05_PIN_00,  /* used in USB on EK */
BSP_IO_PORT_05_PIN_01,  /* used in USB on EK */
BSP_IO_PORT_05_PIN_02,
BSP_IO_PORT_05_PIN_03,
BSP_IO_PORT_05_PIN_04,
BSP_IO_PORT_05_PIN_05,  /* this pin is pulled high with a 1k5 */
BSP_IO_PORT_05_PIN_06,  /* this pin is pulled high with a 1k5 */
BSP_IO_PORT_05_PIN_07,
BSP_IO_PORT_05_PIN_08,
BSP_IO_PORT_08_PIN_00,
BSP_IO_PORT_08_PIN_01,
BSP_IO_PORT_08_PIN_02,
BSP_IO_PORT_08_PIN_03,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF },
.DMON = {
BSP_IO_PORT_02_PIN_06, /* IRQ0 */ /* this has a 10k pull up on the board */
BSP_IO_PORT_02_PIN_05, /* IRQ1 */ /* this has a 10k pullup */
BSP_IO_PORT_02_PIN_03, /* IRQ2 */
BSP_IO_PORT_02_PIN_02, /* IRQ3 */
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF },
.EN = {  /* Ones with pullups route to OSPI */
BSP_IO_PORT_01_PIN_00,  /* IRQ2*/  /* this has a 10k pull up on the board */
BSP_IO_PORT_01_PIN_01,  /* IRQ1*/  /* this has a 10k pull up on the board */
BSP_IO_PORT_01_PIN_02,  /*     */  /* this has a 10k pull up on the board */
BSP_IO_PORT_01_PIN_03,  /*     */  /* this has a 10k pull up on the board */
BSP_IO_PORT_01_PIN_04,  /* IRQ1*/  /* this has a 10k pull up on the board */
BSP_IO_PORT_01_PIN_05,  /* IRQ0*/  /* this has a 10k pull up on the board */
BSP_IO_PORT_01_PIN_06,  /*     */  /* this has a 10k pull up on the board */
BSP_IO_PORT_01_PIN_07,  /*     */  /* this has a 10k pull up on the board */
0xFFFF, /* BSP_IO_PORT_01_PIN_08*/ /*     */  /* DEBUG SWDIO */
0xFFFF, /* BSP_IO_PORT_01_PIN_09*/ /*     */ /* DEBUG TDO */
0xFFFF, /* BSP_IO_PORT_01_PIN_10*//* IRQ3*/  /* DEBUG TDI */
BSP_IO_PORT_01_PIN_11, /* IRQ4*/ 
BSP_IO_PORT_01_PIN_12, /*     */ 
BSP_IO_PORT_01_PIN_13, /*     */ 
BSP_IO_PORT_01_PIN_14, /*     */ 
BSP_IO_PORT_01_PIN_15, /*     */ 
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
0xFFFF,
} 





};

