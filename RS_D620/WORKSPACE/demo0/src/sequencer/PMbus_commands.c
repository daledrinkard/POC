/*
            PMBUS_COMMANDS
*/
#include "PMbus_commands.h"
#include "sequencer/power_module.h"

/* USER */

void PMbus_write_execute(pmbus_command_t command, uint8_t *data, uint16_t data_len)
{
    (void) data;
    (void) data_len;
    switch (command)
    {
        /* core PMBus commands (00h-CFh) */
        case PMBUS_CMD_PAGE:
            PowerController.ctrl->page = *data;
            break;
        case PMBUS_CMD_STORE_DEFAULT_ALL:          /* USER CODE */ 
            //@@@ 
            pwr_seq_store_all();
            break;
//        case PMBUS_CMD_RESTORE_DEFAULT_ALL:        /* USER CODE */ break;
//        case PMBUS_CMD_STORE_DEFAULT_CODE:         /* USER CODE */ break;
//        case PMBUS_CMD_RESTORE_DEFAULT_CODE:       /* USER CODE */ break;
//        case PMBUS_CMD_STORE_USER_ALL:             /* USER CODE */ break;
//        case PMBUS_CMD_RESTORE_USER_ALL:           /* USER CODE */ break;
//        case PMBUS_CMD_STORE_USER_CODE:            /* USER CODE */ break;
//        case PMBUS_CMD_RESTORE_USER_CODE:          /* USER CODE */ break;
#if 0 /* not implemented yet */            
        case PMBUS_CMD_OPERATION:                  /* USER CODE */ break;
        case PMBUS_CMD_ON_OFF_CONFIG:              /* USER CODE */ break;
        case PMBUS_CMD_CLEAR_FAULTS:               /* USER CODE */ break;
        case PMBUS_CMD_PHASE:                      /* USER CODE */ break;
        case PMBUS_CMD_PASSKEY:                    /* USER CODE */ break;
        case PMBUS_CMD_ACCESS_CONTROL:             /* USER CODE */ break;
        case PMBUS_CMD_WRITE_PROTECT:              /* USER CODE */ break;
        case PMBUS_CMD_CAPABILITY:                 /* USER CODE */ break;
        case PMBUS_CMD_QUERY:                      /* USER CODE */ break;
        case PMBUS_CMD_SMBALERT_MASK:              /* USER CODE */ break;
        case PMBUS_CMD_VOUT_MODE:                  /* USER CODE */ break;
        case PMBUS_CMD_VOUT_COMMAND:               /* USER CODE */ break;
        case PMBUS_CMD_VOUT_TRIM:                  /* USER CODE */ break;
        case PMBUS_CMD_VOUT_CAL_OFFSET:            /* USER CODE */ break;
        case PMBUS_CMD_VOUT_MAX:                   /* USER CODE */ break;
        case PMBUS_CMD_VOUT_MARGIN_HIGH:           /* USER CODE */ break;
        case PMBUS_CMD_VOUT_MARGIN_LOW:            /* USER CODE */ break;
        case PMBUS_CMD_VOUT_TRANSITION_RATE:       /* USER CODE */ break;
        case PMBUS_CMD_VOUT_DROOP:                 /* USER CODE */ break;
        case PMBUS_CMD_VOUT_SCALE_LOOP:            /* USER CODE */ break;
        case PMBUS_CMD_VOUT_SCALE_MONITOR:         /* USER CODE */ break;
        case PMBUS_CMD_COEFFICIENTS:               /* USER CODE */ break;
        case PMBUS_CMD_POUT_MAX:                   /* USER CODE */ break;
        case PMBUS_CMD_MAX_DUTY:                   /* USER CODE */ break;
        case PMBUS_CMD_FREQUENCY_SWITCH:           /* USER CODE */ break;
        case PMBUS_CMD_VIN_ON:                     /* USER CODE */ break;
        case PMBUS_CMD_VIN_OFF:                    /* USER CODE */ break;
        case PMBUS_CMD_INTERLEAVE:                 /* USER CODE */ break;
        case PMBUS_CMD_IOUT_CAL_GAIN:              /* USER CODE */ break;
        case PMBUS_CMD_IOUT_CAL_OFFSET:            /* USER CODE */ break;
        case PMBUS_CMD_FAN_CONFIG_1_2:             /* USER CODE */ break;
        case PMBUS_CMD_FAN_COMMAND_1:              /* USER CODE */ break;
        case PMBUS_CMD_FAN_COMMAND_2:              /* USER CODE */ break;
        case PMBUS_CMD_FAN_CONFIG_3_4:             /* USER CODE */ break;
        case PMBUS_CMD_FAN_COMMAND_3:              /* USER CODE */ break;
        case PMBUS_CMD_FAN_COMMAND_4:              /* USER CODE */ break;
        case PMBUS_CMD_VOUT_OV_FAULT_LIMIT:        /* USER CODE */ break;
        case PMBUS_CMD_VOUT_OV_FAULT_RESPONSE:     /* USER CODE */ break;
        case PMBUS_CMD_VOUT_OV_WARN_LIMIT:         /* USER CODE */ break;
        case PMBUS_CMD_VOUT_UV_WARN_LIMIT:         /* USER CODE */ break;
        case PMBUS_CMD_VOUT_UV_FAULT_LIMIT:        /* USER CODE */ break;
        case PMBUS_CMD_VOUT_UV_FAULT_RESPONSE:     /* USER CODE */ break;
        case PMBUS_CMD_IOUT_OC_FAULT_LIMIT:        /* USER CODE */ break;
        case PMBUS_CMD_IOUT_OC_FAULT_RESPONSE:     /* USER CODE */ break;
        case PMBUS_CMD_IOUT_OC_LV_FAULT_LIMIT:     /* USER CODE */ break;
        case PMBUS_CMD_IOUT_OC_LV_FAULT_RESPONSE:  /* USER CODE */ break;
        case PMBUS_CMD_IOUT_OC_WARN_LIMIT:         /* USER CODE */ break;
        case PMBUS_CMD_IOUT_UC_FAULT_LIMIT:        /* USER CODE */ break;
        case PMBUS_CMD_IOUT_UC_FAULT_RESPONSE:     /* USER CODE */ break;
        case PMBUS_CMD_OT_FAULT_LIMIT:             /* USER CODE */ break;
        case PMBUS_CMD_OT_FAULT_RESPONSE:          /* USER CODE */ break;
        case PMBUS_CMD_OT_WARN_LIMIT:              /* USER CODE */ break;
        case PMBUS_CMD_UT_WARN_LIMIT:              /* USER CODE */ break;
        case PMBUS_CMD_UT_FAULT_LIMIT:             /* USER CODE */ break;
        case PMBUS_CMD_UT_FAULT_RESPONSE:          /* USER CODE */ break;
        case PMBUS_CMD_VIN_OV_FAULT_LIMIT:         /* USER CODE */ break;
        case PMBUS_CMD_VIN_OV_FAULT_RESPONSE:      /* USER CODE */ break;
        case PMBUS_CMD_VIN_OV_WARN_LIMIT:          /* USER CODE */ break;
        case PMBUS_CMD_VIN_UV_WARN_LIMIT:          /* USER CODE */ break;
        case PMBUS_CMD_VIN_UV_FAULT_LIMIT:         /* USER CODE */ break;
        case PMBUS_CMD_VIN_UV_FAULT_RESPONSE:      /* USER CODE */ break;
        case PMBUS_CMD_IIN_OC_FAULT_LIMIT:         /* USER CODE */ break;
        case PMBUS_CMD_IIN_OC_FAULT_RESPONSE:      /* USER CODE */ break;
        case PMBUS_CMD_IIN_OC_WARN_LIMIT:          /* USER CODE */ break;
        case PMBUS_CMD_POWER_GOOD_ON:              /* USER CODE */ break;
        case PMBUS_CMD_POWER_GOOD_OFF:             /* USER CODE */ break;
        case PMBUS_CMD_TON_DELAY:                  /* USER CODE */ break;
        case PMBUS_CMD_TON_RISE:                   /* USER CODE */ break;
        case PMBUS_CMD_TON_MAX_FAULT_LIMIT:        /* USER CODE */ break;
        case PMBUS_CMD_TON_MAX_FAULT_RESPONSE:     /* USER CODE */ break;
        case PMBUS_CMD_TOFF_DELAY:                 /* USER CODE */ break;
        case PMBUS_CMD_TOFF_FALL:                  /* USER CODE */ break;
        case PMBUS_CMD_TOFF_MAX_WARN_LIMIT:        /* USER CODE */ break;
        case PMBUS_CMD_POUT_OP_FAULT_LIMIT:        /* USER CODE */ break;
        case PMBUS_CMD_POUT_OP_FAULT_RESPONSE:     /* USER CODE */ break;
        case PMBUS_CMD_POUT_OP_WARN_LIMIT:         /* USER CODE */ break;
        case PMBUS_CMD_PIN_OP_WARN_LIMIT:          /* USER CODE */ break;
        case PMBUS_CMD_STATUS_BYTE:                /* USER CODE */ break;
        case PMBUS_CMD_STATUS_WORD:                /* USER CODE */ break;
        case PMBUS_CMD_STATUS_VOUT:                /* USER CODE */ break;
        case PMBUS_CMD_STATUS_IOUT:                /* USER CODE */ break;
        case PMBUS_CMD_STATUS_INPUT:               /* USER CODE */ break;
        case PMBUS_CMD_STATUS_TEMPERATURE:         /* USER CODE */ break;
        case PMBUS_CMD_STATUS_CML:                 /* USER CODE */ break;
        case PMBUS_CMD_STATUS_OTHER:               /* USER CODE */ break;
        case PMBUS_CMD_STATUS_MFR_SPECIFIC:        /* USER CODE */ break;
        case PMBUS_CMD_STATUS_FANS_1_2:            /* USER CODE */ break;
        case PMBUS_CMD_STATUS_FANS_3_4:            /* USER CODE */ break;
        case PMBUS_CMD_READ_VIN:                   /* USER CODE */ break;
        case PMBUS_CMD_READ_IIN:                   /* USER CODE */ break;
        case PMBUS_CMD_READ_VCAP:                  /* USER CODE */ break;
        case PMBUS_CMD_READ_VOUT:                  /* USER CODE */ break;
        case PMBUS_CMD_READ_IOUT:                  /* USER CODE */ break;
        case PMBUS_CMD_READ_TEMPERATURE_1:         /* USER CODE */ break;
        case PMBUS_CMD_READ_TEMPERATURE_2:         /* USER CODE */ break;
        case PMBUS_CMD_READ_TEMPERATURE_3:         /* USER CODE */ break;
        case PMBUS_CMD_READ_FAN_SPEED_1:           /* USER CODE */ break;
        case PMBUS_CMD_READ_FAN_SPEED_2:           /* USER CODE */ break;
        case PMBUS_CMD_READ_FAN_SPEED_3:           /* USER CODE */ break;
        case PMBUS_CMD_READ_FAN_SPEED_4:           /* USER CODE */ break;
        case PMBUS_CMD_READ_DUTY_CYCLE:            /* USER CODE */ break;
        case PMBUS_CMD_READ_FREQUENCY:             /* USER CODE */ break;
        case PMBUS_CMD_READ_POUT:                  /* USER CODE */ break;
        case PMBUS_CMD_READ_PIN:                   /* USER CODE */ break;
        case PMBUS_CMD_PMBUS_REVISION:             /* USER CODE */ break;
        case PMBUS_CMD_MFR_ID:                     /* USER CODE */ break;
        case PMBUS_CMD_MFR_MODEL:                  /* USER CODE */ break;
        case PMBUS_CMD_MFR_REVISION:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_LOCATION:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_DATE:                   /* USER CODE */ break;
        case PMBUS_CMD_MFR_SERIAL:                 /* USER CODE */ break;
        case PMBUS_CMD_MFR_VIN_MIN:                /* USER CODE */ break;
        case PMBUS_CMD_MFR_VIN_MAX:                /* USER CODE */ break;
        case PMBUS_CMD_MFR_IIN_MAX:                /* USER CODE */ break;
        case PMBUS_CMD_MFR_PIN_MAX:                /* USER CODE */ break;
        case PMBUS_CMD_MFR_VOUT_MIN:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_VOUT_MAX:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_IOUT_MAX:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_POUT_MAX:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_TAMBIENT_MAX:           /* USER CODE */ break;
        case PMBUS_CMD_MFR_TAMBIENT_MIN:           /* USER CODE */ break;
        case PMBUS_CMD_IC_DEVICE_ID:               /* USER CODE */ break;
        case PMBUS_CMD_IC_DEVICE_REV:              /* USER CODE */ break;
        case PMBUS_CMD_MFR_STATUS_0:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_STATUS_1:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_STATUS_2:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_STATUS_3:               /* USER CODE */ break;
        case PMBUS_CMD_MFR_STATUS_4:               /* USER CODE */ break;
        case PMBUS_CMD_FIRST_BLACK_BOX_FAULT_INFO: /* USER CODE */ break;
        case PMBUS_CMD_LAST_BLACK_BOX_FAULT_INFO:  /* USER CODE */ break;
        case PMBUS_CMD_RAIL_PROFILE:               /* USER CODE */ break;
        case PMBUS_CMD_RAIL_STATE:                 /* USER CODE */ break;
        case PMBUS_CMD_USER_DATA_10:               /* USER CODE */ break;
        case PMBUS_CMD_USER_DATA_11:               /* USER CODE */ break;
        case PMBUS_CMD_USER_DATA_12:               /* USER CODE */ break;
        case PMBUS_CMD_USER_DATA_13:               /* USER CODE */ break;
        case PMBUS_CMD_USER_DATA_14:               /* USER CODE */ break;
        case PMBUS_CMD_USER_DATA_15:               /* USER CODE */ break;
#endif
        /* UCD91320 manufacturer-specific commands (D0h-FDh) */
        /* must be set along with the GPI_CONFIG may change things... let's see */
        case PMBUS_CMD_FAULT_PIN_CONFIG:           /* USER CODE */ 
            pwr_seq_update_fault(data,data_len );
            break;
        case PMBUS_CMD_MONITOR_CONFIG:             /* USER CODE */ 
            // 1 byte per monitor pin.  There are 32 monitor pins
            // 7:5 is encoded: 0=no monitor, 1=Analog, 2=temp, 3=current(NS), 4=voltage compare(NS), 5= input voltage(NS) 6=Digital monitor
            pwr_seq_update_monitor(data,data_len);
            break;
        case PMBUS_CMD_SEQ_CONFIG:                 /* USER CODE */ 
            break;
        case PMBUS_CMD_RESEQUENCE:                 /* USER CODE */ 
            pwr_seq_update_cfg((uint8_t*) &PowerController.cfg->resequence,data,data_len);
            break;
        case PMBUS_CMD_GPO_CONFIG_INDEX:           /* USER CODE */ 
            PowerController.ctrl->gpo_index = *data;
            break;
        case PMBUS_CMD_GPO_CONFIG:                 /* USER CODE */ 
        
            break;
        case PMBUS_CMD_GPI_CONFIG:                 /* USER CODE */ 
            pwr_seq_update_cfg((uint8_t *) &PowerController.cfg->GPI_config,data,data_len);
            break;

#if 0 /* not implemented yet*/        
        case PMBUS_CMD_VOUT_CAL_MONITOR:           /* USER CODE */ break;
        case PMBUS_CMD_SYSTEM_RESET_CONFIG:        /* USER CODE */ break;
        case PMBUS_CMD_SYSTEM_WATCHDOG_CONFIG:     /* USER CODE */ break;
        case PMBUS_CMD_SYSTEM_WATCHDOG_RESET:      /* USER CODE */ break;
        case PMBUS_CMD_NUM_PAGES:                  /* USER CODE */ break;
        case PMBUS_CMD_RUN_TIME_CLOCK:             /* USER CODE */ break;
        case PMBUS_CMD_RUN_TIME_CLOCK_TRIM:        /* USER CODE */ break;
        case PMBUS_CMD_USER_RAM_00:                /* USER CODE */ break;
        case PMBUS_CMD_SOFT_RESET:                 /* USER CODE */ break;
        case PMBUS_CMD_RESET_COUNT:                /* USER CODE */ break;
        case PMBUS_CMD_PIN_SELECTED_RAIL_STATES:   /* USER CODE */ break;
        case PMBUS_CMD_CONSTANTS:                  /* USER CODE */ break;
        case PMBUS_CMD_PWM_SELECT:                 /* USER CODE */ break;
        case PMBUS_CMD_PWM_CONFIG:                 /* USER CODE */ break;
        case PMBUS_CMD_PARM_INFO:                  /* USER CODE */ break;
        case PMBUS_CMD_PARM_VALUE:                 /* USER CODE */ break;
        case PMBUS_CMD_TEMPERATURE_CAL_GAIN:       /* USER CODE */ break;
        case PMBUS_CMD_TEMPERATURE_CAL_OFFSET:     /* USER CODE */ break;
        case PMBUS_CMD_SET_BREAKPOINTS:            /* USER CODE */ break;
        case PMBUS_CMD_DEBUG_CONTINUE:              /* USER CODE */ break;
        case PMBUS_CMD_FAULT_RESPONSES:            /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULTS:              /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULT_DETAIL_INDEX:  /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULT_DETAIL:        /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_PAGE_PEAKS:          /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_COMMON_PEAKS:        /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULT_DETAIL_ENABLES:/* USER CODE */ break;
        case PMBUS_CMD_EXECUTE_FLASH:              /* USER CODE */ break;
        case PMBUS_CMD_SECURITY:                   /* USER CODE */ break;
        case PMBUS_CMD_SECURITY_BIT_MASK:          /* USER CODE */ break;
        case PMBUS_CMD_MFR_STATUS:                 /* USER CODE */ break;
        case PMBUS_CMD_GPI_FAULT_RESPONSES:        /* USER CODE */ break;
        case PMBUS_CMD_MARGIN_CONFIG:              /* USER CODE */ break;
        case PMBUS_CMD_GPIO_SELECT:                /* USER CODE */ break;
        case PMBUS_CMD_GPIO_CONFIG:                /* USER CODE */ break;
        case PMBUS_CMD_MISC_CONFIG:                /* USER CODE */ break;
        case PMBUS_CMD_DEVICE_ID:                  /* USER CODE */ break;

        /* PMBus 1.3 extended-command escape codes */
        case PMBUS_CMD_MFR_SPECIFIC_EXTENDED_COMMAND: /* USER CODE */ break;
        case PMBUS_CMD_PMBUS_EXTENDED_COMMAND:        /* USER CODE */ break;
#endif
        default:
            /* USER CODE: unrecognized command */
            break;
    }
}
