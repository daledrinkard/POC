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
    .SEQ_config = {
        .pad1 = 0x55AA,
        .ID = 1,
        .ID_other = 0,
        .GPI_seq_mask_on  = 0x00000001,
        .GPI_seq_mask_off = 0x00000002,
        .seq_timeout_cfg = 0x05,
        .seq_on_timeout = 10,
        .seq_off_timeout = 15,
        .pad2 = 0x99,
        .page_seq_on_dep_msk = 0x00000003,
        .page_seq_off_dep_msk = 0x00000003,
        .fault_slave_mask = 0xFFFFFFFF,
        .GPO_seq_on_dep_msk = 0x0000000F,
        .GPO_seq_off_dep_msk = 0x0000000F
    },
    .operation = 0x80,
    .spare = {0,0,0}
    /*,
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
    .enable_active_high = true */
};
const power_rail_cfg_t power_analog_5000 = {
    .SEQ_config = {
        .pad1 = 0x55AA,
        .ID = 2,
        .ID_other = 0,
        .GPI_seq_mask_on  = 0x00000001,
        .GPI_seq_mask_off = 0x00000002,
        .seq_timeout_cfg = 0x05,
        .seq_on_timeout = 10,
        .seq_off_timeout = 15,
        .pad2 = 0x99,
        .page_seq_on_dep_msk = 0x00000003,
        .page_seq_off_dep_msk = 0x00000003,
        .fault_slave_mask = 0xFFFFFFFF,
        .GPO_seq_on_dep_msk = 0x0000000F,
        .GPO_seq_off_dep_msk = 0x0000000F
    },
    .operation = 0x80,
    .spare = {0,0,0}
    /*
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
    .enable_active_high = true */
};
const power_rail_cfg_t power_analog_1200 = {
    .SEQ_config = {
        .pad1 = 0x55AA,
        .ID = 3,
        .ID_other = 0,
        .GPI_seq_mask_on  = 0x00000001,
        .GPI_seq_mask_off = 0x00000002,
        .seq_timeout_cfg = 0x05,
        .seq_on_timeout = 10,
        .seq_off_timeout = 15,
        .pad2 = 0x99,
        .page_seq_on_dep_msk = 0x00000003,
        .page_seq_off_dep_msk = 0x00000003,
        .fault_slave_mask = 0xFFFFFFFF,
        .GPO_seq_on_dep_msk = 0x0000000F,
        .GPO_seq_off_dep_msk = 0x0000000F
    },
    .operation = 0x80,
    .spare = {0,0,0}
    /*
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
    .enable_active_high = true */
};
const power_rail_cfg_t power_digital_2400 = {
    .SEQ_config = {
        .pad1 = 0x55AA,
        .ID = 4,
        .ID_other = 0,
        .GPI_seq_mask_on  = 0x00000001,
        .GPI_seq_mask_off = 0x00000002,
        .seq_timeout_cfg = 0x05,
        .seq_on_timeout = 10,
        .seq_off_timeout = 15,
        .pad2 = 0x99,
        .page_seq_on_dep_msk = 0x00000003,
        .page_seq_off_dep_msk = 0x00000003,
        .fault_slave_mask = 0xFFFFFFFF,
        .GPO_seq_on_dep_msk = 0x0000000F,
        .GPO_seq_off_dep_msk = 0x0000000F
    },
    .operation = 0x80,
    .spare = {0,0,0}
    /*
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
    .enable_active_high = true */
};
const power_rail_cfg_t power_digital_OFFLINE = {
    .SEQ_config = {
        .pad1 = 0x55AA,
        .ID = 0,
        .ID_other = 0,
        .GPI_seq_mask_on  = 0x00000000,
        .GPI_seq_mask_off = 0x00000000,
        .seq_timeout_cfg = 0x05,
        .seq_on_timeout = 20,
        .seq_off_timeout = 6,
        .pad2 = 0x99,
        .page_seq_on_dep_msk = 0x0000000F,
        .page_seq_off_dep_msk = 0x0000000C,
        .fault_slave_mask = 0xFFFFCCFF,
        .GPO_seq_on_dep_msk = 0x0000000C,
        .GPO_seq_off_dep_msk = 0x00000003
    },
    .operation = 0x80,
    .spare = {0,0,0}

    /*
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
    .enable_active_high = true */
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
//        0xff,/* MON[24]*/
//        0xff,/* MON[25]*/
//        0xff,/* MON[26]*/
//        0xff,/* MON[27]*/
//        0xff,/* MON[28]*/
//        0xff,/* MON[29]*/
//        0xff, /* MON[30]*/
//        0xff /* MON[31]*/
    },
    .faults = {
        .fault_mask = {0x00000000, 0x00000000, 0x00000000, 0x00000000},
        .fault_pins = {0x0000,     0x0000,     0x0000,     0x0000},
        .GPI_mask   = {0x00000000, 0x00000000, 0x00000000, 0x00000000},
        .other_mask = 0,
        .spare = {0xff}
    },
    .GPI_config = {
        .GPI = {
            { .id = 6,  .conf = PWR_MON_GPI_MODE_INPUT   }, /* P005, this is SW1 */
            { .id = 5,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* P004*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[2]*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[3]*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[4]*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[5]*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[6]*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[7]*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[8]*/
            { .id = 0,  .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[9]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[10]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[11]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[12]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[13]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[14]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[15]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[16]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[17]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[18]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[19]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[20]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[21]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[22]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[23]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[24]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[25]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[26]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[27]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[28]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[29]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  }, /* GPI[30]*/
            { .id = 0, .conf = PWR_MON_GPI_MODE_UNUSED  } /* GPI[31]*/
        },
        .fault_en = 0x00000000,
        .LSCP = 0x00,
        .MRG_EN = 0x00,
        .MRG_LOW = 0x00,
        .rsv1 = 0xff,
        .debug_pin = 0x00
    },
    .GPO_config = {
        {
         {.id = 1,.conf = 0},
         .conf      = 0xAA,
         .dly       = 0x11,
         .and_path0 = 0x05,
         .and_path1 = 0x03,
         .path = 
         {
            {.status_mask     = 0x00000000,
             .status_inv_mask = 0x00000000,
             .GPI_mask        = 0x00000000,
             .GPI_inv_mask    = 0x00000000,
             .GPO_mask        = 0x0000,
             .GPO_inv_mask    = 0x0000
            },
            {.status_mask     = 0x00000000,
             .status_inv_mask = 0x00000000,
             .GPI_mask        = 0x00000000,
             .GPI_inv_mask    = 0x00000000,
             .GPO_mask        = 0x0000,
             .GPO_inv_mask    = 0x0000
            }
        }
        },
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0},
        {0}
    },
    .GPI        = 0x00000000,
    .resequence = 0x00000000,
    .reset_config = {0},
    .watchdog_config = {0},
    .RTC = {
        .seconds = (30 << 10)  | 500,
        .DHM     = (1 << 11)   | (8 << 6) | 45,
        .YM      = (2026 << 4) | 9,
        .rsv = 0
    },
    .RTC_trim = 0,
    .railstate = { //@@@ does this belong here????
        .spare = {0},
        .state_enables = 0x00,
        .soft_off_enables = 0x00,
        .system_state = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
    },
    .MSCCFG = {
        .misc_cfg = 0x00, /* 7: no limit to how many times the device can be resequenced,*/
                          /* 6: Resequence abort if TOFF_MAX_WARN occurs */
                          /* 5:4  maximum times to resequence 00=1 01=2 ... */
                          /* 3:  Slave see table 26-46 */
                          /* 2:  Enable FIFO Log */
                          /* 1:  External ADC reference enable */
                          /* 0:  SUB oscillator in use */
        .time_2_reseq = 0x00, /* 7:6 00  1mS   (0..63)mS 
                                     01  8mS   (8..504)mS
                                     10 64mS   (64..4032)mS
                                     11 512mS   (512..322565)mS */
        .external_reference = 0x0000, /* LINEAR16 format*/
        .reseq_rail_mask = 0x00000000
        
    },
    .active_rail_group = 0,
    .pmbus_address = 0xC0,
    .cascade_id = 0,
    .cascade_count = 1,
    .watchdog_timeout_ms = 0,
    .gpi_mask = 0,
    .gpo_mask = 0,
    .watchdog_enabled = false,
    .active_rail_group = 0,
    .sync_clock_enabled = false,
    .fault_pin_asserted = false,
    .spare1 = 0xAA
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
const R_PFS_PORT_Type *G = 0;
R_PFS_Type *p;

#define IO_PFS(_a_) (void*) (R_PFS_BASE + ((_a_ & 0xFF00)>>2) + (0x04 * (_a_ & 0x000F)))

const power_sequencer_IO_t power_sequencer_IO = {
.usage = {
.MON = {
IO_PFS(BSP_IO_PORT_00_PIN_00),   /* PIN ID 1 */
IO_PFS(BSP_IO_PORT_00_PIN_01),
IO_PFS(BSP_IO_PORT_00_PIN_02),
IO_PFS(BSP_IO_PORT_00_PIN_03),
IO_PFS(BSP_IO_PORT_00_PIN_04),
IO_PFS(BSP_IO_PORT_00_PIN_05),
IO_PFS(BSP_IO_PORT_00_PIN_06), /* BLUE LED */
IO_PFS(BSP_IO_PORT_00_PIN_07), /* GREEN LED */
IO_PFS(BSP_IO_PORT_00_PIN_08), /* RED LED */
IO_PFS(BSP_IO_PORT_00_PIN_09),
IO_PFS(BSP_IO_PORT_00_PIN_10),
IO_PFS(BSP_IO_PORT_00_PIN_14),
IO_PFS(BSP_IO_PORT_00_PIN_15),
IO_PFS(BSP_IO_PORT_05_PIN_00),  /* used in USB on EK */
IO_PFS(BSP_IO_PORT_05_PIN_01),  /* used in USB on EK */
IO_PFS(BSP_IO_PORT_05_PIN_02),
IO_PFS(BSP_IO_PORT_05_PIN_03),
IO_PFS(BSP_IO_PORT_05_PIN_04),
IO_PFS(BSP_IO_PORT_05_PIN_05),  /* this pin is pulled high with a 1k5 */
IO_PFS(BSP_IO_PORT_05_PIN_06),  /* this pin is pulled high with a 1k5 */
IO_PFS(BSP_IO_PORT_05_PIN_07),
IO_PFS(BSP_IO_PORT_05_PIN_08),
IO_PFS(BSP_IO_PORT_08_PIN_00),
IO_PFS(BSP_IO_PORT_08_PIN_01),
},
.DMON = {
IO_PFS(BSP_IO_PORT_02_PIN_06), /* IRQ0 */ /* this has a 10k pull up on the board */ /* PIN ID 25 */
IO_PFS(BSP_IO_PORT_02_PIN_05), /* IRQ1 */ /* this has a 10k pullup */
IO_PFS(BSP_IO_PORT_02_PIN_03), /* IRQ2 */
IO_PFS(BSP_IO_PORT_02_PIN_02), /* IRQ3 */
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF) },
.EN = {  /* Ones with pullups route to OSPI */
IO_PFS(BSP_IO_PORT_01_PIN_00),  /* IRQ2*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_01),  /* IRQ1*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_02),  /*     */  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_03),  /*     */  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_04),  /* IRQ1*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_05),  /* IRQ0*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_06),  /*     */  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_07),  /*     */  /* this has a 10k pull up on the board */
IO_PFS(0xFFFF), /* BSP_IO_PORT_01_PIN_08*/ /*     */  /* DEBUG SWDIO */
IO_PFS(0xFFFF), /* BSP_IO_PORT_01_PIN_09*/ /*     */ /* DEBUG TDO */
IO_PFS(0xFFFF), /* BSP_IO_PORT_01_PIN_10*//* IRQ3*/  /* DEBUG TDI */
IO_PFS(BSP_IO_PORT_01_PIN_11), /* IRQ4*/ 
IO_PFS(BSP_IO_PORT_01_PIN_12), /*     */ 
IO_PFS(BSP_IO_PORT_01_PIN_13), /*     */ 
IO_PFS(BSP_IO_PORT_01_PIN_14), /*     */ 
IO_PFS(BSP_IO_PORT_01_PIN_15), /*     */ 
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
} ,
.MAR = {
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF)
},
.GPIO = {
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF),
IO_PFS(0xFFFF)
}
}

};

const power_sequencer_CONST_t power_sequencer_CONST = {
    .max_digital_comp = 8,
    .max_GPOs = 16,
    .max_GPIs = 32,
    .max_pages = 32,
    .max_fans = 0,
    .max_monitors = 0,
    .max_fault_entries = 8,
    .max_PWMs = 0
};
