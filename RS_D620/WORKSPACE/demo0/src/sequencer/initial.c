#include "application_common.h"
#include "power_module.h"
#include "ra/fsp/src/bsp/mcu/all/bsp_io.h"

/*
    Minimal PMBus LINEAR16/LINEAR11 encoders, used below to stub plausible
    voltage/time/temperature register values for power_rail_cfg_t.

    PMBUS_L16_MV assumes VOUT_MODE's exponent is fixed at -9 (the common TI
    default - see .vout_mode = 0x17 below: mode=000 linear, exponent=0b10111
    = -9), so mantissa = mV * 2^9 / 1000.

    PMBUS_L11_N0 encodes a LINEAR11 value with exponent N=0 (plain integer
    units, e.g. degrees C or a x1 gain). PMBUS_L11_MS encodes a LINEAR11
    value with exponent N=-3 (1/8 ms units), used for the ms-based delay and
    timeout fields.
*/
#define PMBUS_L16_MV(_mv_)   ((uint16_t) (((uint32_t)(_mv_) * 512u) / 1000u))
#define PMBUS_L11_N0(_y_)    ((uint16_t) ((_y_) & 0x07FFu))
#define PMBUS_L11_MS(_ms_)   ((uint16_t) (0xE800u | (((uint32_t)(_ms_) * 8u) & 0x07FFu)))
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
    .fault_responses = { /* 0x00 placeholders: TODO confirm FAULT_RESPONSE bit encoding against datasheet */
        .vout_ov = 0x00, .vout_uv = 0x00, .iout_oc = 0x00, .iout_uc = 0x00, .ot = 0x00, .ton_max = 0x00,
        .time_to_retry = 0, .max_glitch_voltage_fault = 0, .max_glitch_voltage_no_fault = 0, .spare = {0,0,0}
    },
    .gpi_fault_responses = { /* no GPI faults mapped */
        .gpi_response = {0}, .time_to_retry = 0, .max_glitch_GPI = 0,
        .profile_section_1 = 0, .profile_section_2 = 0, .block_out_period = 0
    },
    .operation = 0x80,
    .on_off_config = 0x00,          /* TODO confirm ON_OFF_CONFIG bit encoding against datasheet */
    .vout_mode = 0x17,              /* linear mode, exponent -9 (see PMBUS_L16_MV above) */
    .ton_max_fault_response = 0x00, /* TODO confirm FAULT_RESPONSE bit encoding against datasheet */
    .vout_command      = PMBUS_L16_MV(3300),
    .status_vout = 0x00,
    .status_iout = 0x00,
    .status_temperature = 0x00,
    .vout_margin_high     = PMBUS_L16_MV(3465), /* +5% */
    .vout_margin_low      = PMBUS_L16_MV(3135), /* -5% */
    .vout_scale_monitor   = PMBUS_L11_N0(1),    /* x1, no attenuation */
    .vout_ov_fault_limit  = PMBUS_L16_MV(3564), /* +8% */
    .vout_ov_warn_limit   = PMBUS_L16_MV(3465), /* +5% */
    .vout_uv_warn_limit   = PMBUS_L16_MV(3135), /* -5% */
    .vout_uv_fault_limit  = PMBUS_L16_MV(3036), /* -8% */
    .ot_fault_limit = PMBUS_L11_N0(105), /* degC */
    .ot_warn_limit  = PMBUS_L11_N0(95),  /* degC */
    .power_good_on  = PMBUS_L16_MV(3135), /* -5% */
    .power_good_off = PMBUS_L16_MV(2970), /* -10% */
    .ton_delay            = PMBUS_L11_MS(5),
    .ton_max_fault_limit  = PMBUS_L11_MS(10),
    .toff_delay           = PMBUS_L11_MS(5),
    .toff_max_warn_limit  = PMBUS_L11_MS(50),
    .vout_cal_monitor = 0,         /* no offset calibration */
    .temperature_cal_gain = 0,     /* no gain calibration */
    .temperature_cal_offset = 0,   /* no offset calibration */
    .margin_config = 0x0000,       /* margining disabled / default pin */

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
    .fault_responses = { /* 0x00 placeholders: TODO confirm FAULT_RESPONSE bit encoding against datasheet */
        .vout_ov = 0x00, .vout_uv = 0x00, .iout_oc = 0x00, .iout_uc = 0x00, .ot = 0x00, .ton_max = 0x00,
        .time_to_retry = 0, .max_glitch_voltage_fault = 0, .max_glitch_voltage_no_fault = 0, .spare = {0,0,0}
    },
    .gpi_fault_responses = { /* no GPI faults mapped */
        .gpi_response = {0}, .time_to_retry = 0, .max_glitch_GPI = 0,
        .profile_section_1 = 0, .profile_section_2 = 0, .block_out_period = 0
    },
    .operation = 0x80,
    .on_off_config = 0x00,          /* TODO confirm ON_OFF_CONFIG bit encoding against datasheet */
    .vout_mode = 0x17,              /* linear mode, exponent -9 (see PMBUS_L16_MV above) */
    .ton_max_fault_response = 0x00, /* TODO confirm FAULT_RESPONSE bit encoding against datasheet */
    .vout_command      = PMBUS_L16_MV(5000),
    .status_vout = 0x00,
    .status_iout = 0x00,
    .status_temperature = 0x00,
    .vout_margin_high     = PMBUS_L16_MV(5250), /* +5% */
    .vout_margin_low      = PMBUS_L16_MV(4750), /* -5% */
    .vout_scale_monitor   = PMBUS_L11_N0(1),    /* x1, no attenuation */
    .vout_ov_fault_limit  = PMBUS_L16_MV(5400), /* +8% */
    .vout_ov_warn_limit   = PMBUS_L16_MV(5250), /* +5% */
    .vout_uv_warn_limit   = PMBUS_L16_MV(4750), /* -5% */
    .vout_uv_fault_limit  = PMBUS_L16_MV(4600), /* -8% */
    .ot_fault_limit = PMBUS_L11_N0(105), /* degC */
    .ot_warn_limit  = PMBUS_L11_N0(95),  /* degC */
    .power_good_on  = PMBUS_L16_MV(4750), /* -5% */
    .power_good_off = PMBUS_L16_MV(4500), /* -10% */
    .ton_delay            = PMBUS_L11_MS(10),
    .ton_max_fault_limit  = PMBUS_L11_MS(100),
    .toff_delay           = PMBUS_L11_MS(20),
    .toff_max_warn_limit  = PMBUS_L11_MS(150),
    .vout_cal_monitor = 0,         /* no offset calibration */
    .temperature_cal_gain = 0,     /* no gain calibration */
    .temperature_cal_offset = 0,   /* no offset calibration */
    .margin_config = 0x0000,       /* margining disabled / default pin */
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
    .fault_responses = { /* 0x00 placeholders: TODO confirm FAULT_RESPONSE bit encoding against datasheet */
        .vout_ov = 0x00, .vout_uv = 0x00, .iout_oc = 0x00, .iout_uc = 0x00, .ot = 0x00, .ton_max = 0x00,
        .time_to_retry = 0, .max_glitch_voltage_fault = 0, .max_glitch_voltage_no_fault = 0, .spare = {0,0,0}
    },
    .gpi_fault_responses = { /* no GPI faults mapped */
        .gpi_response = {0}, .time_to_retry = 0, .max_glitch_GPI = 0,
        .profile_section_1 = 0, .profile_section_2 = 0, .block_out_period = 0
    },
    .operation = 0x80,
    .on_off_config = 0x00,          /* TODO confirm ON_OFF_CONFIG bit encoding against datasheet */
    .vout_mode = 0x17,              /* linear mode, exponent -9 (see PMBUS_L16_MV above) */
    .ton_max_fault_response = 0x00, /* TODO confirm FAULT_RESPONSE bit encoding against datasheet */
    .vout_command      = PMBUS_L16_MV(12000),
    .status_vout = 0x00,
    .status_iout = 0x00,
    .status_temperature = 0x00,
    .vout_margin_high     = PMBUS_L16_MV(12600), /* +5% */
    .vout_margin_low      = PMBUS_L16_MV(11400), /* -5% */
    .vout_scale_monitor   = PMBUS_L11_N0(1),     /* x1, no attenuation */
    .vout_ov_fault_limit  = PMBUS_L16_MV(12960), /* +8% */
    .vout_ov_warn_limit   = PMBUS_L16_MV(12600), /* +5% */
    .vout_uv_warn_limit   = PMBUS_L16_MV(11400), /* -5% */
    .vout_uv_fault_limit  = PMBUS_L16_MV(11040), /* -8% */
    .ot_fault_limit = PMBUS_L11_N0(105), /* degC */
    .ot_warn_limit  = PMBUS_L11_N0(95),  /* degC */
    .power_good_on  = PMBUS_L16_MV(11400), /* -5% */
    .power_good_off = PMBUS_L16_MV(10800), /* -10% */
    .ton_delay            = PMBUS_L11_MS(2),
    .ton_max_fault_limit  = PMBUS_L11_MS(10),
    .toff_delay           = PMBUS_L11_MS(30),
    .toff_max_warn_limit  = PMBUS_L11_MS(80),
    .vout_cal_monitor = 0,         /* no offset calibration */
    .temperature_cal_gain = 0,     /* no gain calibration */
    .temperature_cal_offset = 0,   /* no offset calibration */
    .margin_config = 0x0000,       /* margining disabled / default pin */
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
    .fault_responses = { /* 0x00 placeholders: TODO confirm FAULT_RESPONSE bit encoding against datasheet */
        .vout_ov = 0x00, .vout_uv = 0x00, .iout_oc = 0x00, .iout_uc = 0x00, .ot = 0x00, .ton_max = 0x00,
        .time_to_retry = 0, .max_glitch_voltage_fault = 0, .max_glitch_voltage_no_fault = 0, .spare = {0,0,0}
    },
    .gpi_fault_responses = { /* no GPI faults mapped */
        .gpi_response = {0}, .time_to_retry = 0, .max_glitch_GPI = 0,
        .profile_section_1 = 0, .profile_section_2 = 0, .block_out_period = 0
    },
    .operation = 0x80,
    .on_off_config = 0x00,          /* TODO confirm ON_OFF_CONFIG bit encoding against datasheet */
    .vout_mode = 0x17,              /* linear mode, exponent -9 (see PMBUS_L16_MV above) */
    .ton_max_fault_response = 0x00, /* TODO confirm FAULT_RESPONSE bit encoding against datasheet */
    .vout_command      = PMBUS_L16_MV(2400),
    .status_vout = 0x00,
    .status_iout = 0x00,
    .status_temperature = 0x00,
    .vout_margin_high     = PMBUS_L16_MV(2520), /* +5% */
    .vout_margin_low      = PMBUS_L16_MV(2280), /* -5% */
    .vout_scale_monitor   = PMBUS_L11_N0(1),    /* x1, no attenuation */
    .vout_ov_fault_limit  = PMBUS_L16_MV(2592), /* +8% */
    .vout_ov_warn_limit   = PMBUS_L16_MV(2520), /* +5% */
    .vout_uv_warn_limit   = PMBUS_L16_MV(2280), /* -5% */
    .vout_uv_fault_limit  = PMBUS_L16_MV(2208), /* -8% */
    .ot_fault_limit = PMBUS_L11_N0(105), /* degC */
    .ot_warn_limit  = PMBUS_L11_N0(95),  /* degC */
    .power_good_on  = PMBUS_L16_MV(2280), /* -5% */
    .power_good_off = PMBUS_L16_MV(2160), /* -10% */
    .ton_delay            = PMBUS_L11_MS(60),
    .ton_max_fault_limit  = PMBUS_L11_MS(50),
    .toff_delay           = PMBUS_L11_MS(0),
    .toff_max_warn_limit  = PMBUS_L11_MS(50),
    .vout_cal_monitor = 0,         /* no offset calibration */
    .temperature_cal_gain = 0,     /* no gain calibration */
    .temperature_cal_offset = 0,   /* no offset calibration */
    .margin_config = 0x0000,       /* margining disabled / default pin */
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
    .fault_responses = { /* 0x00 placeholders: TODO confirm FAULT_RESPONSE bit encoding against datasheet */
        .vout_ov = 0x00, .vout_uv = 0x00, .iout_oc = 0x00, .iout_uc = 0x00, .ot = 0x00, .ton_max = 0x00,
        .time_to_retry = 0, .max_glitch_voltage_fault = 0, .max_glitch_voltage_no_fault = 0, .spare = {0,0,0}
    },
    .gpi_fault_responses = { /* no GPI faults mapped */
        .gpi_response = {0}, .time_to_retry = 0, .max_glitch_GPI = 0,
        .profile_section_1 = 0, .profile_section_2 = 0, .block_out_period = 0
    },
    .operation = 0x80,
    .on_off_config = 0x00,          /* TODO confirm ON_OFF_CONFIG bit encoding against datasheet */
    .vout_mode = 0x17,              /* linear mode, exponent -9 (see PMBUS_L16_MV above) */
    .ton_max_fault_response = 0x00, /* TODO confirm FAULT_RESPONSE bit encoding against datasheet */
    .vout_command      = PMBUS_L16_MV(2400),
    .status_vout = 0x00,
    .status_iout = 0x00,
    .status_temperature = 0x00,
    .vout_margin_high     = PMBUS_L16_MV(2520), /* +5% */
    .vout_margin_low      = PMBUS_L16_MV(2280), /* -5% */
    .vout_scale_monitor   = PMBUS_L11_N0(1),    /* x1, no attenuation */
    .vout_ov_fault_limit  = PMBUS_L16_MV(2592), /* +8% */
    .vout_ov_warn_limit   = PMBUS_L16_MV(2520), /* +5% */
    .vout_uv_warn_limit   = PMBUS_L16_MV(2280), /* -5% */
    .vout_uv_fault_limit  = PMBUS_L16_MV(2208), /* -8% */
    .ot_fault_limit = PMBUS_L11_N0(105), /* degC */
    .ot_warn_limit  = PMBUS_L11_N0(95),  /* degC */
    .power_good_on  = PMBUS_L16_MV(2280), /* -5% */
    .power_good_off = PMBUS_L16_MV(2160), /* -10% */
    .ton_delay            = PMBUS_L11_MS(60),
    .ton_max_fault_limit  = PMBUS_L11_MS(50),
    .toff_delay           = PMBUS_L11_MS(0),
    .toff_max_warn_limit  = PMBUS_L11_MS(50),
    .vout_cal_monitor = 0,         /* no offset calibration */
    .temperature_cal_gain = 0,     /* no gain calibration */
    .temperature_cal_offset = 0,   /* no offset calibration */
    .margin_config = 0x0000,       /* margining disabled / default pin */

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
        (PWR_MON_TYPE_ANALOG  | 1), /* Tie MON1 to the voltage monitoring of rail[0]*/
        (PWR_MON_TYPE_ANALOG  | 2), /* MON2 to the current monitoring of rail[0]*/  
        (PWR_MON_TYPE_ANALOG  | 3), /* MON3 to the voltage monitoring of rail[1]*/  
        (PWR_MON_TYPE_NONE    | 0), /* MON4 to the voltage monitoring of rail[0]*/  
        (PWR_MON_TYPE_NONE    | 0), /* physically connected to SW2 on EK boards */
        (PWR_MON_TYPE_NONE),        /* physically connected to SW1 on EK boards */
        (PWR_MON_TYPE_NONE),        /* BLUE LED */
        (PWR_MON_TYPE_NONE),        /* GREEN LED */
        (PWR_MON_TYPE_NONE),        /* RED LED */
        (PWR_MON_TYPE_NONE),
        (PWR_MON_TYPE_NONE),/* MON[11]*/
        (PWR_MON_TYPE_NONE),/* MON[12]*/
        (PWR_MON_TYPE_NONE),/* MON[13]*/
        (PWR_MON_TYPE_NONE),/* MON[14]*/
        (PWR_MON_TYPE_NONE),/* MON[15]*/
        (PWR_MON_TYPE_NONE),/* MON[16]*/
        (PWR_MON_TYPE_NONE),/* MON[17]*/
        (PWR_MON_TYPE_NONE),/* MON[18]*/
        (PWR_MON_TYPE_NONE),/* MON[19]*/
        (PWR_MON_TYPE_NONE),/* MON[20]*/
        (PWR_MON_TYPE_NONE),/* MON[21]*/
        (PWR_MON_TYPE_NONE),/* MON[22]*/
        (PWR_MON_TYPE_NONE),/* MON[23]*/
        (PWR_MON_TYPE_NONE),/* MON[24]*/
        (PWR_MON_TYPE_NONE),/* MON[25]*/
        (PWR_MON_TYPE_NONE),/* MON[26]*/
        (PWR_MON_TYPE_NONE),/* MON[27]*/
        (PWR_MON_TYPE_NONE),/* MON[28]*/
        (PWR_MON_TYPE_NONE),/* MON[29]*/
        (PWR_MON_TYPE_NONE), /* MON[30]*/
        (PWR_MON_TYPE_NONE) /* MON[31]*/
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
IO_PFS(BSP_IO_PORT_00_PIN_00),   /* ID 1 */    /* AN000 */
IO_PFS(BSP_IO_PORT_00_PIN_01),                 /* AN001 */
IO_PFS(BSP_IO_PORT_00_PIN_02),                 /* AN002 */
IO_PFS(BSP_IO_PORT_00_PIN_03),                 /* AN003 */
IO_PFS(BSP_IO_PORT_00_PIN_04),                 /* AN004 */
IO_PFS(BSP_IO_PORT_00_PIN_05),                 /* AN005 */
IO_PFS(BSP_IO_PORT_00_PIN_06),                 /* AN006 */ /* BLUE LED */
IO_PFS(BSP_IO_PORT_00_PIN_07),                 /* AN007 */ /* GREEN LED */
IO_PFS(BSP_IO_PORT_00_PIN_08),                 /* AN008 */ /* RED LED */
IO_PFS(BSP_IO_PORT_00_PIN_09),                 /* AN009 */
IO_PFS(BSP_IO_PORT_00_PIN_10), /* ID 11 */     /* AN010 */  //@@@ note there is no AN011???
IO_PFS(BSP_IO_PORT_00_PIN_14),                 /* AN012 */
IO_PFS(BSP_IO_PORT_00_PIN_15), /* ID 13 */     /* AN013 */

IO_PFS(BSP_IO_PORT_05_PIN_00),                 /* AN116 */            /* used in USB on EK */
IO_PFS(BSP_IO_PORT_05_PIN_01),                 /* AN117 */            /* used in USB on EK */
IO_PFS(BSP_IO_PORT_05_PIN_02),                 /* AN118 */
IO_PFS(BSP_IO_PORT_05_PIN_03),                 /* AN119 */
IO_PFS(BSP_IO_PORT_05_PIN_04),                 /* AN120 */
IO_PFS(BSP_IO_PORT_05_PIN_05),                 /* AN121 */ /* this pin is pulled high with a 1k5 */
IO_PFS(BSP_IO_PORT_05_PIN_06),                 /* AN122 */ /* this pin is pulled high with a 1k5 */
IO_PFS(BSP_IO_PORT_05_PIN_07),                 /* AN123 */
IO_PFS(BSP_IO_PORT_05_PIN_08),                 /* AN124 */
IO_PFS(BSP_IO_PORT_08_PIN_00),                 /* AN125 */
IO_PFS(BSP_IO_PORT_08_PIN_01), /* ID 24 */     /* AN126 */           //@@@ NOTE  P802 and P803 are mapped to AN127 and AN128 
},
.DMON = {
IO_PFS(BSP_IO_PORT_02_PIN_06), /* ID 25 */     /* IRQ0 */ /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_02_PIN_05), /* ID 26 */     /* IRQ1 */ /* this has a 10k pullup */
IO_PFS(BSP_IO_PORT_02_PIN_03), /* ID 27 */     /* IRQ2 */
IO_PFS(BSP_IO_PORT_02_PIN_02), /* ID 28 */     /* IRQ3 */
IO_PFS(0xFFFF),                /* ID 29 */
IO_PFS(0xFFFF),                /* ID 30 */
IO_PFS(0xFFFF),                /* ID 31 */
IO_PFS(0xFFFF) },              /* ID 32 */
.EN = {  /* Ones with pullups route to OSPI */
IO_PFS(BSP_IO_PORT_01_PIN_00), /* ID 33 */ /* IRQ2*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_01), /* ID 34 */ /* IRQ1*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_02), /* ID 35 */ /*     */  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_03), /* ID 36 */ /*     */  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_04), /* ID 37 */ /* IRQ1*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_05), /* ID 38 */ /* IRQ0*/  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_06), /* ID 39 */ /*     */  /* this has a 10k pull up on the board */
IO_PFS(BSP_IO_PORT_01_PIN_07), /* ID 40 */ /*     */  /* this has a 10k pull up on the board */
IO_PFS(0xFFFF), /* BSP_IO_PORT_01_PIN_08*/ /*     */  /* DEBUG SWDIO */
IO_PFS(0xFFFF), /* BSP_IO_PORT_01_PIN_09*/ /*     */ /* DEBUG TDO */
IO_PFS(0xFFFF), /* BSP_IO_PORT_01_PIN_10*/ /* IRQ3*/  /* DEBUG TDI */
IO_PFS(BSP_IO_PORT_01_PIN_11), /* ID 44 */ /* IRQ4*/ 
IO_PFS(BSP_IO_PORT_01_PIN_12), /* ID 45 */ /*     */ 
IO_PFS(BSP_IO_PORT_01_PIN_13), /* ID 46 */ /*     */ 
IO_PFS(BSP_IO_PORT_01_PIN_14), /* ID 47 */ /*     */ 
IO_PFS(BSP_IO_PORT_01_PIN_15), /* ID 48 */ /*     */ 
IO_PFS(0xFFFF),                /* ID 49 */
IO_PFS(0xFFFF),                /* ID 50 */
IO_PFS(0xFFFF), /* ID 51 */
IO_PFS(0xFFFF), /* ID 52 */
IO_PFS(0xFFFF), /* ID 53 */
IO_PFS(0xFFFF), /* ID 54 */
IO_PFS(0xFFFF), /* ID 55 */
IO_PFS(0xFFFF), /* ID 56 */
IO_PFS(0xFFFF), /* ID 57 */
IO_PFS(0xFFFF), /* ID 58 */
IO_PFS(0xFFFF), /* ID 59 */
IO_PFS(0xFFFF), /* ID 60 */
IO_PFS(0xFFFF), /* ID 61 */
IO_PFS(0xFFFF), /* ID 62 */
IO_PFS(0xFFFF), /* ID 63 */
IO_PFS(0xFFFF), /* ID 64 */
} ,
.MAR = {
IO_PFS(0xFFFF), /* ID 65 */
IO_PFS(0xFFFF), /* ID 66 */
IO_PFS(0xFFFF), /* ID 67 */
IO_PFS(0xFFFF), /* ID 68 */
IO_PFS(0xFFFF), /* ID 69 */
IO_PFS(0xFFFF), /* ID 70 */
IO_PFS(0xFFFF), /* ID 71 */
IO_PFS(0xFFFF), /* ID 72 */
IO_PFS(0xFFFF), /* ID 73 */
IO_PFS(0xFFFF), /* ID 74 */
IO_PFS(0xFFFF), /* ID 75 */
IO_PFS(0xFFFF), /* ID 76 */
IO_PFS(0xFFFF), /* ID 77 */
IO_PFS(0xFFFF), /* ID 78 */
IO_PFS(0xFFFF), /* ID 79 */
IO_PFS(0xFFFF)  /* ID 80 */
},
.GPIO = {
IO_PFS(0xFFFF), /* ID 81 */
IO_PFS(0xFFFF), /* ID 82 */
IO_PFS(0xFFFF), /* ID 83 */
IO_PFS(0xFFFF), /* ID 84 */
IO_PFS(0xFFFF), /* ID 85 */
IO_PFS(0xFFFF), /* ID 86 */
IO_PFS(0xFFFF), /* ID 87 */
IO_PFS(0xFFFF)  /* ID 88 */
},
.PMBUS = {
    .SDA = IO_PFS(BSP_IO_PORT_05_PIN_11),
    .SCL = IO_PFS(BSP_IO_PORT_05_PIN_12),
    .SEL = { IO_PFS(BSP_IO_PORT_06_PIN_00), 
             IO_PFS(BSP_IO_PORT_06_PIN_01), 
             IO_PFS(BSP_IO_PORT_06_PIN_03) },
    .ALERT = IO_PFS(BSP_IO_PORT_06_PIN_04),
    .CONTROL = IO_PFS(BSP_IO_PORT_06_PIN_05)
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
