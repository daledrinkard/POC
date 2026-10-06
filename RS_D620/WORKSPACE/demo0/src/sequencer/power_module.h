/*
 *     POWER_MODULE.H
 *
 * Power sequencer application header file.
 *
 * Defines the power_controller_t object used by the sequencer app to model a
 * multi-rail power sequencer/system manager, in the spirit of the
 * TI UCD91320 32-Rail PMBus Power Sequencer and System Manager
 * (see documents/ucd91320.pdf). Field/feature choices below are traced back
 * to that datasheet in the comments; sizes are scaled down from the part's
 * 32-rail/100-entry-log numbers to something reasonable for this MCU target.
 *
 */

#ifndef POWER_MODULE_H_
#define POWER_MODULE_H_
#include "application_common.h"  // IWYU pragma: keep (provides BSP types/macros for downstream includers)
/* USER INCLUDE */

/* PWR_ prefix keeps these distinct from the CPAN CPAN_* macros */
#define PWR_MAX_RAILS        (32) /* ucd91320: up to 24 analog/digital + 8 digital-only (32 total) rails */
#define PWR_MAX_RAIL_GROUPS  (8)  /* ucd91320 6.1: up to 8 GPI-selected pin-selected rail states (ACPI-style) */
#define PWR_FAULT_LOG_DEPTH  (8)  /* ucd91320 1/6.3.1: single-event fault log (100 entries) + Black Box Fault Log */
#define PWR_MAX_CASCADE      (4)  /* ucd91320 1: cascade up to 4 devices to sequence up to 128 rails */
#define PWR_MAX_MONITOR      (24)
#define PWR_MAX_DMON         (8)
#define PWR_MAX_EN           (32)
#define PWR_MAX_GPIO         (8)
#define PWR_MAX_MAR          (16)
#define PWR_IO_DATA_WORDS (PWR_MAX_MONITOR + PWR_MAX_DMON + PWR_MAX_EN + PWR_MAX_MAR + PWR_MAX_GPIO)

#define PWR_MAX_FAULT_GROUP  (4)

#define PWR_MON_TYPE_NONE         (0x00)
#define PWR_MON_TYPE_ANALOG       (0x01 << 5) /* see section 26.7 in the UCD commands manual */
#define PWR_MON_TYPE_TEMP         (0x02 << 5)
#define PWR_MON_TYPE_CURRENT      (0x03 << 5)
#define PWR_MON_TYPE_VOLT_COMPARE (0x04 << 5)
#define PWR_MON_TYPE_VOLT_INPUT   (0x05 << 5)
#define PWR_MON_TYPE_DIGITAL      (0x06 << 5)

#define PWR_MON_GPI_POLARITY_BIT  (0x0001 << 10)
#define PWR_MON_GPI_ACTIVE_LOW    (0)
#define PWR_MON_GPI_ACTIVE_HIGH   (0x0001 << 10)
#define PWR_MON_GPI_MODE_UNUSED   (0x0000 << 9)
#define PWR_MON_GPI_MODE_INPUT    (0x0001 << 9)
#define PWR_MON_GPI_MODE_ACTIVE   (0x0002 << 9)
#define PWR_MON_GPI_MODE_OD       (0x0003 << 9)
/* USER DEFINE */

#define PWR_FLAG_ENABLE     (0x00000001)
#define PWR_FLAG_READBACK   (0x00000002) /* this flag enables the main loop to poll the comm during a read operation */
/* These are RA specific coding */
#define PWR_PIN_p_PFS(_a_) (uint32_t*) PowerController.GPIO->pin[_a_]
#if 0
#define PWR_PIN_ASSERT_LOW(_a_) *((uint32_t*) PowerController.GPIO->pin[_a_]) |= (uint32_t)  0x00000004;  /* Set PDR bit */ \
                                *((uint32_t*) PowerController.GPIO->pin[_a_]) &= (uint32_t) ~0x00000001;  /* clear PODR bit */
#define PWR_PIN_ASSERT_HI(_a_)  *((uint32_t*) PowerController.GPIO->pin[_a_]) |= (uint32_t)  0x00000004;  /* Set PDR bit */ \
                                *((uint32_t*) PowerController.GPIO->pin[_a_]) |= (uint32_t)  0x00000001;  /* Set PODR bit */
#define PWR_PIN_ASSERT_OPEN(_a_)  *((uint32_t*) PowerController.GPIO->pin[_a_]) &= (uint32_t)  ~0x00000004;  /* clear PDR bit */ \
                                  *((uint32_t*) PowerController.GPIO->pin[_a_]) |= (uint32_t)  ~0x00000001;  /* clear PODR bit */
#endif

#define PWR_PIN_ASSERT_LOW(_a_)   *(PWR_PIN_p_PFS(_a_)) |= (uint32_t)  0x00000004;  /* Set PDR bit */ \
                                  *(PWR_PIN_p_PFS(_a_)) &= (uint32_t) ~0x00000001;  /* clear PODR bit */
#define PWR_PIN_ASSERT_HI(_a_)    *(PWR_PIN_p_PFS(_a_)) |= (uint32_t)  0x00000004;  /* Set PDR bit */ \
                                  *(PWR_PIN_p_PFS(_a_)) |= (uint32_t)  0x00000001;  /* Set PODR bit */
#define PWR_PIN_ASSERT_OPEN(_a_)  *(PWR_PIN_p_PFS(_a_)) &= (uint32_t)  ~0x00000004;  /* clear PDR bit */ \
                                  *(PWR_PIN_p_PFS(_a_)) |= (uint32_t)  ~0x00000001;  /* clear PODR bit */
#define PWR_PIN_CFG_ANALOG(_a_)   *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00018000;  /* Set PMR and ASEL */ 
#define PWR_PIN_CFG_INPUT(_a_)    *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00000000;  /* input no pull up */ 
#define PWR_PIN_CFG_OUTPUT0(_a_)  *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00000004;  /* output drive low */ 
#define PWR_PIN_CFG_OUTPUT1(_a_)  *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00000005;  /* output drive hi */ 
#define PWR_PIN_CFG_PULL(_a_)     *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00000010;  /* input pull up */ 
#define PWR_PIN_CFG_OD1(_a_)      *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00000040;  /* OD initially pulled hi input */ 
#define PWR_PIN_CFG_OD0(_a_)      *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00000044;  /* OD initially driven low */ 

#define PWR_PIN_CFG_DVM(_a_)      *(PWR_PIN_p_PFS(_a_))  = (uint32_t)  0x00004000;  /* Set ISEL  */ 

/* how a rail's output is measured (ucd91320 4: MONx = analog or digital monitor, or GPIO) */
typedef enum power_monitor_type_e {
    PWR_MON_NONE,
    PWR_MON_ANALOG,   /* AMONx: 0V-3.3V analog rail monitor */
    PWR_MON_DIGITAL   /* DMONx: digital (power-good) rail monitor */
} power_monitor_type_t;

/* per-rail sequencing state (ucd91320 6.1 Overview: power-ON/OFF sequencing, margining) */
typedef enum power_rail_state_e {
    PWR_RAIL_OFF,
    PWR_RAIL_SEQ_ON,     /* turn-on delay in progress (ENx asserted, waiting for MONx good) */
    PWR_RAIL_ON,
    PWR_RAIL_MARGIN_HIGH,/* closed-loop margin/trim pushed high (MARx, up to 16 rails) */
    PWR_RAIL_MARGIN_LOW, /* closed-loop margin/trim pushed low (MARx, up to 16 rails) */
    PWR_RAIL_SEQ_OFF,    /* turn-off delay in progress */
    PWR_RAIL_FAULT
} power_rail_state_t;

/* fault classification (ucd91320 1 Features: "Monitor and respond to OV, UV, time-out, and GPI-triggered faults") */
typedef enum power_fault_type_e {
    PWR_FAULT_NONE,
    PWR_FAULT_OV,        /* over-voltage */
    PWR_FAULT_UV,        /* under-voltage */
    PWR_FAULT_TIMEOUT,   /* rail did not reach its good threshold within its sequencing delay */
    PWR_FAULT_GPI,        /* GPI-triggered fault */
    PWR_FAULT_WATCHDOG    /* ucd91320 6.1: "programmable watchdog timer and system reset" */
} power_fault_type_t;

/* overall sequencer state machine (ucd91320 6.1: power-on/off sequencing + fault shutdown) */
typedef enum power_controller_state_e {
    PWR_SEQ_RESET,
    PWR_SEQ_IDLE,
    PWR_SEQ_SEQUENCING_UP,
    PWR_SEQ_RUN,
    PWR_SEQ_SEQUENCING_DOWN,
    PWR_SEQ_FAULT_SHUTDOWN
} power_controller_state_t;

typedef struct power_controller_RTC_s {
    uint16_t seconds; /* 15:10=seconds  9:0=milliseconds */
    uint16_t DHM;     /* 15:11= Day 10:6 = Hours 5:0 = Minutes */
    uint16_t YM;      /* 15:4 = Year 3:0 = Month */
    uint16_t rsv;
} power_controller_RTC_t;
typedef struct power_controller_GPI_s {
    uint8_t id;
    uint8_t conf; //@@@ make this an enum
}power_controller_GPI_t;
typedef struct power_controller_GPICFG_s {
   power_controller_GPI_t GPI[32];  /* id=0 | (1 to 88) : conf [7:3][2]:polarity[1..0]:mode*/
   uint32_t fault_en;
   uint8_t LSCP;
   uint8_t MRG_EN;
   uint8_t MRG_LOW;
   uint8_t rsv1;
   uint8_t debug_pin;
}power_controller_GPICFG_t;
/* one of these for each rail */
typedef struct power_controller_SEQCFG_s {  //@@@ alignment issues
    uint16_t pad1;
    uint8_t ID;
    uint8_t ID_other;
    uint32_t GPI_seq_mask_on;
    uint32_t GPI_seq_mask_off;
    uint8_t seq_timeout_cfg;
    uint8_t seq_on_timeout;
    uint8_t seq_off_timeout;
    uint8_t pad2;
    uint32_t page_seq_on_dep_msk;
    uint32_t page_seq_off_dep_msk;
    uint32_t fault_slave_mask;
    uint16_t GPO_seq_on_dep_msk;
    uint16_t GPO_seq_off_dep_msk;
} power_controller_SEQCFG_t;
typedef struct power_controller_GPOCFG_s {
    power_controller_GPI_t GPO;
    uint8_t conf;     /* see table 26-41 */
    uint8_t dly;      /* see table 26-41 */
    uint8_t and_path0;
    uint8_t and_path1;
    struct {
        uint32_t status_mask;
        uint32_t status_inv_mask;
        uint32_t GPI_mask;
        uint32_t GPI_inv_mask;
        uint16_t GPO_mask;
        uint16_t GPO_inv_mask;
    } path[2];
}power_controller_GPOCFG_t;

typedef struct power_sequencer_MSCCFG_s {
    uint8_t misc_cfg;
    uint8_t time_2_reseq;
    uint16_t external_reference;
    uint32_t reseq_rail_mask;
} power_sequencer_MSCCFG_t;

/*
  _____       _ _       
 |  __ \     (_) |      
 | |__) |__ _ _| |___   
 |  _  // _` | | / __|  
 | | \ \ (_| | | \__ \  
 |_|  \_\__,_|_|_|___/   ( PAGE)
                        
                                            
*/
/* static, per-rail configuration (ucd91320 7.2.2: rail setup / monitoring / sequence / fault response / margining) */
typedef struct power_rail_cfg_s {
    power_controller_SEQCFG_t SEQ_config;



    uint16_t nominal_mv;         /* expected rail voltage, in mV */
    uint16_t ov_threshold_mv;    /* over-voltage fault threshold */
    uint16_t uv_threshold_mv;    /* under-voltage fault threshold */
    uint16_t nominal_c;          /* expected rail current, in mA */
    uint16_t oc_threshold;       /* over-current fault threshold, in some unit */ 
    uint16_t uc_threshold;       /* under-current fault threshold, in some unit */ 
    uint16_t on_delay_ms;        /* ENx assert -> expected rail-good delay (Figure 7-3 start-up waveforms) */
    uint16_t off_delay_ms;       /* ENx de-assert -> expected rail-off delay (Figure 7-4 shut-down waveforms) */
    uint16_t timeout_ms;         /* max time allowed to reach/leave PWR_RAIL_ON before PWR_FAULT_TIMEOUT */
    uint32_t dependency_mask;    /* bitmask (1<<rail_id) of rails that must be ON before this rail sequences on */
    bool                enabled;     /* current commanded ENx pin level */
    uint8_t  rail_group_mask;    /* bitmask of PWR_MAX_RAIL_GROUPS pin-selected states this rail participates in */
    power_monitor_type_t monitor_type;
    bool     margin_capable;     /* true if this rail's ENx pin doubles as a MARx margin/trim output */
    int8_t   margin_high_pct;    /* closed-loop margin/trim high limit, signed % of nominal_mv */
    int8_t   margin_low_pct;     /* closed-loop margin/trim low limit, signed % of nominal_mv */
    bool     enable_active_high; /* ENx polarity */
    /* USER SECTION */
} power_rail_cfg_t;
typedef struct power_rail_ctrl_s {
    power_rail_state_t state;
    uint8_t             en_out;     
    uint8_t             dis_out;
    power_fault_type_t last_fault;
    uint16_t            monitor_mv;  /* last-read MONx value, in mV (0 if monitor_type == PWR_MON_NONE) */
    uint16_t            monitor_c;   /* last-read current value, in mA (0 if monitor_type == PWR_MON_NONE) */
    /* USER SECTION */
    uint32_t            cnt; /* used in timing operations*/
    uint16_t            adc_raw_lo; /* computed adc value in raw counts */
    uint16_t            adc_raw_hi;
    uint32_t            adc_convert; /* used to scale from raw counts to mV*/
} power_rail_ctrl_t;
/* runtime state of a single rail */
typedef struct power_rail_s {
    power_rail_cfg_t   *cfg;         /* SRAM working copy, populated from cfg_store */
    power_rail_cfg_t   *cfg_store;   /* persistent configuration location in dataflash */
    power_rail_ctrl_t  *ctrl;  /* control data is in SRAM */

} power_rail_t;

/*
   _____                                           
  / ____|                                          
 | (___   ___  __ _ _   _  ___ _ __   ___ ___ _ __ 
  \___ \ / _ \/ _` | | | |/ _ \ '_ \ / __/ _ \ '__|
  ____) |  __/ (_| | |_| |  __/ | | | (_|  __/ |   
 |_____/ \___|\__, |\__,_|\___|_| |_|\___\___|_|   
                 | |                               
                 |_|                               
*/
typedef union power_sequencer_IO_u {
    struct {
        void* MON[PWR_MAX_MONITOR];
        void* DMON[PWR_MAX_DMON];
        void* EN[PWR_MAX_EN];
        void* MAR[PWR_MAX_MAR];
        void* GPIO[PWR_MAX_GPIO];
    } usage;
    void* pin[PWR_IO_DATA_WORDS];
} power_sequencer_IO_t;
typedef struct power_sequencer_CONST_s {
    uint8_t max_digital_comp;
    uint8_t max_GPOs;
    uint8_t max_GPIs;
    uint8_t max_pages;
    uint8_t max_fans;
    uint8_t max_monitors;
    uint8_t max_fault_entries;
    uint8_t max_PWMs;
} power_sequencer_CONST_t;
extern const power_sequencer_IO_t power_sequencer_IO;

typedef struct power_rail_map_s {
    uint8_t en_port;
    uint8_t en_pin_bit;
    uint8_t ADC0_index; //@@@ may also be an IO for fault.
    uint8_t ADC1_index;
}power_rail_map_t;

typedef struct power_sequencer_monitor_s {
    uint8_t MON[PWR_MAX_MONITOR + PWR_MAX_DMON];  //@@@ don't like this
}power_sequencer_monitor_t;

typedef struct power_sequencer_reset_config_s {
    uint32_t page_flag;
    uint32_t GPI_flag;
    uint8_t delay_timne;
    uint8_t pulse_time;
    uint8_t GPI_number;
    uint8_t GPI_tracking;
    uint8_t GPI_tracking_release_dly;
    uint8_t reset_pin_configurations;
    uint8_t reset_pin_config_2;
    uint8_t spare_1;
} power_sequencer_reset_config_t;

typedef struct power_sequencer_watchdog_config_s {
    uint8_t control;  /* 7=enable, 6=watcch reset bin, 5 = rsv, 4 = disable until system reset, 3:0 = start time */
    uint8_t WDI_ID;
    uint8_t WDI_conf;
    uint8_t reset_period;
    uint8_t WDO_ID;
    uint8_t WDO_conf;
    uint8_t spare[2];
} power_sequencer_watchdog_config_t;


/* one fault log record (ucd91320 6.3.1: Black Box Fault Log captures the first fault + full rail status) */
typedef struct power_fault_log_entry_s {
    uint8_t             rail_id;
    power_fault_type_t  fault;
    uint32_t            timestamp;   /* app_delay_ms()-relative ms, or RTC seconds if available */
} power_fault_log_entry_t;

/* nonvolatile-style event log (ucd91320 1 Features: "Nonvolatile fault event logging with RTC and timestamping") */
typedef struct power_fault_log_s {
    power_fault_log_entry_t entries[PWR_FAULT_LOG_DEPTH];
    uint8_t                 count;
    bool                    black_box_valid; /* first-fault snapshot captured, must be cleared before reuse */
} power_fault_log_t;
typedef struct power_fault_output_s {
  uint32_t fault_mask[4];  //@@@ arbitrary needs a define
  uint16_t fault_pins[4];  //@@@
  uint32_t GPI_mask[4];    //@@@
  uint8_t other_mask;
  uint8_t spare[3];
} power_fault_output_t;
typedef struct power_controller_railstate_s {
    uint8_t spare[2]; /* for alignment */
    uint8_t state_enables;
    uint8_t soft_off_enables;
    uint32_t system_state[8];
}power_controller_railstate_t;
/*
   _____            _             _ _           
  / ____|          | |           | | |          
 | |     ___  _ __ | |_ _ __ ___ | | | ___ _ __ 
 | |    / _ \| '_ \| __| '__/ _ \| | |/ _ \ '__|
 | |___| (_) | | | | |_| | | (_) | | |  __/ |   
  \_____\___/|_| |_|\__|_|  \___/|_|_|\___|_|   
                                                
*/
typedef struct power_controller_cfg_s {
    power_sequencer_monitor_t monitor;
    power_fault_output_t      faults;
    power_controller_GPICFG_t GPI_config;
    power_controller_GPOCFG_t GPO_config[16]; //@@@arbitrary
    power_controller_railstate_t railstate;
    uint32_t                  GPI;
    uint32_t                  resequence;
    power_sequencer_reset_config_t    reset_config;
    power_sequencer_watchdog_config_t watchdog_config;
    power_controller_RTC_t   RTC;
    uint32_t                 RTC_trim; /* see section 26.10 and 11 */
    power_sequencer_MSCCFG_t MSCCFG;   /* see section 26.42 */

    uint8_t                  active_rail_group;  /* 0..PWR_MAX_RAIL_GROUPS-1, selected via GPI Controlled Rail Groups */
    uint8_t                  pmbus_address;      /* ucd91320 6.3.2: 7-bit PMBus address (PMBUS_ADDRx pins) */
    uint8_t                  cascade_id;         /* 0..PWR_MAX_CASCADE-1 */
    uint8_t                  cascade_count;       /* number of devices in the cascade, 1 if standalone */
    uint16_t                 watchdog_timeout_ms;
    uint32_t                 gpi_mask;           /* general purpose input pin states */
    uint32_t                 gpo_mask;           /* command/logic controlled (LGPO) output pin states */
    bool                     watchdog_enabled;
    bool                     sync_clock_enabled; /* SYNC_CLK shared clock for cascaded devices */
    bool                     fault_pin_asserted; /* shared fault pin coordinating cascaded devices */
    uint8_t                  spare1; /* for alignment */

} power_controller_cfg_t;
typedef struct power_controller_ctrl_s {
    uint32_t                 GPI;     /* this is the "live" GPIs */
    uint32_t                 Page;    /* these are the pages that are ON */
    power_controller_state_t state;
    power_fault_log_t        fault_log;
    uint8_t                  gpo_index;
    uint8_t                  page;
    uint8_t                  ram_00;
    uint8_t                  GPIO_select;
    uint8_t                  GPIO_config;
    uint8_t                  spare[3];

    uint32_t                 rails_ready;
    uint32_t                 rails_enabled;
    uint32_t                 rails_faulted;
    uint32_t                 event; 
} power_controller_ctrl_t;
/*
 * Power sequencer / system manager object.
 * One instance models one UCD91320-like device; PWR_MAX_CASCADE of these
 * could be linked (ucd91320 1 Features: "Cascade up to 4 devices to
 * sequence up to 128 rails") via cascade_id/cascade_count below.
 */
typedef struct power_controller_s {
    power_rail_t            *rails;
    power_controller_cfg_t  *cfg;
    power_controller_cfg_t  *cfg_store;
    power_controller_ctrl_t *ctrl;
    power_rail_map_t        *map;   /* mapping data is in SRAM */
    power_sequencer_IO_t    *GPIO;
    power_sequencer_CONST_t *CONSTANTS;

} power_controller_t;

extern const power_controller_t PowerController;
/* USER ADDITIONAL PUBLIC FUNCTIONS */
/* Scan ADC_data[i] (raw ADC counts, one per rail, same indexing as PowerController.rails[])
 * into each PWR_MON_ANALOG rail's monitor_mv. Call this once per ADC scan cycle. */
void pwr_mod_poll(uint16_t *ADC_data);
void pwr_seq_poll_GPI(void);
void pwr_seq_poll_Page(void);
/* Checks the CRC-16 trailer of a DF_POWER_RAIL_RECORD_SIZE-byte data-flash record at `p`. */
bool pwr_dataflash_check(const uint8_t *p);

void pwr_rail_store_config(uint16_t rail_index,uint8_t *data,uint16_t len );

void pwr_seq_update_cfg(uint8_t *dest,uint8_t *src,uint16_t len);

void pwr_seq_store_config(uint8_t *data,uint16_t len );
uint16_t pwr_seq_read_config(uint8_t *data,uint16_t len );

void pwr_seq_store_map(uint8_t *data,uint16_t len ); //@@@ this needs to die.

void pwr_seq_store_fault_config(uint8_t *data,uint16_t len );
uint16_t pwr_seq_read_fault_config(uint8_t *data);

void pwr_seq_store_all(void);
void pwr_seq_restore_all(void);

// void pwr_seq_update_monitor(uint8_t *data,uint16_t len);
void pwr_seq_update_seqcfg(uint8_t rail_index,uint8_t *src,uint16_t data_len);
uint16_t pwr_seq_read_seqcfg(uint8_t rail_index,uint8_t *src);
void pwr_seq_update_railstate(uint8_t *src,uint16_t data_len);
uint16_t pwr_seq_read_railstate(uint8_t *src);

void pwr_seq_update_GPIO_cfg(uint8_t *p);
uint16_t pwr_seq_read_GPIO_cfg(uint8_t *p);
void pwr_seq_configure(void);


#endif /* POWER_MODULE_H_ */
