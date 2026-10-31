/*
 *        DBG_PRINT.C
 *
 *  Human-readable dump of the PowerController object (power_module.h) to
 *  the debug console. All register/value fields are printed in hexadecimal;
 *  ANSI escape codes are used to highlight state/fault information for a
 *  terminal that supports them.
 *
 */
#include "dbg_print.h"
#include "power_module.h"
#include "PMbus_commands.h"
#include <stdio.h>
/* USER */

/* ANSI escapes - CLR_RESET must follow any of the others to stop highlighting */
#define CLR_RESET    "\x1b[0m"
#define CLR_HEADING  "\x1b[1;36m"  /* bold cyan   - section headings */
#define CLR_LABEL    "\x1b[1;37m"  /* bold white  - sub-labels */
#define CLR_OK       "\x1b[1;32m"  /* bold green  - ON / good / enabled */
#define CLR_WARN     "\x1b[1;33m"  /* bold yellow - transitional / margin states */
#define CLR_FAULT    "\x1b[1;31m"  /* bold red    - faults / disabled */

static char line[160]; /* scratch buffer reused for every sprintf()+APP_PRINT() below */

static const char *rail_state_text(power_rail_state_t s);
static const char *rail_state_color(power_rail_state_t s);
static const char *ctrl_state_text(power_controller_state_t s);
static const char *ctrl_state_color(power_controller_state_t s);
static const char *fault_text(power_fault_type_t f);
static const char *pmbus_cmd_text(pmbus_command_t cmd);
static void bin32_str(uint32_t v, char *out /* at least 33 bytes */);
static const char *mon_type_text(uint8_t v);
static void print_monitor_config(const power_sequencer_monitor_t *mon);
static void print_fault_log(const power_fault_log_t *log);
static void print_GPI_config(const power_controller_GPICFG_t *gpi_cfg);
static void print_GPO_config(const power_controller_GPOCFG_t *gpo_cfg, uint8_t count);
static void print_rail(uint8_t index, const power_rail_t *rail);

void print_controller(int num_rails)
{
    const power_controller_cfg_t  *cfg  = PowerController.cfg;
    const power_controller_ctrl_t *ctrl = PowerController.ctrl;

    sprintf(line, "\n" CLR_HEADING "===================== POWER CONTROLLER =====================" CLR_RESET "\n");
    APP_PRINT(line);

    /* ---- runtime state ---- */
    sprintf(line, CLR_HEADING "-- state --" CLR_RESET "\n");
    APP_PRINT(line);
    sprintf(line, "  %-20s: %s0x%02X (%s)" CLR_RESET "\n", "seq state",
            ctrl_state_color(ctrl->state), (unsigned) ctrl->state, ctrl_state_text(ctrl->state));
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%02X\n", "page (PMBus)", (unsigned) ctrl->page);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%08X\n", "GPI (live)", (unsigned) ctrl->GPI);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%08X\n", "Page (rails ON)", (unsigned) ctrl->Page);
    APP_PRINT(line);
    sprintf(line, "  %-20s: " CLR_OK "0x%08X" CLR_RESET "\n", "rails_ready", (unsigned) ctrl->rails_ready);
    APP_PRINT(line);
    sprintf(line, "  %-20s: " CLR_OK "0x%08X" CLR_RESET "\n", "rails_enabled", (unsigned) ctrl->rails_enabled);
    APP_PRINT(line);
    sprintf(line, "  %-20s: %s0x%08X" CLR_RESET "\n", "rails_faulted",
            (0 != ctrl->rails_faulted) ? CLR_FAULT : CLR_RESET, (unsigned) ctrl->rails_faulted);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%08X\n", "event", (unsigned) ctrl->event);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%02X / 0x%02X / 0x%02X\n", "gpo_idx/GPIOsel/cfg",
            (unsigned) ctrl->gpo_index, (unsigned) ctrl->GPIO_select, (unsigned) ctrl->GPIO_config);
    APP_PRINT(line);

    print_fault_log(&ctrl->fault_log);

    /* ---- static config: identity / cascade / watchdog ---- */
    sprintf(line, CLR_HEADING "-- controller config --" CLR_RESET "\n");
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%02X\n", "pmbus_address", (unsigned) cfg->pmbus_address);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%02X\n", "active_rail_group", (unsigned) cfg->active_rail_group);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%02X / 0x%02X\n", "cascade_id/count",
            (unsigned) cfg->cascade_id, (unsigned) cfg->cascade_count);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%08X / 0x%08X\n", "gpi_mask/gpo_mask",
            (unsigned) cfg->gpi_mask, (unsigned) cfg->gpo_mask);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%02X\n", "pwm_select", (unsigned) cfg->pwm_select);
    APP_PRINT(line);
    sprintf(line, "  %-20s: %s0x%02X" CLR_RESET " / %s0x%02X" CLR_RESET " / %s0x%02X" CLR_RESET "\n",
            "wdog/sync/fault_pin",
            cfg->watchdog_enabled     ? CLR_OK : CLR_RESET, (unsigned) cfg->watchdog_enabled,
            cfg->sync_clock_enabled   ? CLR_OK : CLR_RESET, (unsigned) cfg->sync_clock_enabled,
            cfg->fault_pin_asserted   ? CLR_FAULT : CLR_RESET, (unsigned) cfg->fault_pin_asserted);
    APP_PRINT(line);
    sprintf(line, "  %-20s: 0x%04X\n", "watchdog_timeout_ms", (unsigned) cfg->watchdog_timeout_ms);
    APP_PRINT(line);
    sprintf(line, "  %-20s: ctl=0x%02X WDI=0x%02X/0x%02X WDO=0x%02X/0x%02X period=0x%02X\n", "watchdog_config",
            (unsigned) cfg->watchdog_config.control, (unsigned) cfg->watchdog_config.WDI_ID,
            (unsigned) cfg->watchdog_config.WDI_conf, (unsigned) cfg->watchdog_config.WDO_ID,
            (unsigned) cfg->watchdog_config.WDO_conf, (unsigned) cfg->watchdog_config.reset_period);
    APP_PRINT(line);
    sprintf(line, "  %-20s: page_flag=0x%08X GPI_flag=0x%08X dly=0x%02X pulse=0x%02X\n", "reset_config",
            (unsigned) cfg->reset_config.page_flag, (unsigned) cfg->reset_config.GPI_flag,
            (unsigned) cfg->reset_config.delay_timne, (unsigned) cfg->reset_config.pulse_time);
    APP_PRINT(line);
    sprintf(line, "  %-20s: GPI=0x%02X track=0x%02X/0x%02X pincfg=0x%02X/0x%02X\n", "",
            (unsigned) cfg->reset_config.GPI_number, (unsigned) cfg->reset_config.GPI_tracking,
            (unsigned) cfg->reset_config.GPI_tracking_release_dly,
            (unsigned) cfg->reset_config.reset_pin_configurations, (unsigned) cfg->reset_config.reset_pin_config_2);
    APP_PRINT(line);
    sprintf(line, "  %-20s: duty=0x%04X freq=0x%08X\n", "pwm_config",
            (unsigned) cfg->pwm_config.duty_cycle, (unsigned) cfg->pwm_config.frequency);
    APP_PRINT(line);
    sprintf(line, "  %-20s: misc=0x%02X t2reseq=0x%02X ext_ref=0x%04X rail_msk=0x%08X\n", "MSCCFG",
            (unsigned) cfg->MSCCFG.misc_cfg, (unsigned) cfg->MSCCFG.time_2_reseq,
            (unsigned) cfg->MSCCFG.external_reference, (unsigned) cfg->MSCCFG.reseq_rail_mask);
    APP_PRINT(line);
    sprintf(line, "  %-20s: GPI=0x%08X resequence=0x%08X RTC_trim=0x%08X\n", "misc",
            (unsigned) cfg->GPI, (unsigned) cfg->resequence, (unsigned) cfg->RTC_trim);
    APP_PRINT(line);

    /* ---- RTC (decoded per the bitfield comments in power_module.h) ---- */
    {
        unsigned sec   = (cfg->RTC.seconds >> 10) & 0x3F;
        unsigned ms    =  cfg->RTC.seconds        & 0x3FF;
        unsigned day   = (cfg->RTC.DHM    >> 11) & 0x1F;
        unsigned hour  = (cfg->RTC.DHM    >> 6)  & 0x1F;
        unsigned min   =  cfg->RTC.DHM           & 0x3F;
        unsigned year  = (cfg->RTC.YM     >> 4)  & 0xFFF;
        unsigned month =  cfg->RTC.YM            & 0xF;
        sprintf(line, "  %-20s: 0x%04X/0x%04X/0x%04X  (day=0x%02X %02X:%02X:%02X.%03X  year=0x%04X mon=0x%02X)\n",
                "RTC", (unsigned) cfg->RTC.seconds, (unsigned) cfg->RTC.DHM, (unsigned) cfg->RTC.YM,
                day, hour, min, sec, ms, year, month);
        APP_PRINT(line);
    }

    /* ---- fault output routing ---- */
    sprintf(line, CLR_HEADING "-- fault outputs --" CLR_RESET "\n");
    APP_PRINT(line);
    for (uint8_t g = 0; g < PWR_MAX_FAULT_GROUPS; g++)
    {
        sprintf(line, "  group[%u] mask=0x%08X pins=0x%04X GPI_mask=0x%08X\n", g,
                (unsigned) cfg->faults.fault_mask[g], (unsigned) cfg->faults.fault_pins[g],
                (unsigned) cfg->faults.GPI_mask[g]);
        APP_PRINT(line);
    }
    sprintf(line, "  %-20s: 0x%02X\n", "other_mask", (unsigned) cfg->faults.other_mask);
    APP_PRINT(line);

    print_GPI_config(&cfg->GPI_config);
    print_GPO_config(cfg->GPO_config, PWR_GPO_SIZE);

    print_monitor_config(&cfg->monitor);

    /* ---- per rail ---- */
    {
        uint8_t rail_count = (uint8_t) ((num_rails < 0) ? 0 : num_rails);
        if (rail_count > PWR_MAX_RAILS) rail_count = PWR_MAX_RAILS;

        sprintf(line, "\n" CLR_HEADING "======================= RAILS (0x%02X) =======================" CLR_RESET "\n", rail_count);
        APP_PRINT(line);
        for (uint8_t i = 0; i < rail_count; i++)
        {
            print_rail(i, &PowerController.rails[i]);
        }
    }

    sprintf(line, "\n" CLR_HEADING "=============================================================" CLR_RESET "\n");
    APP_PRINT(line);
}

static void print_rail(uint8_t index, const power_rail_t *rail)
{
    const power_rail_cfg_t  *c = rail->cfg;
    const power_rail_ctrl_t *r = rail->ctrl;

    sprintf(line, CLR_HEADING "-- rail 0x%02X (ID=0x%02X/0x%02X) --" CLR_RESET "\n",
            index, (unsigned) c->SEQ_config.ID, (unsigned) c->SEQ_config.ID_other);
    APP_PRINT(line);
    sprintf(line, "  %-20s: %s0x%02X (%s)" CLR_RESET "   en=0x%02X dis=0x%02X   fault=%s%s" CLR_RESET "\n",
            "state", rail_state_color(r->state), (unsigned) r->state, rail_state_text(r->state),
            (unsigned) r->en_out, (unsigned) r->dis_out,
            (PWR_FAULT_NONE != r->last_fault) ? CLR_FAULT : CLR_RESET, fault_text(r->last_fault));
    APP_PRINT(line);
    sprintf(line, "  %-20s: mv=0x%04X  c=0x%04X  adc_lo/hi=0x%04X/0x%04X  conv=0x%08X\n", "monitor",
            (unsigned) r->monitor_mv, (unsigned) r->monitor_c,
            (unsigned) r->adc_raw_lo, (unsigned) r->adc_raw_hi, (unsigned) r->adc_convert);
    APP_PRINT(line);

    sprintf(line, "  %-20s: op=0x%02X on_off=0x%02X vout_mode=0x%02X ton_max_rsp=0x%02X\n", "config",
            (unsigned) c->operation, (unsigned) c->on_off_config, (unsigned) c->vout_mode,
            (unsigned) c->ton_max_fault_response);
    APP_PRINT(line);
    sprintf(line, "  %-20s: cmd=0x%04X margin_hi/lo=0x%04X/0x%04X scale=0x%04X margin_cfg=0x%04X\n", "vout",
            (unsigned) c->vout_command, (unsigned) c->vout_margin_high, (unsigned) c->vout_margin_low,
            (unsigned) c->vout_scale_monitor, (unsigned) c->margin_config);
    APP_PRINT(line);
    sprintf(line, "  %-20s: ov_fault/warn=0x%04X/0x%04X uv_warn/fault=0x%04X/0x%04X\n", "vout limits",
            (unsigned) c->vout_ov_fault_limit, (unsigned) c->vout_ov_warn_limit,
            (unsigned) c->vout_uv_warn_limit, (unsigned) c->vout_uv_fault_limit);
    APP_PRINT(line);
    sprintf(line, "  %-20s: ot_fault/warn=0x%04X/0x%04X cal_gain/off=0x%04X/0x%04X cal_mon=0x%04X\n", "temperature",
            (unsigned) c->ot_fault_limit, (unsigned) c->ot_warn_limit,
            (unsigned) c->temperature_cal_gain, (unsigned) c->temperature_cal_offset,
            (unsigned) c->vout_cal_monitor);
    APP_PRINT(line);
    sprintf(line, "  %-20s: pg_on/off=0x%04X/0x%04X ton/toff_delay=0x%04X/0x%04X\n", "sequencing",
            (unsigned) c->power_good_on, (unsigned) c->power_good_off,
            (unsigned) c->ton_delay, (unsigned) c->toff_delay);
    APP_PRINT(line);
    sprintf(line, "  %-20s: ton_max_lim=0x%04X toff_warn_lim=0x%04X\n", "",
            (unsigned) c->ton_max_fault_limit, (unsigned) c->toff_max_warn_limit);
    APP_PRINT(line);
    sprintf(line, "  %-20s: status vout/iout/temp=0x%02X/0x%02X/0x%02X\n", "status",
            (unsigned) c->status_vout, (unsigned) c->status_iout, (unsigned) c->status_temperature);
    APP_PRINT(line);
    sprintf(line, "  %-20s: rail=%u conf=%u\n", "SEQ_config",
            (unsigned) c->SEQ_config.ID, (unsigned) c->SEQ_config.ID_other);
    APP_PRINT(line);
    {
        char bin_a[33];
        char bin_b[33];
        bin32_str(c->SEQ_config.GPI_seq_mask_on, bin_a);
        bin32_str(c->SEQ_config.page_seq_on_dep_msk, bin_b);
        sprintf(line, "  %-20s: GPI_seq_on =%s  page_dep_on =%s\n", "", bin_a, bin_b);
        APP_PRINT(line);
        bin32_str(c->SEQ_config.GPI_seq_mask_off, bin_a);
        bin32_str(c->SEQ_config.page_seq_off_dep_msk, bin_b);
        sprintf(line, "  %-20s: GPI_seq_off=%s  page_dep_off=%s\n", "", bin_a, bin_b);
        APP_PRINT(line);
    }
    sprintf(line, "  %-20s: fault_slave=0x%08X GPO_dep on/off=0x%04X/0x%04X timeout cfg/on/off=0x%02X/0x%02X/0x%02X\n", "",
            (unsigned) c->SEQ_config.fault_slave_mask,
            (unsigned) c->SEQ_config.GPO_seq_on_dep_msk, (unsigned) c->SEQ_config.GPO_seq_off_dep_msk,
            (unsigned) c->SEQ_config.seq_timeout_cfg, (unsigned) c->SEQ_config.seq_on_timeout,
            (unsigned) c->SEQ_config.seq_off_timeout);
    APP_PRINT(line);

    sprintf(line, "  %-20s: ov/uv=0x%02X/0x%02X oc/uc=0x%02X/0x%02X ot=0x%02X ton_max=0x%02X retry=0x%02X glitch f/nf=0x%02X/0x%02X\n",
            "fault_responses",
            (unsigned) c->fault_responses.vout_ov, (unsigned) c->fault_responses.vout_uv,
            (unsigned) c->fault_responses.iout_oc, (unsigned) c->fault_responses.iout_uc,
            (unsigned) c->fault_responses.ot, (unsigned) c->fault_responses.ton_max,
            (unsigned) c->fault_responses.time_to_retry,
            (unsigned) c->fault_responses.max_glitch_voltage_fault,
            (unsigned) c->fault_responses.max_glitch_voltage_no_fault);
    APP_PRINT(line);
    sprintf(line, "  %-20s: retry=0x%02X max_glitch=0x%04X profile=0x%02X/0x%02X block_out=0x%04X\n",
            "gpi_fault_response",
            (unsigned) c->gpi_fault_responses.time_to_retry,
            (unsigned) c->gpi_fault_responses.max_glitch_GPI,
            (unsigned) c->gpi_fault_responses.profile_section_1,
            (unsigned) c->gpi_fault_responses.profile_section_2,
            (unsigned) c->gpi_fault_responses.block_out_period);
    APP_PRINT(line);
}

static void print_GPI_config(const power_controller_GPICFG_t *gpi_cfg)
{
    sprintf(line, CLR_HEADING "-- GPI config --" CLR_RESET "\n");
    APP_PRINT(line);
    sprintf(line, "  %-20s: fault_en=0x%08X LSCP=0x%02X MRG_EN=0x%02X MRG_LOW=0x%02X debug_pin=0x%02X\n", "",
            (unsigned) gpi_cfg->fault_en, (unsigned) gpi_cfg->LSCP,
            (unsigned) gpi_cfg->MRG_EN, (unsigned) gpi_cfg->MRG_LOW, (unsigned) gpi_cfg->debug_pin);
    APP_PRINT(line);
    for (uint8_t i = 0; i < PWR_GPI_SIZE; i++)
    {
        if (0 == gpi_cfg->GPI[i].id) break; /* id==0 marks the end of the populated entries */
        sprintf(line, "  [0x%02X] id=0x%02X conf=0x%02X\n",
                i, (unsigned) gpi_cfg->GPI[i].id, (unsigned) gpi_cfg->GPI[i].conf);
        APP_PRINT(line);
    }
}

static void print_GPO_config(const power_controller_GPOCFG_t *gpo_cfg, uint8_t count)
{
    sprintf(line, CLR_HEADING "-- GPO config --" CLR_RESET "\n");
    APP_PRINT(line);
    for (uint8_t i = 0; i < count; i++)
    {
        const power_controller_GPOCFG_t *g = &gpo_cfg[i];
        if (0 == g->GPO.id) break; /* id==0 marks the end of the populated entries */
        sprintf(line, "  [0x%02X] id=0x%02X conf=0x%02X dly=0x%02X and=0x%02X/0x%02X\n",
                i, (unsigned) g->GPO.id, (unsigned) g->conf, (unsigned) g->dly,
                (unsigned) g->and_path0, (unsigned) g->and_path1);
        APP_PRINT(line);
        for (uint8_t p = 0; p < 2; p++)
        {
            sprintf(line, "        path%u: st=0x%08X st_inv=0x%08X gpi=0x%08X gpi_inv=0x%08X gpo=0x%04X gpo_inv=0x%04X\n",
                    p, (unsigned) g->path[p].status_mask, (unsigned) g->path[p].status_inv_mask,
                    (unsigned) g->path[p].GPI_mask, (unsigned) g->path[p].GPI_inv_mask,
                    (unsigned) g->path[p].GPO_mask, (unsigned) g->path[p].GPO_inv_mask);
            APP_PRINT(line);
        }
    }
}

static void print_fault_log(const power_fault_log_t *log)
{
    sprintf(line, CLR_HEADING "-- fault log --" CLR_RESET "   count=0x%02X  black_box=%s0x%02X" CLR_RESET "\n",
            (unsigned) log->count, log->black_box_valid ? CLR_WARN : CLR_RESET, (unsigned) log->black_box_valid);
    APP_PRINT(line);
    for (uint8_t i = 0; i < PWR_FAULT_LOG_DEPTH; i++)
    {
        const power_fault_log_entry_t *e = &log->entries[i];
        if (PWR_FAULT_NONE == e->fault) continue;
        sprintf(line, "  [0x%02X] rail=0x%02X fault=" CLR_FAULT "%s" CLR_RESET " t=0x%08X\n",
                i, (unsigned) e->rail_id, fault_text(e->fault), (unsigned) e->timestamp);
        APP_PRINT(line);
    }
}

/* decodes a MON[] byte's upper 3 bits (type field, power_module.h line 38) */
static const char *mon_type_text(uint8_t v)
{
    switch (v & 0xE0)
    {
    case PWR_MON_TYPE_NONE:         return "NONE";
    case PWR_MON_TYPE_ANALOG:       return "ANALOG";
    case PWR_MON_TYPE_TEMP:         return "TEMP";
    case PWR_MON_TYPE_CURRENT:      return "CURRENT";
    case PWR_MON_TYPE_VOLT_COMPARE: return "VOLT_COMPARE";
    case PWR_MON_TYPE_VOLT_INPUT:   return "VOLT_INPUT";
    case PWR_MON_TYPE_DIGITAL:      return "DIGITAL";
    default:                        return "?";
    }
}

/* prints each MON[] byte decoded as type (bits 7:5) + page number (bits 4:0, decimal) */
static void print_monitor_config(const power_sequencer_monitor_t *mon)
{
    sprintf(line, CLR_HEADING "-- monitor config (MON[]) --" CLR_RESET "\n");
    APP_PRINT(line);
    for (uint8_t i = 0; i < (uint8_t) sizeof(mon->MON); i++)
    {
        uint8_t v = mon->MON[i];
        if (PWR_MON_TYPE_NONE == (v & 0xE0)) continue;
        {
            void *pin_ptr = power_sequencer_IO.pin[i];
            uint32_t pfs  = (NULL != pin_ptr) ? *((uint32_t *) pin_ptr) : 0;
            sprintf(line, "  [0x%02X] %-12s %u   pin=0x%08X pfs=0x%08X\n", i, mon_type_text(v),
                    (unsigned) (v & 0x1F), (unsigned) (uintptr_t) pin_ptr, (unsigned) pfs);
        }
        APP_PRINT(line);
    }
}

/* renders v's 32 bits MSB-first into out[0..31] as '.'=0 / '*'=1, out[32]=0 */
static void bin32_str(uint32_t v, char *out)
{
    for (uint8_t i = 0; i < 32; i++)
    {
        out[i] = (0 != (v & (1UL << (31 - i)))) ? '*' : '.';
    }
    out[32] = 0;
}

static const char *rail_state_text(power_rail_state_t s)
{
    switch (s)
    {
    case PWR_RAIL_OFF:         return "OFF";
    case PWR_RAIL_SEQ_ON:      return "SEQ_ON";
    case PWR_RAIL_ON:          return "ON";
    case PWR_RAIL_MARGIN_HIGH: return "MARGIN_HIGH";
    case PWR_RAIL_MARGIN_LOW:  return "MARGIN_LOW";
    case PWR_RAIL_SEQ_OFF:     return "SEQ_OFF";
    case PWR_RAIL_FAULT:       return "FAULT";
    default:                   return "?";
    }
}
static const char *rail_state_color(power_rail_state_t s)
{
    switch (s)
    {
    case PWR_RAIL_ON:          return CLR_OK;
    case PWR_RAIL_SEQ_ON:
    case PWR_RAIL_SEQ_OFF:
    case PWR_RAIL_MARGIN_HIGH:
    case PWR_RAIL_MARGIN_LOW:  return CLR_WARN;
    case PWR_RAIL_FAULT:       return CLR_FAULT;
    case PWR_RAIL_OFF:
    default:                  return CLR_RESET;
    }
}
static const char *ctrl_state_text(power_controller_state_t s)
{
    switch (s)
    {
    case PWR_SEQ_RESET:            return "RESET";
    case PWR_SEQ_IDLE:              return "IDLE";
    case PWR_SEQ_SEQUENCING_UP:     return "SEQUENCING_UP";
    case PWR_SEQ_RUN:                return "RUN";
    case PWR_SEQ_SEQUENCING_DOWN:   return "SEQUENCING_DOWN";
    case PWR_SEQ_FAULT_SHUTDOWN:    return "FAULT_SHUTDOWN";
    default:                        return "?";
    }
}
static const char *ctrl_state_color(power_controller_state_t s)
{
    switch (s)
    {
    case PWR_SEQ_RUN:              return CLR_OK;
    case PWR_SEQ_SEQUENCING_UP:
    case PWR_SEQ_SEQUENCING_DOWN:  return CLR_WARN;
    case PWR_SEQ_FAULT_SHUTDOWN:   return CLR_FAULT;
    case PWR_SEQ_RESET:
    case PWR_SEQ_IDLE:
    default:                       return CLR_RESET;
    }
}
static const char *fault_text(power_fault_type_t f)
{
    switch (f)
    {
    case PWR_FAULT_NONE:     return "NONE";
    case PWR_FAULT_OV:       return "OV";
    case PWR_FAULT_UV:       return "UV";
    case PWR_FAULT_TIMEOUT:  return "TIMEOUT";
    case PWR_FAULT_GPI:      return "GPI";
    case PWR_FAULT_WATCHDOG: return "WATCHDOG";
    default:                 return "?";
    }
}
static const char *pmbus_cmd_text(pmbus_command_t cmd)
{
    switch (cmd)
    {
    case PMBUS_CMD_PAGE: return "PAGE";
    case PMBUS_CMD_OPERATION: return "OPERATION";
    case PMBUS_CMD_ON_OFF_CONFIG: return "ON_OFF_CONFIG";
    case PMBUS_CMD_CLEAR_FAULTS: return "CLEAR_FAULTS";
    case PMBUS_CMD_PHASE: return "PHASE";
    case PMBUS_CMD_PASSKEY: return "PASSKEY";
    case PMBUS_CMD_ACCESS_CONTROL: return "ACCESS_CONTROL";
    case PMBUS_CMD_WRITE_PROTECT: return "WRITE_PROTECT";
    case PMBUS_CMD_STORE_DEFAULT_ALL: return "STORE_DEFAULT_ALL";
    case PMBUS_CMD_RESTORE_DEFAULT_ALL: return "RESTORE_DEFAULT_ALL";
    case PMBUS_CMD_STORE_DEFAULT_CODE: return "STORE_DEFAULT_CODE";
    case PMBUS_CMD_RESTORE_DEFAULT_CODE: return "RESTORE_DEFAULT_CODE";
    case PMBUS_CMD_STORE_USER_ALL: return "STORE_USER_ALL";
    case PMBUS_CMD_RESTORE_USER_ALL: return "RESTORE_USER_ALL";
    case PMBUS_CMD_STORE_USER_CODE: return "STORE_USER_CODE";
    case PMBUS_CMD_RESTORE_USER_CODE: return "RESTORE_USER_CODE";
    case PMBUS_CMD_CAPABILITY: return "CAPABILITY";
    case PMBUS_CMD_QUERY: return "QUERY";
    case PMBUS_CMD_SMBALERT_MASK: return "SMBALERT_MASK";
    case PMBUS_CMD_VOUT_MODE: return "VOUT_MODE";
    case PMBUS_CMD_VOUT_COMMAND: return "VOUT_COMMAND";
    case PMBUS_CMD_VOUT_TRIM: return "VOUT_TRIM";
    case PMBUS_CMD_VOUT_CAL_OFFSET: return "VOUT_CAL_OFFSET";
    case PMBUS_CMD_VOUT_MAX: return "VOUT_MAX";
    case PMBUS_CMD_VOUT_MARGIN_HIGH: return "VOUT_MARGIN_HIGH";
    case PMBUS_CMD_VOUT_MARGIN_LOW: return "VOUT_MARGIN_LOW";
    case PMBUS_CMD_VOUT_TRANSITION_RATE: return "VOUT_TRANSITION_RATE";
    case PMBUS_CMD_VOUT_DROOP: return "VOUT_DROOP";
    case PMBUS_CMD_VOUT_SCALE_LOOP: return "VOUT_SCALE_LOOP";
    case PMBUS_CMD_VOUT_SCALE_MONITOR: return "VOUT_SCALE_MONITOR";
    case PMBUS_CMD_COEFFICIENTS: return "COEFFICIENTS";
    case PMBUS_CMD_POUT_MAX: return "POUT_MAX";
    case PMBUS_CMD_MAX_DUTY: return "MAX_DUTY";
    case PMBUS_CMD_FREQUENCY_SWITCH: return "FREQUENCY_SWITCH";
    case PMBUS_CMD_VIN_ON: return "VIN_ON";
    case PMBUS_CMD_VIN_OFF: return "VIN_OFF";
    case PMBUS_CMD_INTERLEAVE: return "INTERLEAVE";
    case PMBUS_CMD_IOUT_CAL_GAIN: return "IOUT_CAL_GAIN";
    case PMBUS_CMD_IOUT_CAL_OFFSET: return "IOUT_CAL_OFFSET";
    case PMBUS_CMD_FAN_CONFIG_1_2: return "FAN_CONFIG_1_2";
    case PMBUS_CMD_FAN_COMMAND_1: return "FAN_COMMAND_1";
    case PMBUS_CMD_FAN_COMMAND_2: return "FAN_COMMAND_2";
    case PMBUS_CMD_FAN_CONFIG_3_4: return "FAN_CONFIG_3_4";
    case PMBUS_CMD_FAN_COMMAND_3: return "FAN_COMMAND_3";
    case PMBUS_CMD_FAN_COMMAND_4: return "FAN_COMMAND_4";
    case PMBUS_CMD_VOUT_OV_FAULT_LIMIT: return "VOUT_OV_FAULT_LIMIT";
    case PMBUS_CMD_VOUT_OV_FAULT_RESPONSE: return "VOUT_OV_FAULT_RESPONSE";
    case PMBUS_CMD_VOUT_OV_WARN_LIMIT: return "VOUT_OV_WARN_LIMIT";
    case PMBUS_CMD_VOUT_UV_WARN_LIMIT: return "VOUT_UV_WARN_LIMIT";
    case PMBUS_CMD_VOUT_UV_FAULT_LIMIT: return "VOUT_UV_FAULT_LIMIT";
    case PMBUS_CMD_VOUT_UV_FAULT_RESPONSE: return "VOUT_UV_FAULT_RESPONSE";
    case PMBUS_CMD_IOUT_OC_FAULT_LIMIT: return "IOUT_OC_FAULT_LIMIT";
    case PMBUS_CMD_IOUT_OC_FAULT_RESPONSE: return "IOUT_OC_FAULT_RESPONSE";
    case PMBUS_CMD_IOUT_OC_LV_FAULT_LIMIT: return "IOUT_OC_LV_FAULT_LIMIT";
    case PMBUS_CMD_IOUT_OC_LV_FAULT_RESPONSE: return "IOUT_OC_LV_FAULT_RESPONSE";
    case PMBUS_CMD_IOUT_OC_WARN_LIMIT: return "IOUT_OC_WARN_LIMIT";
    case PMBUS_CMD_IOUT_UC_FAULT_LIMIT: return "IOUT_UC_FAULT_LIMIT";
    case PMBUS_CMD_IOUT_UC_FAULT_RESPONSE: return "IOUT_UC_FAULT_RESPONSE";
    case PMBUS_CMD_OT_FAULT_LIMIT: return "OT_FAULT_LIMIT";
    case PMBUS_CMD_OT_FAULT_RESPONSE: return "OT_FAULT_RESPONSE";
    case PMBUS_CMD_OT_WARN_LIMIT: return "OT_WARN_LIMIT";
    case PMBUS_CMD_UT_WARN_LIMIT: return "UT_WARN_LIMIT";
    case PMBUS_CMD_UT_FAULT_LIMIT: return "UT_FAULT_LIMIT";
    case PMBUS_CMD_UT_FAULT_RESPONSE: return "UT_FAULT_RESPONSE";
    case PMBUS_CMD_VIN_OV_FAULT_LIMIT: return "VIN_OV_FAULT_LIMIT";
    case PMBUS_CMD_VIN_OV_FAULT_RESPONSE: return "VIN_OV_FAULT_RESPONSE";
    case PMBUS_CMD_VIN_OV_WARN_LIMIT: return "VIN_OV_WARN_LIMIT";
    case PMBUS_CMD_VIN_UV_WARN_LIMIT: return "VIN_UV_WARN_LIMIT";
    case PMBUS_CMD_VIN_UV_FAULT_LIMIT: return "VIN_UV_FAULT_LIMIT";
    case PMBUS_CMD_VIN_UV_FAULT_RESPONSE: return "VIN_UV_FAULT_RESPONSE";
    case PMBUS_CMD_IIN_OC_FAULT_LIMIT: return "IIN_OC_FAULT_LIMIT";
    case PMBUS_CMD_IIN_OC_FAULT_RESPONSE: return "IIN_OC_FAULT_RESPONSE";
    case PMBUS_CMD_IIN_OC_WARN_LIMIT: return "IIN_OC_WARN_LIMIT";
    case PMBUS_CMD_POWER_GOOD_ON: return "POWER_GOOD_ON";
    case PMBUS_CMD_POWER_GOOD_OFF: return "POWER_GOOD_OFF";
    case PMBUS_CMD_TON_DELAY: return "TON_DELAY";
    case PMBUS_CMD_TON_RISE: return "TON_RISE";
    case PMBUS_CMD_TON_MAX_FAULT_LIMIT: return "TON_MAX_FAULT_LIMIT";
    case PMBUS_CMD_TON_MAX_FAULT_RESPONSE: return "TON_MAX_FAULT_RESPONSE";
    case PMBUS_CMD_TOFF_DELAY: return "TOFF_DELAY";
    case PMBUS_CMD_TOFF_FALL: return "TOFF_FALL";
    case PMBUS_CMD_TOFF_MAX_WARN_LIMIT: return "TOFF_MAX_WARN_LIMIT";
    case PMBUS_CMD_POUT_OP_FAULT_LIMIT: return "POUT_OP_FAULT_LIMIT";
    case PMBUS_CMD_POUT_OP_FAULT_RESPONSE: return "POUT_OP_FAULT_RESPONSE";
    case PMBUS_CMD_POUT_OP_WARN_LIMIT: return "POUT_OP_WARN_LIMIT";
    case PMBUS_CMD_PIN_OP_WARN_LIMIT: return "PIN_OP_WARN_LIMIT";
    case PMBUS_CMD_STATUS_BYTE: return "STATUS_BYTE";
    case PMBUS_CMD_STATUS_WORD: return "STATUS_WORD";
    case PMBUS_CMD_STATUS_VOUT: return "STATUS_VOUT";
    case PMBUS_CMD_STATUS_IOUT: return "STATUS_IOUT";
    case PMBUS_CMD_STATUS_INPUT: return "STATUS_INPUT";
    case PMBUS_CMD_STATUS_TEMPERATURE: return "STATUS_TEMPERATURE";
    case PMBUS_CMD_STATUS_CML: return "STATUS_CML";
    case PMBUS_CMD_STATUS_OTHER: return "STATUS_OTHER";
    case PMBUS_CMD_STATUS_MFR_SPECIFIC: return "STATUS_MFR_SPECIFIC";
    case PMBUS_CMD_STATUS_FANS_1_2: return "STATUS_FANS_1_2";
    case PMBUS_CMD_STATUS_FANS_3_4: return "STATUS_FANS_3_4";
    case PMBUS_CMD_READ_VIN: return "READ_VIN";
    case PMBUS_CMD_READ_IIN: return "READ_IIN";
    case PMBUS_CMD_READ_VCAP: return "READ_VCAP";
    case PMBUS_CMD_READ_VOUT: return "READ_VOUT";
    case PMBUS_CMD_READ_IOUT: return "READ_IOUT";
    case PMBUS_CMD_READ_TEMPERATURE_1: return "READ_TEMPERATURE_1";
    case PMBUS_CMD_READ_TEMPERATURE_2: return "READ_TEMPERATURE_2";
    case PMBUS_CMD_READ_TEMPERATURE_3: return "READ_TEMPERATURE_3";
    case PMBUS_CMD_READ_FAN_SPEED_1: return "READ_FAN_SPEED_1";
    case PMBUS_CMD_READ_FAN_SPEED_2: return "READ_FAN_SPEED_2";
    case PMBUS_CMD_READ_FAN_SPEED_3: return "READ_FAN_SPEED_3";
    case PMBUS_CMD_READ_FAN_SPEED_4: return "READ_FAN_SPEED_4";
    case PMBUS_CMD_READ_DUTY_CYCLE: return "READ_DUTY_CYCLE";
    case PMBUS_CMD_READ_FREQUENCY: return "READ_FREQUENCY";
    case PMBUS_CMD_READ_POUT: return "READ_POUT";
    case PMBUS_CMD_READ_PIN: return "READ_PIN";
    case PMBUS_CMD_PMBUS_REVISION: return "PMBUS_REVISION";
    case PMBUS_CMD_MFR_ID: return "MFR_ID";
    case PMBUS_CMD_MFR_MODEL: return "MFR_MODEL";
    case PMBUS_CMD_MFR_REVISION: return "MFR_REVISION";
    case PMBUS_CMD_MFR_LOCATION: return "MFR_LOCATION";
    case PMBUS_CMD_MFR_DATE: return "MFR_DATE";
    case PMBUS_CMD_MFR_SERIAL: return "MFR_SERIAL";
    case PMBUS_CMD_MFR_VIN_MIN: return "MFR_VIN_MIN";
    case PMBUS_CMD_MFR_VIN_MAX: return "MFR_VIN_MAX";
    case PMBUS_CMD_MFR_IIN_MAX: return "MFR_IIN_MAX";
    case PMBUS_CMD_MFR_PIN_MAX: return "MFR_PIN_MAX";
    case PMBUS_CMD_MFR_VOUT_MIN: return "MFR_VOUT_MIN";
    case PMBUS_CMD_MFR_VOUT_MAX: return "MFR_VOUT_MAX";
    case PMBUS_CMD_MFR_IOUT_MAX: return "MFR_IOUT_MAX";
    case PMBUS_CMD_MFR_POUT_MAX: return "MFR_POUT_MAX";
    case PMBUS_CMD_MFR_TAMBIENT_MAX: return "MFR_TAMBIENT_MAX";
    case PMBUS_CMD_MFR_TAMBIENT_MIN: return "MFR_TAMBIENT_MIN";
    case PMBUS_CMD_IC_DEVICE_ID: return "IC_DEVICE_ID";
    case PMBUS_CMD_IC_DEVICE_REV: return "IC_DEVICE_REV";
    case PMBUS_CMD_MFR_STATUS_0: return "MFR_STATUS_0";
    case PMBUS_CMD_MFR_STATUS_1: return "MFR_STATUS_1";
    case PMBUS_CMD_MFR_STATUS_2: return "MFR_STATUS_2";
    case PMBUS_CMD_MFR_STATUS_3: return "MFR_STATUS_3";
    case PMBUS_CMD_MFR_STATUS_4: return "MFR_STATUS_4";
    case PMBUS_CMD_FIRST_BLACK_BOX_FAULT_INFO: return "FIRST_BLACK_BOX_FAULT_INFO";
    case PMBUS_CMD_LAST_BLACK_BOX_FAULT_INFO: return "LAST_BLACK_BOX_FAULT_INFO";
    case PMBUS_CMD_RAIL_PROFILE: return "RAIL_PROFILE";
    case PMBUS_CMD_RAIL_STATE: return "RAIL_STATE";
    case PMBUS_CMD_USER_DATA_10: return "USER_DATA_10";
    case PMBUS_CMD_USER_DATA_11: return "USER_DATA_11";
    case PMBUS_CMD_USER_DATA_12: return "USER_DATA_12";
    case PMBUS_CMD_USER_DATA_13: return "USER_DATA_13";
    case PMBUS_CMD_USER_DATA_14: return "USER_DATA_14";
    case PMBUS_CMD_USER_DATA_15: return "USER_DATA_15";
    case PMBUS_CMD_FAULT_PIN_CONFIG: return "FAULT_PIN_CONFIG";
    case PMBUS_CMD_VOUT_CAL_MONITOR: return "VOUT_CAL_MONITOR";
    case PMBUS_CMD_SYSTEM_RESET_CONFIG: return "SYSTEM_RESET_CONFIG";
    case PMBUS_CMD_SYSTEM_WATCHDOG_CONFIG: return "SYSTEM_WATCHDOG_CONFIG";
    case PMBUS_CMD_SYSTEM_WATCHDOG_RESET: return "SYSTEM_WATCHDOG_RESET";
    case PMBUS_CMD_MONITOR_CONFIG: return "MONITOR_CONFIG";
    case PMBUS_CMD_NUM_PAGES: return "NUM_PAGES";
    case PMBUS_CMD_RUN_TIME_CLOCK: return "RUN_TIME_CLOCK";
    case PMBUS_CMD_RUN_TIME_CLOCK_TRIM: return "RUN_TIME_CLOCK_TRIM";
    case PMBUS_CMD_USER_RAM_00: return "USER_RAM_00";
    case PMBUS_CMD_SOFT_RESET: return "SOFT_RESET";
    case PMBUS_CMD_RESET_COUNT: return "RESET_COUNT";
    case PMBUS_CMD_PIN_SELECTED_RAIL_STATES: return "PIN_SELECTED_RAIL_STATES";
    case PMBUS_CMD_RESEQUENCE: return "RESEQUENCE";
    case PMBUS_CMD_CONSTANTS: return "CONSTANTS";
    case PMBUS_CMD_PWM_SELECT: return "PWM_SELECT";
    case PMBUS_CMD_PWM_CONFIG: return "PWM_CONFIG";
    case PMBUS_CMD_PARM_INFO: return "PARM_INFO";
    case PMBUS_CMD_PARM_VALUE: return "PARM_VALUE";
    case PMBUS_CMD_TEMPERATURE_CAL_GAIN: return "TEMPERATURE_CAL_GAIN";
    case PMBUS_CMD_TEMPERATURE_CAL_OFFSET: return "TEMPERATURE_CAL_OFFSET";
    case PMBUS_CMD_SET_BREAKPOINTS: return "SET_BREAKPOINTS";
    case PMBUS_CMD_DEBUG_CONTINUE: return "DEBUG_CONTINUE";
    case PMBUS_CMD_FAULT_RESPONSES: return "FAULT_RESPONSES";
    case PMBUS_CMD_LOGGED_FAULTS: return "LOGGED_FAULTS";
    case PMBUS_CMD_LOGGED_FAULT_DETAIL_INDEX: return "LOGGED_FAULT_DETAIL_INDEX";
    case PMBUS_CMD_LOGGED_FAULT_DETAIL: return "LOGGED_FAULT_DETAIL";
    case PMBUS_CMD_LOGGED_PAGE_PEAKS: return "LOGGED_PAGE_PEAKS";
    case PMBUS_CMD_LOGGED_COMMON_PEAKS: return "LOGGED_COMMON_PEAKS";
    case PMBUS_CMD_LOGGED_FAULT_DETAIL_ENABLES: return "LOGGED_FAULT_DETAIL_ENABLES";
    case PMBUS_CMD_EXECUTE_FLASH: return "EXECUTE_FLASH";
    case PMBUS_CMD_SECURITY: return "SECURITY";
    case PMBUS_CMD_SECURITY_BIT_MASK: return "SECURITY_BIT_MASK";
    case PMBUS_CMD_MFR_STATUS: return "MFR_STATUS";
    case PMBUS_CMD_GPI_FAULT_RESPONSES: return "GPI_FAULT_RESPONSES";
    case PMBUS_CMD_MARGIN_CONFIG: return "MARGIN_CONFIG";
    case PMBUS_CMD_SEQ_CONFIG: return "SEQ_CONFIG";
    case PMBUS_CMD_GPO_CONFIG_INDEX: return "GPO_CONFIG_INDEX";
    case PMBUS_CMD_GPO_CONFIG: return "GPO_CONFIG";
    case PMBUS_CMD_GPI_CONFIG: return "GPI_CONFIG";
    case PMBUS_CMD_GPIO_SELECT: return "GPIO_SELECT";
    case PMBUS_CMD_GPIO_CONFIG: return "GPIO_CONFIG";
    case PMBUS_CMD_MISC_CONFIG: return "MISC_CONFIG";
    case PMBUS_CMD_DEVICE_ID: return "DEVICE_ID";
    case PMBUS_CMD_MFR_SPECIFIC_EXTENDED_COMMAND: return "MFR_SPECIFIC_EXTENDED_COMMAND";
    case PMBUS_CMD_PMBUS_EXTENDED_COMMAND: return "PMBUS_EXTENDED_COMMAND";
    default: return "RESERVED/UNKNOWN";
    }
}

void print_pmbus_cmd(uint8_t x)
{
    pmbus_command_t cmd = (pmbus_command_t) x;
    sprintf(line, "  0x%02X  %s\n", x, pmbus_cmd_text(cmd));
    APP_PRINT(line);
}
