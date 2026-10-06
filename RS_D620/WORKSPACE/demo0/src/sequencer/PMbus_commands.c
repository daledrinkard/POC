/*
            PMBUS_COMMANDS
*/
#include "PMbus_commands.h"
#include "sequencer/power_module.h"

/* USER */
/*
     These functions are called in an interrupt context.

*/
#define PWRSEQ_UPDATE(_a_,_b_,_c_)                                                                      \
            if (_a_) {pwr_seq_update_cfg((uint8_t *) _b_,data,data_len); return data_len;}             \
            else    {pwr_seq_update_cfg(data, (uint8_t *) _b_,_c_);                                   \
                     return _c_;}

int PMbus_execute(pmbus_command_t command, uint8_t *data, uint16_t data_len, uint8_t RW) /* 0=read 1=write*/
{
    (void) data;
    (void) data_len;
    switch (command)
    {
        /* core PMBus commands (00h-CFh) */
        case PMBUS_CMD_PAGE: /* 0x00 */ PWRSEQ_UPDATE(RW,&PowerController.ctrl->page,1);
        case PMBUS_CMD_OPERATION: /* R/W */ 
            break;
        case PMBUS_CMD_ON_OFF_CONFIG:              /* USER CODE */ 
            break;
        case PMBUS_CMD_CLEAR_FAULTS:               /* USER CODE */ 
            break;
        case PMBUS_CMD_STORE_DEFAULT_ALL:          /* USER CODE */ 
            pwr_seq_store_all();
            break;
        case PMBUS_CMD_CAPABILITY:                 /* USER CODE */ 
            break;
        case PMBUS_CMD_VOUT_MODE:                  /* USER CODE */ 
            break;
        case PMBUS_CMD_VOUT_COMMAND:               /* USER CODE */ 
            break;


            //        case PMBUS_CMD_RESTORE_DEFAULT_ALL:        /* USER CODE */ break;
//        case PMBUS_CMD_STORE_DEFAULT_CODE:         /* USER CODE */ break;
//        case PMBUS_CMD_RESTORE_DEFAULT_CODE:       /* USER CODE */ break;
//        case PMBUS_CMD_STORE_USER_ALL:             /* USER CODE */ break;
//        case PMBUS_CMD_RESTORE_USER_ALL:           /* USER CODE */ break;
//        case PMBUS_CMD_STORE_USER_CODE:            /* USER CODE */ break;
//        case PMBUS_CMD_RESTORE_USER_CODE:          /* USER CODE */ break;
#if 0 /* not implemented yet */            
        case PMBUS_CMD_PHASE:                      /* USER CODE */ break;
        case PMBUS_CMD_PASSKEY:                    /* USER CODE */ break;
        case PMBUS_CMD_ACCESS_CONTROL:             /* USER CODE */ break;
        case PMBUS_CMD_WRITE_PROTECT:              /* USER CODE */ break;
        case PMBUS_CMD_QUERY:                      /* USER CODE */ break;
        case PMBUS_CMD_SMBALERT_MASK:              /* USER CODE */ break;
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
        case PMBUS_CMD_FAULT_PIN_CONFIG: /* 0xD0 */ //@@@ cannot use PWRSEQ_UPDATE macro because of alignment issues
            if (RW) {pwr_seq_store_fault_config(data,data_len ); return data_len;}
            else    { return pwr_seq_read_fault_config(data); }
            break;
        case PMBUS_CMD_VOUT_CAL_MONITOR:       /* 0xD1 */ /* USER CODE */ break;
        case PMBUS_CMD_SYSTEM_RESET_CONFIG:    /* 0xD2 */ PWRSEQ_UPDATE(RW,&PowerController.cfg->reset_config,15);
        case PMBUS_CMD_SYSTEM_WATCHDOG_CONFIG: /* 0xD3 */ PWRSEQ_UPDATE(RW,&PowerController.cfg->watchdog_config,6);
        case PMBUS_CMD_SYSTEM_WATCHDOG_RESET:  /* 0xD4 */ /* USER CODE */ 
            //@@@ this commands needs to force a WDI condition...
            return 0;
        case PMBUS_CMD_MONITOR_CONFIG:        /* 0xD5 */ PWRSEQ_UPDATE(RW,&PowerController.cfg->monitor,sizeof(power_sequencer_monitor_t));
        case PMBUS_CMD_NUM_PAGES:             /* 0xD6 */      /* USER CODE */ 
            //@@@ this is a read only command and returns the number of active pages
            return 0;
        case PMBUS_CMD_RUN_TIME_CLOCK:        /* 0xD7 */ PWRSEQ_UPDATE(RW,&PowerController.cfg->RTC,8);
        case PMBUS_CMD_RUN_TIME_CLOCK_TRIM:   /* 0xD8 */ PWRSEQ_UPDATE(RW,&PowerController.cfg->RTC_trim,4);
//      case (pmbus_command_t) 0xD9: break;   /* 0xD9 */ does not exist
        case PMBUS_CMD_USER_RAM_00:           /* 0xDA */ PWRSEQ_UPDATE(RW,&PowerController.ctrl->ram_00,1);
        case PMBUS_CMD_SOFT_RESET:            /* 0xDB */
            //@@@@ this command is write only and causes the firmware to reset
            return 0;
        case PMBUS_CMD_RESET_COUNT:           /* 0xDC */      /* USER CODE */ break; //@@@ related to brownout feature
        case PMBUS_CMD_PIN_SELECTED_RAIL_STATES:   /* 0xDD */ 
             if (RW) {pwr_seq_update_railstate(data,data_len); return data_len;}
             else    {return pwr_seq_read_railstate(data);}
        case PMBUS_CMD_RESEQUENCE:            /* 0xDE */ PWRSEQ_UPDATE(RW,&PowerController.cfg->resequence,sizeof(uint32_t));
        case PMBUS_CMD_CONSTANTS:             /* 0xDF */
             if (RW) {return 0;} /* Read Only */
             else PWRSEQ_UPDATE(0,(uint8_t *) PowerController.CONSTANTS,8);

        case PMBUS_CMD_PWM_SELECT:            /* 0xE0 */      /* USER CODE */ break;
        case PMBUS_CMD_PWM_CONFIG:            /* 0xE1 */      /* USER CODE */ break;
        case PMBUS_CMD_PARM_INFO:             /* 0xE2 */      /* USER CODE */ break;
        case PMBUS_CMD_PARM_VALUE:            /* 0xE3 */      /* USER CODE */ break;
        case PMBUS_CMD_TEMPERATURE_CAL_GAIN:   /* 0xE4 */     /* USER CODE */ break;
        case PMBUS_CMD_TEMPERATURE_CAL_OFFSET: /* 0xE5 */     /* USER CODE */ break;
        case PMBUS_CMD_SET_BREAKPOINTS:        /* 0xE6 */     /* USER CODE */ break;
        case PMBUS_CMD_DEBUG_CONTINUE:         /* 0xE7 */     /* USER CODE */ break;
//        case 0xE8; break;
        case PMBUS_CMD_FAULT_RESPONSES:        /* 0xE9 */      /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULTS:          /* 0xEA */      /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULT_DETAIL_INDEX: /* 0xEB */   /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULT_DETAIL:    /* 0xEC */      /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_PAGE_PEAKS:      /* 0xED */      /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_COMMON_PEAKS:    /* 0xEE */      /* USER CODE */ break;
        case PMBUS_CMD_LOGGED_FAULT_DETAIL_ENABLES: /* 0xEF */ /* USER CODE */ break;



        case PMBUS_CMD_EXECUTE_FLASH:            /* 0xF0 */  /* USER CODE */ break;
        case PMBUS_CMD_SECURITY:                 /* 0xF1 */  /* USER CODE */ break;
        case PMBUS_CMD_SECURITY_BIT_MASK:        /* 0xF2 */  /* USER CODE */ break;
        case PMBUS_CMD_MFR_STATUS:               /* 0xF3 */  /* USER CODE */ break;
        case PMBUS_CMD_GPI_FAULT_RESPONSES:      /* 0xF4 */  /* USER CODE */ break;
        case PMBUS_CMD_MARGIN_CONFIG:            /* 0xF5 */  /* USER CODE */ break;

        case PMBUS_CMD_SEQ_CONFIG:               /* 0xF6 */             /* PAGE */ 
            if (RW) {pwr_seq_update_seqcfg(PowerController.ctrl->page,data,data_len); return data_len;}
            else    {return pwr_seq_read_seqcfg(PowerController.ctrl->page,data);}
        case PMBUS_CMD_GPO_CONFIG_INDEX:         /* 0xF7 */
            if (RW) PowerController.ctrl->gpo_index = *data;
            else    *data = PowerController.ctrl->gpo_index;
            return 1;
        case PMBUS_CMD_GPO_CONFIG:                /* 0xF8 */  
            PWRSEQ_UPDATE(RW,&PowerController.cfg->GPO_config[PowerController.ctrl->gpo_index],sizeof(power_controller_GPOCFG_t));
        case PMBUS_CMD_GPI_CONFIG:                 /* 0xF9 */              /* USER CODE */ 
            PWRSEQ_UPDATE(RW,&PowerController.cfg->GPI_config,sizeof(power_controller_GPICFG_t));
            break;
        case PMBUS_CMD_GPIO_SELECT:              /* 0xFA */  /* USER CODE */ break;
            PWRSEQ_UPDATE(RW,&PowerController.ctrl->GPIO_select,1);
        case PMBUS_CMD_GPIO_CONFIG:              /* 0xFB */  /* USER CODE */ break;
            if (RW) 
            {
                 pwr_seq_update_cfg((uint8_t *) &PowerController.ctrl->GPIO_config,data,data_len); 
                 return 1;
            }             
            else    
            {
                pwr_seq_update_cfg(data, (uint8_t *) &PowerController.ctrl->GPIO_config,1);                                   \
                return 1;
            }

        case PMBUS_CMD_MISC_CONFIG:              /* 0xFC */  PWRSEQ_UPDATE(RW,&PowerController.cfg->MSCCFG,8);
        case PMBUS_CMD_DEVICE_ID:                /* 0xFD */  /* USER CODE */ break;
        case PMBUS_CMD_MFR_SPECIFIC_EXTENDED_COMMAND: /* 0xFE */   /* USER CODE */ break;
        case PMBUS_CMD_PMBUS_EXTENDED_COMMAND:        /* 0xFF */   /* USER CODE */ break;

        default:
            /* USER CODE: unrecognized command */
            break;
    }
}
