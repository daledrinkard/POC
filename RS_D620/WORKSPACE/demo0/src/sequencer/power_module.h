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
#define PWR_MAX_RAILS       (8)  /* ucd91320: up to 24 analog/digital + 8 digital-only (32 total) rails */
#define PWR_MAX_RAIL_GROUPS (8)  /* ucd91320 6.1: up to 8 GPI-selected pin-selected rail states (ACPI-style) */
#define PWR_FAULT_LOG_DEPTH (8)  /* ucd91320 1/6.3.1: single-event fault log (100 entries) + Black Box Fault Log */
#define PWR_MAX_CASCADE     (4)  /* ucd91320 1: cascade up to 4 devices to sequence up to 128 rails */
/* USER DEFINE */

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
typedef enum power_sequencer_state_e {
    PWR_SEQ_IDLE,
    PWR_SEQ_SEQUENCING_UP,
    PWR_SEQ_RUN,
    PWR_SEQ_SEQUENCING_DOWN,
    PWR_SEQ_FAULT_SHUTDOWN
} power_sequencer_state_t;

/* static, per-rail configuration (ucd91320 7.2.2: rail setup / monitoring / sequence / fault response / margining) */
typedef struct power_rail_cfg_s {
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
    power_fault_type_t last_fault;
    uint16_t            monitor_mv;  /* last-read MONx value, in mV (0 if monitor_type == PWR_MON_NONE) */
    uint16_t            monitor_c;   /* last-read current value, in mA (0 if monitor_type == PWR_MON_NONE) */
    /* USER SECTION */
    uint32_t            cnt; /* used in timing operations*/
    uint16_t            adc_raw_lo; /* computed adc value in raw counts */
    uint16_t            adc_raw_hi;
    uint32_t            adc_convert; /* used to scale from raw counts to mV*/
} power_rail_ctrl_t;
typedef struct power_rail_map_s {
    uint8_t en_port;
    uint8_t en_pin_bit;
    uint8_t ADC0_index;
    uint8_t ADC1_index;
}power_rail_map_t;
/* runtime state of a single rail */
typedef struct power_rail_s {
    power_rail_cfg_t   *cfg;   /* configuration is in dataflash */
    power_rail_ctrl_t  *ctrl;  /* control data is in SRAM */
} power_rail_t;

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
typedef struct power_controller_cfg_s {
    /* USER SECTION */
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
    /* USER SECTION */
    power_sequencer_state_t  state;
    power_fault_log_t        fault_log;
} power_controller_ctrl_t;
/*
 * Power sequencer / system manager object.
 * One instance models one UCD91320-like device; PWR_MAX_CASCADE of these
 * could be linked (ucd91320 1 Features: "Cascade up to 4 devices to
 * sequence up to 128 rails") via cascade_id/cascade_count below.
 */
typedef struct power_controller_s {
    power_rail_t            *rails;//[PWR_MAX_RAILS];
    power_controller_cfg_t  *cfg;
    power_controller_ctrl_t *ctrl;
    power_rail_map_t        *map;//[PWR_MAX_RAILS];   /* mapping data is in SRAM */

    /* USER SECTION */
} power_controller_t;

extern const power_controller_t PowerController;
/* USER ADDITIONAL PUBLIC FUNCTIONS */
/* Scan ADC_data[i] (raw ADC counts, one per rail, same indexing as PowerController.rails[])
 * into each PWR_MON_ANALOG rail's monitor_mv. Call this once per ADC scan cycle. */
void pwr_mod_poll(uint16_t *ADC_data);
/* Checks the CRC-16 trailer of a DF_POWER_RAIL_RECORD_SIZE-byte data-flash record at `p`. */
bool pwr_dataflash_check(const uint8_t *p);
void pwr_mod_update_config(uint16_t rail_index,uint8_t *data,uint16_t len );
void pwr_seq_update_config(uint8_t *data,uint16_t len );
void pwr_seq_update_map(uint8_t *data,uint16_t len );
#endif /* POWER_MODULE_H_ */
