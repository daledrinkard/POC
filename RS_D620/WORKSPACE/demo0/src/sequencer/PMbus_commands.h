/*
 *     PMBUS_COMMANDS.H
 *
 * PMBus command code enumeration for the TI UCD91320 32-Rail PMBus Power
 * Sequencer and System Manager, transcribed from documents/UCD91320_commands.pdf
 * (UCD91xxx Sequencer and System Health Controller PMBus Command Reference,
 * SLVUCU5, December 2025), Table 21-1 "PMBus Commands" and the per-command
 * implementation sections (22, 25, 26) that give each command's hex code.
 *
 * Commands 00h-CFh are PMBus-specification core commands; D0h-FFh are
 * UCD91320 manufacturer-specific commands (their MFR_SPECIFIC_nn / USER_DATA_nn
 * internal names are noted in trailing comments). Codes not implemented by
 * the UCD91320 (marked "Reserved" in Table 21-1) are omitted below.
 *
 */

#ifndef PMBUS_COMMANDS_H_
#define PMBUS_COMMANDS_H_
#include <stdint.h>
/* USER INCLUDE */

#define PMBUS_OPERATION_ON_OFF       (0x80) /* bit 7 = 1: ON, 0: OFF */
#define PMBUS_OPERATION_OFF_BEHAVIOR (0x40) /* bit 6 = (if bit 7=0) 0: off immediately, 1: off after delay */
#define PMBUS_OPERATION_VOLTAGE_CMD  (0x30)
#define PMBUS_OPERATION_MARGIN_RESP  (0x0C)
#define PMBUS_OPERATION_TRANSISTION  (0x02)
#define PMBUS_OPERATION_RESERVED     (0x01)

typedef enum pmbus_command_e {
    /* core PMBus commands (00h-CFh) */
    PMBUS_CMD_PAGE                       = 0x00,
    PMBUS_CMD_OPERATION                  = 0x01,
    PMBUS_CMD_ON_OFF_CONFIG              = 0x02,
    PMBUS_CMD_CLEAR_FAULTS               = 0x03,
    PMBUS_CMD_PHASE                      = 0x04,
    PMBUS_CMD_PASSKEY                    = 0x0E,
    PMBUS_CMD_ACCESS_CONTROL             = 0x0F,
    PMBUS_CMD_WRITE_PROTECT              = 0x10,
    PMBUS_CMD_STORE_DEFAULT_ALL          = 0x11,
    PMBUS_CMD_RESTORE_DEFAULT_ALL        = 0x12,
    PMBUS_CMD_STORE_DEFAULT_CODE         = 0x13,
    PMBUS_CMD_RESTORE_DEFAULT_CODE       = 0x14,
    PMBUS_CMD_STORE_USER_ALL             = 0x15,
    PMBUS_CMD_RESTORE_USER_ALL           = 0x16,
    PMBUS_CMD_STORE_USER_CODE            = 0x17,
    PMBUS_CMD_RESTORE_USER_CODE          = 0x18,
    PMBUS_CMD_CAPABILITY                 = 0x19,
    PMBUS_CMD_QUERY                      = 0x1A,
    PMBUS_CMD_SMBALERT_MASK              = 0x1B,
    PMBUS_CMD_VOUT_MODE                  = 0x20,
    PMBUS_CMD_VOUT_COMMAND               = 0x21,
    PMBUS_CMD_VOUT_TRIM                  = 0x22,
    PMBUS_CMD_VOUT_CAL_OFFSET            = 0x23,
    PMBUS_CMD_VOUT_MAX                   = 0x24,
    PMBUS_CMD_VOUT_MARGIN_HIGH           = 0x25,
    PMBUS_CMD_VOUT_MARGIN_LOW            = 0x26,
    PMBUS_CMD_VOUT_TRANSITION_RATE       = 0x27,
    PMBUS_CMD_VOUT_DROOP                 = 0x28,
    PMBUS_CMD_VOUT_SCALE_LOOP            = 0x29,
    PMBUS_CMD_VOUT_SCALE_MONITOR         = 0x2A,
    PMBUS_CMD_COEFFICIENTS               = 0x30,
    PMBUS_CMD_POUT_MAX                   = 0x31,
    PMBUS_CMD_MAX_DUTY                   = 0x32,
    PMBUS_CMD_FREQUENCY_SWITCH           = 0x33,
    PMBUS_CMD_VIN_ON                     = 0x35,
    PMBUS_CMD_VIN_OFF                    = 0x36,
    PMBUS_CMD_INTERLEAVE                 = 0x37,
    PMBUS_CMD_IOUT_CAL_GAIN              = 0x38,
    PMBUS_CMD_IOUT_CAL_OFFSET            = 0x39,
    PMBUS_CMD_FAN_CONFIG_1_2             = 0x3A,
    PMBUS_CMD_FAN_COMMAND_1              = 0x3B,
    PMBUS_CMD_FAN_COMMAND_2              = 0x3C,
    PMBUS_CMD_FAN_CONFIG_3_4             = 0x3D,
    PMBUS_CMD_FAN_COMMAND_3              = 0x3E,
    PMBUS_CMD_FAN_COMMAND_4              = 0x3F,
    PMBUS_CMD_VOUT_OV_FAULT_LIMIT        = 0x40,
    PMBUS_CMD_VOUT_OV_FAULT_RESPONSE     = 0x41,
    PMBUS_CMD_VOUT_OV_WARN_LIMIT         = 0x42,
    PMBUS_CMD_VOUT_UV_WARN_LIMIT         = 0x43,
    PMBUS_CMD_VOUT_UV_FAULT_LIMIT        = 0x44,
    PMBUS_CMD_VOUT_UV_FAULT_RESPONSE     = 0x45,
    PMBUS_CMD_IOUT_OC_FAULT_LIMIT        = 0x46,
    PMBUS_CMD_IOUT_OC_FAULT_RESPONSE     = 0x47,
    PMBUS_CMD_IOUT_OC_LV_FAULT_LIMIT     = 0x48,
    PMBUS_CMD_IOUT_OC_LV_FAULT_RESPONSE  = 0x49,
    PMBUS_CMD_IOUT_OC_WARN_LIMIT         = 0x4A,
    PMBUS_CMD_IOUT_UC_FAULT_LIMIT        = 0x4B,
    PMBUS_CMD_IOUT_UC_FAULT_RESPONSE     = 0x4C,
    PMBUS_CMD_OT_FAULT_LIMIT             = 0x4F,
    PMBUS_CMD_OT_FAULT_RESPONSE          = 0x50,
    PMBUS_CMD_OT_WARN_LIMIT              = 0x51,
    PMBUS_CMD_UT_WARN_LIMIT              = 0x52,
    PMBUS_CMD_UT_FAULT_LIMIT             = 0x53,
    PMBUS_CMD_UT_FAULT_RESPONSE          = 0x54,
    PMBUS_CMD_VIN_OV_FAULT_LIMIT         = 0x55,
    PMBUS_CMD_VIN_OV_FAULT_RESPONSE      = 0x56,
    PMBUS_CMD_VIN_OV_WARN_LIMIT          = 0x57,
    PMBUS_CMD_VIN_UV_WARN_LIMIT          = 0x58,
    PMBUS_CMD_VIN_UV_FAULT_LIMIT         = 0x59,
    PMBUS_CMD_VIN_UV_FAULT_RESPONSE      = 0x5A,
    PMBUS_CMD_IIN_OC_FAULT_LIMIT         = 0x5B,
    PMBUS_CMD_IIN_OC_FAULT_RESPONSE      = 0x5C,
    PMBUS_CMD_IIN_OC_WARN_LIMIT          = 0x5D,
    PMBUS_CMD_POWER_GOOD_ON              = 0x5E,
    PMBUS_CMD_POWER_GOOD_OFF             = 0x5F,
    PMBUS_CMD_TON_DELAY                  = 0x60,
    PMBUS_CMD_TON_RISE                   = 0x61,
    PMBUS_CMD_TON_MAX_FAULT_LIMIT        = 0x62,
    PMBUS_CMD_TON_MAX_FAULT_RESPONSE     = 0x63,
    PMBUS_CMD_TOFF_DELAY                 = 0x64,
    PMBUS_CMD_TOFF_FALL                  = 0x65,
    PMBUS_CMD_TOFF_MAX_WARN_LIMIT        = 0x66,
    PMBUS_CMD_POUT_OP_FAULT_LIMIT        = 0x68,
    PMBUS_CMD_POUT_OP_FAULT_RESPONSE     = 0x69,
    PMBUS_CMD_POUT_OP_WARN_LIMIT         = 0x6A,
    PMBUS_CMD_PIN_OP_WARN_LIMIT          = 0x6B,
    PMBUS_CMD_STATUS_BYTE                = 0x78,
    PMBUS_CMD_STATUS_WORD                = 0x79,
    PMBUS_CMD_STATUS_VOUT                = 0x7A,
    PMBUS_CMD_STATUS_IOUT                = 0x7B,
    PMBUS_CMD_STATUS_INPUT               = 0x7C,
    PMBUS_CMD_STATUS_TEMPERATURE         = 0x7D,
    PMBUS_CMD_STATUS_CML                 = 0x7E,
    PMBUS_CMD_STATUS_OTHER               = 0x7F,
    PMBUS_CMD_STATUS_MFR_SPECIFIC        = 0x80,
    PMBUS_CMD_STATUS_FANS_1_2            = 0x81,
    PMBUS_CMD_STATUS_FANS_3_4            = 0x82,
    PMBUS_CMD_READ_VIN                   = 0x88,
    PMBUS_CMD_READ_IIN                   = 0x89,
    PMBUS_CMD_READ_VCAP                  = 0x8A,
    PMBUS_CMD_READ_VOUT                  = 0x8B,
    PMBUS_CMD_READ_IOUT                  = 0x8C,
    PMBUS_CMD_READ_TEMPERATURE_1         = 0x8D,
    PMBUS_CMD_READ_TEMPERATURE_2         = 0x8E,
    PMBUS_CMD_READ_TEMPERATURE_3         = 0x8F,
    PMBUS_CMD_READ_FAN_SPEED_1           = 0x90,
    PMBUS_CMD_READ_FAN_SPEED_2           = 0x91,
    PMBUS_CMD_READ_FAN_SPEED_3           = 0x92,
    PMBUS_CMD_READ_FAN_SPEED_4           = 0x93,
    PMBUS_CMD_READ_DUTY_CYCLE            = 0x94,
    PMBUS_CMD_READ_FREQUENCY             = 0x95,
    PMBUS_CMD_READ_POUT                  = 0x96,
    PMBUS_CMD_READ_PIN                   = 0x97,
    PMBUS_CMD_PMBUS_REVISION             = 0x98,
    PMBUS_CMD_MFR_ID                     = 0x99,
    PMBUS_CMD_MFR_MODEL                  = 0x9A,
    PMBUS_CMD_MFR_REVISION               = 0x9B,
    PMBUS_CMD_MFR_LOCATION               = 0x9C,
    PMBUS_CMD_MFR_DATE                   = 0x9D,
    PMBUS_CMD_MFR_SERIAL                 = 0x9E,
    PMBUS_CMD_MFR_VIN_MIN                = 0xA0,
    PMBUS_CMD_MFR_VIN_MAX                = 0xA1,
    PMBUS_CMD_MFR_IIN_MAX                = 0xA2,
    PMBUS_CMD_MFR_PIN_MAX                = 0xA3,
    PMBUS_CMD_MFR_VOUT_MIN               = 0xA4,
    PMBUS_CMD_MFR_VOUT_MAX               = 0xA5,
    PMBUS_CMD_MFR_IOUT_MAX               = 0xA6,
    PMBUS_CMD_MFR_POUT_MAX               = 0xA7,
    PMBUS_CMD_MFR_TAMBIENT_MAX           = 0xA8,
    PMBUS_CMD_MFR_TAMBIENT_MIN           = 0xA9,
    PMBUS_CMD_IC_DEVICE_ID               = 0xAD,
    PMBUS_CMD_IC_DEVICE_REV              = 0xAE,
    PMBUS_CMD_MFR_STATUS_0               = 0xB0,
    PMBUS_CMD_MFR_STATUS_1               = 0xB1,
    PMBUS_CMD_MFR_STATUS_2               = 0xB2,
    PMBUS_CMD_MFR_STATUS_3               = 0xB3,
    PMBUS_CMD_MFR_STATUS_4               = 0xB4,
    PMBUS_CMD_FIRST_BLACK_BOX_FAULT_INFO = 0xB5, /* USER_DATA_05 */
    PMBUS_CMD_LAST_BLACK_BOX_FAULT_INFO  = 0xB6, /* USER_DATA_06 */
    PMBUS_CMD_RAIL_PROFILE               = 0xB8, /* USER_DATA_08 */
    PMBUS_CMD_RAIL_STATE                 = 0xB9, /* USER_DATA_09 */
    PMBUS_CMD_USER_DATA_10               = 0xBA,
    PMBUS_CMD_USER_DATA_11               = 0xBB,
    PMBUS_CMD_USER_DATA_12               = 0xBC,
    PMBUS_CMD_USER_DATA_13               = 0xBD,
    PMBUS_CMD_USER_DATA_14               = 0xBE,
    PMBUS_CMD_USER_DATA_15               = 0xBF,

    /* UCD91320 manufacturer-specific commands (D0h-FDh) */
    PMBUS_CMD_FAULT_PIN_CONFIG           = 0xD0, /* MFR_SPECIFIC_00 */
    PMBUS_CMD_VOUT_CAL_MONITOR           = 0xD1, /* MFR_SPECIFIC_01 */
    PMBUS_CMD_SYSTEM_RESET_CONFIG        = 0xD2, /* MFR_SPECIFIC_02 */
    PMBUS_CMD_SYSTEM_WATCHDOG_CONFIG     = 0xD3, /* MFR_SPECIFIC_03 */
    PMBUS_CMD_SYSTEM_WATCHDOG_RESET      = 0xD4, /* MFR_SPECIFIC_04 */
    PMBUS_CMD_MONITOR_CONFIG             = 0xD5, /* MFR_SPECIFIC_05 */
    PMBUS_CMD_NUM_PAGES                  = 0xD6, /* MFR_SPECIFIC_06 */
    PMBUS_CMD_RUN_TIME_CLOCK             = 0xD7, /* MFR_SPECIFIC_07 */
    PMBUS_CMD_RUN_TIME_CLOCK_TRIM        = 0xD8, /* MFR_SPECIFIC_08 */
    PMBUS_CMD_USER_RAM_00                = 0xDA, /* MFR_SPECIFIC_10 */
    PMBUS_CMD_SOFT_RESET                 = 0xDB, /* MFR_SPECIFIC_11 */
    PMBUS_CMD_RESET_COUNT                = 0xDC, /* MFR_SPECIFIC_12 */
    PMBUS_CMD_PIN_SELECTED_RAIL_STATES   = 0xDD, /* MFR_SPECIFIC_13 */
    PMBUS_CMD_RESEQUENCE                 = 0xDE, /* MFR_SPECIFIC_14 */
    PMBUS_CMD_CONSTANTS                  = 0xDF, /* MFR_SPECIFIC_15 */
    PMBUS_CMD_PWM_SELECT                 = 0xE0, /* MFR_SPECIFIC_16 */
    PMBUS_CMD_PWM_CONFIG                 = 0xE1, /* MFR_SPECIFIC_17 */
    PMBUS_CMD_PARM_INFO                  = 0xE2, /* MFR_SPECIFIC_18 */
    PMBUS_CMD_PARM_VALUE                 = 0xE3, /* MFR_SPECIFIC_19 */
    PMBUS_CMD_TEMPERATURE_CAL_GAIN       = 0xE4, /* MFR_SPECIFIC_20 */
    PMBUS_CMD_TEMPERATURE_CAL_OFFSET     = 0xE5, /* MFR_SPECIFIC_21 */
    PMBUS_CMD_SET_BREAKPOINTS            = 0xE6, /* MFR_SPECIFIC_22 */
    PMBUS_CMD_DEBUG_CONTINUE             = 0xE7, /* MFR_SPECIFIC_23 */
    PMBUS_CMD_FAULT_RESPONSES            = 0xE9, /* MFR_SPECIFIC_25 */
    PMBUS_CMD_LOGGED_FAULTS              = 0xEA, /* MFR_SPECIFIC_26 */
    PMBUS_CMD_LOGGED_FAULT_DETAIL_INDEX  = 0xEB, /* MFR_SPECIFIC_27 */
    PMBUS_CMD_LOGGED_FAULT_DETAIL        = 0xEC, /* MFR_SPECIFIC_28 */
    PMBUS_CMD_LOGGED_PAGE_PEAKS          = 0xED, /* MFR_SPECIFIC_29 */
    PMBUS_CMD_LOGGED_COMMON_PEAKS        = 0xEE, /* MFR_SPECIFIC_30 */
    PMBUS_CMD_LOGGED_FAULT_DETAIL_ENABLES= 0xEF, /* MFR_SPECIFIC_31 */
    PMBUS_CMD_EXECUTE_FLASH              = 0xF0, /* MFR_SPECIFIC_32 */
    PMBUS_CMD_SECURITY                   = 0xF1, /* MFR_SPECIFIC_33 */
    PMBUS_CMD_SECURITY_BIT_MASK          = 0xF2, /* MFR_SPECIFIC_34 */
    PMBUS_CMD_MFR_STATUS                 = 0xF3, /* MFR_SPECIFIC_35, replaces STATUS_MFR_SPECIFIC */
    PMBUS_CMD_GPI_FAULT_RESPONSES        = 0xF4, /* MFR_SPECIFIC_36 */
    PMBUS_CMD_MARGIN_CONFIG              = 0xF5, /* MFR_SPECIFIC_37 */
    PMBUS_CMD_SEQ_CONFIG                 = 0xF6, /* MFR_SPECIFIC_38 */
    PMBUS_CMD_GPO_CONFIG_INDEX           = 0xF7, /* MFR_SPECIFIC_39 */
    PMBUS_CMD_GPO_CONFIG                 = 0xF8, /* MFR_SPECIFIC_40 */
    PMBUS_CMD_GPI_CONFIG                 = 0xF9, /* MFR_SPECIFIC_41 */
    PMBUS_CMD_GPIO_SELECT                = 0xFA, /* MFR_SPECIFIC_42 */
    PMBUS_CMD_GPIO_CONFIG                = 0xFB, /* MFR_SPECIFIC_43 */
    PMBUS_CMD_MISC_CONFIG                = 0xFC, /* MFR_SPECIFIC_44 */
    PMBUS_CMD_DEVICE_ID                  = 0xFD, /* MFR_SPECIFIC_45 */

    /* PMBus 1.3 extended-command escape codes (second command byte follows) */
    PMBUS_CMD_MFR_SPECIFIC_EXTENDED_COMMAND = 0xFE,
    PMBUS_CMD_PMBUS_EXTENDED_COMMAND        = 0xFF
} pmbus_command_t;

/* PUBLISHED functions */
void PMbus_write_execute(pmbus_command_t command, uint8_t *data, uint16_t data_len);

#endif /* PMBUS_COMMANDS_H_ */
