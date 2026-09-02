/*
 *        POWER_MODULE.C
 *
 *  Power sequencer application - rail ADC monitoring.
 *
 */
#include "power_module.h"
#include "simulator.h" //@@@ do something with this
#define DATAFLASH_RECORD_SIZE (BSP_FEATURE_FLASH_HP_DF_BLOCK_SIZE) /* 64 bytes: one data-flash erase block */
#define DATAFLASH_CRC_SIZE    (2)                                  /* CRC-16 appended to the end of every record */

static uint16_t adc_raw_to_mv(uint16_t raw_counts, uint32_t scale_q16);

/* CRC-16/CCITT-FALSE (poly 0x1021, init 0xFFFF) over data[0..len-1] */
static void pwr_dataflash_update(uint8_t *p,uint8_t *data,uint16_t len );
static uint16_t crc16_ccitt(const uint8_t *data, uint32_t len);
static uint16_t crc16_ccitt(const uint8_t *data, uint32_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint32_t i = 0; i < len; i++)
    {
        crc = (uint16_t) (crc ^ ((uint16_t) data[i] << 8));
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            crc = (0 != (crc & 0x8000)) ? (uint16_t) ((crc << 1) ^ 0x1021) : (uint16_t) (crc << 1);
        }
    }
    return crc;
}

/*
    Converts a raw ADC sample to millivolts using a Q16.16 fixed-point scale
    factor, avoiding floating point:
        mV = (raw_counts * scale_q16) >> 16
    scale_q16 is expected to be pre-computed per rail (e.g. from the ADC's
    full-scale count and reference voltage, adjusted for any resistor
    divider on that rail) and stored in power_rail_t.adc_convert.
*/
static uint16_t adc_raw_to_mv(uint16_t raw_counts, uint32_t scale_q16)
{
    uint32_t mv = ((uint32_t) raw_counts * scale_q16) >> 16;
    if (mv > UINT16_MAX)
    {
        mv = UINT16_MAX; /* clamp: guard monitor_mv against a bad/uncalibrated scale factor */
    }
    return (uint16_t) mv;
}

/*
    Poll the rails' analog monitors from one ADC scan.
    ADC_data must have at least PowerController.num_rails entries, indexed
    the same way PowerController.rails[] is (ADC_data[i] is the raw scan
    result for rail i). Rails not configured as PWR_MON_ANALOG are skipped;
    monitor_mv is left as-is for those.
*/
void pwr_mod_poll(uint16_t *ADC_data)
{
    /*
typedef enum power_rail_state_e {
    PWR_RAIL_OFF,
    PWR_RAIL_SEQ_ON, 
    PWR_RAIL_ON,
    PWR_RAIL_MARGIN_HIGH,
    PWR_RAIL_MARGIN_LOW, 
    PWR_RAIL_SEQ_OFF,    
    PWR_RAIL_FAULT
} power_rail_state_t;
 */
    volatile power_rail_t     *rail = PowerController.rails; //@@@ volatile only for debugging, remove later
    volatile power_rail_map_t *map  = PowerController.map; //@@@ volatile only for debugging, remove later
    for (uint8_t i = 0; i < 8; i++) //@@@ hard constant heer
    {
        if (true == rail->cfg->enabled)
        {
        if (PWR_MON_ANALOG == rail->cfg->monitor_type)
        {
            rail->ctrl->monitor_mv = adc_raw_to_mv(ADC_data[map->ADC0_index], rail->ctrl->adc_convert);
            rail->ctrl->monitor_c  = adc_raw_to_mv(ADC_data[map->ADC1_index], rail->ctrl->adc_convert); //@@@ this not right
        }
        switch (rail->ctrl->state)
        {
            case PWR_RAIL_OFF:
                 if (PowerController.ctrl->state == PWR_SEQ_SEQUENCING_UP)
                 {
                     rail->ctrl->cnt = rail->cfg->on_delay_ms * 10;
                     rail->ctrl->state = (rail->ctrl->cnt) ? PWR_RAIL_SEQ_ON : PWR_RAIL_ON ;
                 }
                break;
            case PWR_RAIL_SEQ_ON:
            if (rail->ctrl->cnt == 0)
            {
                rail->ctrl->state = PWR_RAIL_ON;
                     simulator_start(i); /* start the simulator for this rail */
                     rail->ctrl->cnt = rail->cfg->timeout_ms * 10; /* start the timeout counter */
            }
            else
            {
                rail->ctrl->cnt--;
            }
              break;
            case PWR_RAIL_FAULT:
                /* USER CODE: handle faulted rail */
                break;
            case PWR_RAIL_ON:
            rail->ctrl->cnt = (rail->ctrl->cnt == 0) ? 0 : rail->ctrl->cnt - 1;
                 //@@@ check for voltage out of range stuff...
                //@@@ check for voltage out of range stuff...
                /* USER CODE: handle on rail */
                break;
            case PWR_RAIL_MARGIN_HIGH:
                 //@@@ check for voltage out of range stuff...
                 break;
            case PWR_RAIL_MARGIN_LOW:
                 //@@@ check for voltage out of range stuff...
                 break;
            case PWR_RAIL_SEQ_OFF:
                 //@@@ check for voltage out of range stuff...
                 break;
            default:
                /* USER CODE: handle unknown state */
                break;
        }
        rail++;
        map++;  
    }
    }
}
    uint8_t record[DF_POWER_RAIL_RECORD_SIZE];

static void pwr_dataflash_update(uint8_t *p,uint8_t *data,uint16_t len )

{

/*
    Writes one DATAFLASH_RECORD_SIZE-byte record to data flash at address p:
      - the first `len` bytes come from `data`
      - the remaining bytes, up to (DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE), are filled with 0xFF
      - a CRC-16/CCITT over those (DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE) bytes is appended
        as the last DATAFLASH_CRC_SIZE bytes of the record

    `p` must be aligned to DATAFLASH_RECORD_SIZE (the data-flash erase block size), and
    `len` must be <= (DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE). The whole record is staged
    in RAM, the destination block is erased, then the record is written in one shot -
    data flash bits can only be programmed 1->0, so the target must be erased first.
*/
    static bool dataflash_open = false;
    fsp_err_t err;
    flash_status_t status;

    memset(record, 0xFF, sizeof(record));   /* fill remaining bytes with FF */
    memcpy(record, data, len);              /* the caller's data */

    uint16_t crc = crc16_ccitt(record, DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE);
    record[DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE]     = (uint8_t) (crc & 0xFF);
    record[DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE + 1] = (uint8_t) (crc >> 8);

    if (!dataflash_open)
    {
        err = R_FLASH_HP_Open(&g_dataflash_ctrl, &g_dataflash_cfg);
        if (FSP_SUCCESS != err)
        {
            while(1); //@@@ cannot happen...
        }
        dataflash_open = true;
    }

    err = R_FLASH_HP_Erase(&g_dataflash_ctrl, (uint32_t)p, DF_POWER_RAIL_RECORD_SIZE/DATAFLASH_RECORD_SIZE); /* erase the one block this record occupies */
    if (FSP_SUCCESS != err)
    {
        while(1);;
    }
    do /* g_dataflash_cfg.data_flash_bgo == true -> erase/write complete asynchronously */
    {
        R_FLASH_HP_StatusGet(&g_dataflash_ctrl, &status);
    } while (FLASH_STATUS_IDLE != status);

    err = R_FLASH_HP_Write(&g_dataflash_ctrl, (uint32_t) record, (uint32_t)p, DATAFLASH_RECORD_SIZE);
    if (FSP_SUCCESS != err)
    {
        while(1);
    }
    do
    {
        R_FLASH_HP_StatusGet(&g_dataflash_ctrl, &status);
    } while (FLASH_STATUS_IDLE != status);

}
/*
    Verifies the CRC-16/CCITT trailer of a DF_POWER_RAIL_RECORD_SIZE (128)
    byte record in data flash, addressed by `p`. Data flash is memory-mapped
    for reads, so the block is read directly from `p` - no R_FLASH_HP driver
    call needed (unlike pwr_dataflash_update, which must go through the
    driver to erase/program).

    Returns true if the CRC-16 stored in the last DATAFLASH_CRC_SIZE bytes of
    the block matches the CRC computed over the preceding
    (DF_POWER_RAIL_RECORD_SIZE - DATAFLASH_CRC_SIZE) bytes, false otherwise
    (blank/erased flash, corrupted record, etc).
*/
bool pwr_dataflash_check(const uint8_t *p)
{
    uint16_t crc    = crc16_ccitt(p, DF_POWER_RAIL_RECORD_SIZE - DATAFLASH_CRC_SIZE);
    uint16_t stored = (uint16_t) (p[DF_POWER_RAIL_RECORD_SIZE - DATAFLASH_CRC_SIZE]
                                   | (p[DF_POWER_RAIL_RECORD_SIZE - DATAFLASH_CRC_SIZE + 1] << 8));
    return (crc == stored);
}
void pwr_mod_update_config(uint16_t rail_index,uint8_t *data,uint16_t len )
{
    uint32_t p;
    p = DF_POWER_RAIL_CONFIG_ADDR + (DF_POWER_RAIL_RECORD_SIZE * rail_index);
    pwr_dataflash_update((uint8_t*) p,data,len);
}
void pwr_seq_update_config(uint8_t *data,uint16_t len )
{
    uint32_t p;
    p = DF_SEQUENCER_CONFIG_ADDR;
    pwr_dataflash_update((uint8_t*) p,data,len);
}
void pwr_seq_update_map(uint8_t *data,uint16_t len )
{
    uint32_t p;
    p = DF_POWER_RAIL_MAP_ADDR;
    pwr_dataflash_update((uint8_t*) p,data,len);
}
