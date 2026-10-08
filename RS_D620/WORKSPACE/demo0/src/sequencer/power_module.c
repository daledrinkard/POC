/*
 *        POWER_MODULE.C
 *
 *  Power sequencer application - rail ADC monitoring.
 *
 */
#include "power_module.h"
#include "simulator.h"                                             //@@@ do something with this
#define DATAFLASH_RECORD_SIZE (BSP_FEATURE_FLASH_HP_DF_BLOCK_SIZE) /* 64 bytes: one data-flash erase block */
#define DATAFLASH_CRC_SIZE (2)                                     /* CRC-16 appended to the end of every record */

static uint16_t adc_raw_to_mv(uint16_t raw_counts, uint32_t scale_q16);

/* CRC-16/CCITT-FALSE (poly 0x1021, init 0xFFFF) over data[0..len-1] */
static void pwr_dataflash_update(uint8_t *p, uint8_t *data, uint16_t len);
static uint16_t crc16_ccitt(const uint8_t *data, uint32_t len);
static uint16_t crc16_ccitt(const uint8_t *data, uint32_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint32_t i = 0; i < len; i++)
    {
        crc = (uint16_t)(crc ^ ((uint16_t)data[i] << 8));
        for (uint8_t bit = 0; bit < 8; bit++)
        {
            crc = (0 != (crc & 0x8000)) ? (uint16_t)((crc << 1) ^ 0x1021) : (uint16_t)(crc << 1);
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
    uint32_t mv = ((uint32_t)raw_counts * scale_q16) >> 16;
    if (mv > UINT16_MAX)
    {
        mv = UINT16_MAX; /* clamp: guard monitor_mv against a bad/uncalibrated scale factor */
    }
    return (uint16_t)mv;
}
void pwr_seq_poll_GPI(void)
{
    PowerController.ctrl->GPI = 0;
    for (int i = 0; i < 32; i++)
    {
        switch (PowerController.cfg->GPI_config.GPI[i].conf & 0x0F)
        { //@@@ could access 16 and mask upper
        /*  ACTIVE LOW   */
        case (1):                                                       /* input */
            if (0 == (*((uint32_t *)power_sequencer_IO.pin[i]) & 0x02)) //@@@ PIDR bit is low in the PFS
            {
                PowerController.ctrl->GPI |= (1 << i);
            }
            break;
        /* ACTIVE HIGH */
        case (5):                                                       /* input */
            if (0 != (*((uint32_t *)power_sequencer_IO.pin[i]) & 0x02)) //@@@ PIDR bit is low in the PFS
            {
                PowerController.ctrl->GPI |= (1 << i);
            }
            break;
        default:
            break;
        }
    }
}
void pwr_seq_poll_Page(void)
{
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
    volatile power_rail_t *rail = PowerController.rails;  //@@@ volatile only for debugging, remove later
    volatile power_rail_map_t *map = PowerController.map; //@@@ volatile only for debugging, remove later
}

uint8_t record[DF_POWER_LARGEST_SIZE];

static void pwr_dataflash_update(uint8_t *p, uint8_t *data, uint16_t len)

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
    volatile uint16_t blkcnt;
    blkcnt = ((len / DATAFLASH_RECORD_SIZE) + 1);
    static bool dataflash_open = false;
    fsp_err_t err;
    flash_status_t status;
    //@@@ could be more efficient
#if 1                                                     //@@@ fill unused space with FF
    memset(record, 0xFF, blkcnt * DATAFLASH_RECORD_SIZE); /* fill remaining bytes with FF */
#endif
    memcpy(record, data, len); /* the caller's data */

#if 0
    uint16_t crc = crc16_ccitt(record, DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE);
    record[DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE]     = (uint8_t) (crc & 0xFF);
    record[DATAFLASH_RECORD_SIZE - DATAFLASH_CRC_SIZE + 1] = (uint8_t) (crc >> 8);
#endif
    if (!dataflash_open)
    {
        err = R_FLASH_HP_Open(&g_dataflash_ctrl, &g_dataflash_cfg);
        if (FSP_SUCCESS != err)
        {
            while (1)
                ; //@@@ cannot happen...
        }
        dataflash_open = true;
    }

    err = R_FLASH_HP_Erase(&g_dataflash_ctrl, (uint32_t)p, blkcnt); /* erase the one block this record occupies */
                                                                    //    err = R_FLASH_HP_Erase(&g_dataflash_ctrl, 0x08000000, 1); /* erase the one block this record occupies */
    if (FSP_SUCCESS != err)
    {
        while (1)
            ;
    }
    do /* g_dataflash_cfg.data_flash_bgo == true -> erase/write complete asynchronously */
    {
        R_FLASH_HP_StatusGet(&g_dataflash_ctrl, &status);
    } while (FLASH_STATUS_IDLE != status);

#if 1 //@@@ fill space with FF
    err = R_FLASH_HP_Write(&g_dataflash_ctrl, (uint32_t)record, (uint32_t)p, blkcnt * DATAFLASH_RECORD_SIZE);
#else
    err = R_FLASH_HP_Write(&g_dataflash_ctrl, (uint32_t)record, (uint32_t)p, len);
#endif
    if (FSP_SUCCESS != err)
    {
        while (1)
            ;
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
    uint16_t crc = crc16_ccitt(p, DF_POWER_RAIL_RECORD_SIZE - DATAFLASH_CRC_SIZE);
    uint16_t stored = (uint16_t)(p[DF_POWER_RAIL_RECORD_SIZE - DATAFLASH_CRC_SIZE] | (p[DF_POWER_RAIL_RECORD_SIZE - DATAFLASH_CRC_SIZE + 1] << 8));
    return (crc == stored);
}
void pwr_rail_store_config(uint16_t rail_index, uint8_t *data, uint16_t len)
{
    uint32_t p;
    p = DF_POWER_RAIL_CONFIG_ADDR + (DF_POWER_RAIL_RECORD_SIZE * rail_index);
    pwr_dataflash_update((uint8_t *)p, data, len);
}
void pwr_seq_store_config(uint8_t *data, uint16_t len)
{
    uint32_t p;
    p = DF_SEQUENCER_CONFIG_ADDR;
    pwr_dataflash_update((uint8_t *)p, data, len);
}
uint16_t pwr_seq_read_config(uint8_t *data, uint16_t len)
{
    memcpy(data, PowerController.cfg, sizeof(power_controller_cfg_t)); /* read from RAM*/
                                                                       //    memcpy(data,PowerController.cfg_store,sizeof(power_controller_cfg_t)); /* read from Dataflash */
    return sizeof(power_controller_cfg_t);
}
void pwr_seq_store_map(uint8_t *data, uint16_t len)
{
    uint32_t p;
    p = DF_POWER_RAIL_PINMAP_ADDR;
    pwr_dataflash_update((uint8_t *)p, data, len);
}
void pwr_seq_store_fault_config(uint8_t *data, uint16_t len)
{
    uint32_t p;
    p = DF_POWER_RAIL_FLTMAP_ADDR;
    pwr_dataflash_update((uint8_t *)p, data, len);
}
/*
    Reads the fault configuration record back from data flash at
    DF_POWER_RAIL_FLTMAP_ADDR into `data`. Data flash is memory-mapped for
    reads, so this copies directly from the address - no R_FLASH_HP driver
    call needed (see pwr_dataflash_check()).

    Returns the number of bytes copied (DF_POWER_FLTMAP_SIZE).
*/
uint16_t pwr_seq_read_fault_config(uint8_t *data)
{
    memcpy(data, (uint8_t *)DF_POWER_RAIL_FLTMAP_ADDR, 41); //@@@ can't use sizeof()
    return 41;
}
void pwr_seq_store_all(void)
{
    pwr_seq_store_config((uint8_t *)PowerController.cfg, sizeof(power_controller_cfg_t));
    for (int i = 0; i < PWR_MAX_RAILS; i++)
    {
        pwr_rail_store_config((uint16_t)i, (uint8_t *)PowerController.rails[i].cfg, sizeof(power_rail_cfg_t));
    }
}
void pwr_seq_restore_all(void)
{
    //    pwr_seq_store_config((uint8_t*) PowerController.cfg,sizeof(power_controller_cfg_t));
    memcpy((uint8_t *)PowerController.cfg, (uint8_t *)PowerController.cfg_store, sizeof(power_controller_cfg_t));
    for (int i = 0; i < PWR_MAX_RAILS; i++)
    {
        memcpy((uint8_t *)PowerController.rails[i].cfg, (uint8_t *)PowerController.rails[i].cfg_store, sizeof(power_rail_cfg_t));
    }
}
/*  This is called right after the dataflash is copied into working ram */
void pwr_seq_configure(void)
{
    /* configure the monitor pins */
    for (int i = 0; i < PWR_MAX_MONITOR; i++)
    {
        switch (i)
        {       // PowerController.cfg->monitor.MON[i]) {
        case 0: /* see table 26.8    No Monitor*/
            // PWR_PIN_CFG_INPUT(i);
            break;
        case 1: /* Analog */
            PWR_PIN_CFG_ANALOG(i);
            break;
        case 2: /* Temperature */
            break;
        case 3: /* Current */
            break;
        case 4: /* Voltage Comparator */
            break;
        case 5: /* Input voltage */
            break;
        case 6: /* Digital voltage monitor */ //@@@ based off interrupts?  These are not DMON configurations are they?
            PWR_PIN_CFG_INPUT(i);
            break;
        default:
            break;
        }
    }
    /* configure the EN pins */
    for (int i = 0; i < PWR_MAX_RAILS; i++)
    {
        uint8_t ID = PowerController.rails[i].cfg->SEQ_config.ID;
        uint8_t conf = (PowerController.rails[i].cfg->SEQ_config.ID_other & 0x07);
        if (ID)
            switch (conf)
            {
            /* Active Low */
            case (0): /* unused */
                break;
            case (1): /* input*/
                break;
            case (2):                        /* ACTIVE driven */
                PWR_PIN_CFG_OUTPUT1(ID - 1); /* sets the pin as an output and initializes it high*/
                break;
            case (3):                    /* Open Drain */
                PWR_PIN_CFG_OD1(ID - 1); /* sets the pin as an open drain output and initializes it pulled high */
                break;
            /* Active High */
            case (4): /* unused */
                break;
            case (5): /* input*/
                break;
            case (6):                        /* ACTIVE driven*/
                PWR_PIN_CFG_OUTPUT0(ID - 1); /* sets the pin as an output and drives it LOW */
                break;
            case (7):
                PWR_PIN_CFG_OD0(ID - 1); /* sets the pin as an open drain output and initializes it driven LOW */
                break;
            }
    }

    /* configure the GPI pins */
    for (int i = 0; i < 32; i++) //@@@ hard coding 32 becaus that's the architecture.
    {
        uint8_t ID = PowerController.cfg->GPI_config.GPI[i].id;
        uint8_t conf = (PowerController.cfg->GPI_config.GPI[i].conf & 0x07);
        if (ID)
            switch (conf)
            {
            case (1):                      /* ACTIVE LOW input */
            case (5):                      /* ACTIVE HIGH input */
                PWR_PIN_CFG_INPUT(ID - 1); /* sets the pin as an input with pull-up */
                break;
            default:
                break;
            }
    }
    /* configure the GPO pins */
    for (int i = 0; i < 16; i++) //@@@ arbitrary, 
    {
        uint8_t ID = PowerController.cfg->GPO_config[i].GPO.id;           //GPIO_config.GPIO[i].id;
        uint8_t conf = (PowerController.cfg->GPO_config[i].GPO.conf & 0x07);
        if (ID)
            switch (conf)
            {
            case (1):                      /* ACTIVE LOW input */
            case (5):                      /* ACTIVE HIGH input */
                PWR_PIN_CFG_INPUT(ID - 1); /* sets the pin as an input with pull-up */
                break;
                /* ACTIVE LOW so initialize them high */
            case (2):                        
                PWR_PIN_CFG_OUTPUT1(ID - 1); 
                break;
            case (3):                        
                PWR_PIN_CFG_OD1(ID - 1); 
                break;
                /* ACTIVE HI so initialize them low */
            case (6):  
                PWR_PIN_CFG_OUTPUT0(ID - 1); /* sets the pin as an output and initializes it low*/
                break;
            case (7):  
                PWR_PIN_CFG_OD0(ID - 1); /* sets the pin as an output and initializes it low*/
                break;
            default:
                break;
            }
    }
}
// void pwr_seq_update_monitor(uint8_t *data,uint16_t len)
//{
//    memcpy((uint8_t*) &PowerController.cfg->monitor,data,len);
// }
void pwr_seq_update_cfg(uint8_t *p, uint8_t *q, uint16_t len)
{
    memcpy(p, q, len);
}
void pwr_seq_update_GPIO_cfg(uint8_t *p)
{
    /* see section 26.41  allows you to manipulate a GPIO */
    PowerController.ctrl->GPIO_config = *p;
    /*   7    6    5    4    3    2    1    0  */
    /*   .    .    .    .   STS OVAL  OEN  EN  */
}
uint16_t pwr_seq_read_GPIO_cfg(uint8_t *p)
{
    *p = PowerController.ctrl->GPIO_config;
    return 1;
}
volatile power_controller_SEQCFG_t *px;
void pwr_seq_update_seqcfg(uint8_t rail_index, uint8_t *src, uint16_t data_len)
{
    //@@@   power_controller_SEQCFG_t *p = &PowerController.rails[rail_index].cfg->SEQ_config;
    //@@@  parameter check of data_len
    px = &PowerController.rails[rail_index].cfg->SEQ_config;
    uint8_t *p = (uint8_t *)px;
    p = p + 2; //@@@ skip the pad at the beginning of the data structure.
    memcpy((uint8_t *)p, src, 13);
    p += 14;
    src += 13;
    memcpy((uint8_t *)p, src, 16);
}
uint16_t pwr_seq_read_seqcfg(uint8_t rail_index, uint8_t *src)
{
    //@@@   power_controller_SEQCFG_t *p = &PowerController.rails[rail_index].cfg->SEQ_config;
    //@@@  parameter check of data_len
    px = &PowerController.rails[rail_index].cfg->SEQ_config;
    uint8_t *p = (uint8_t *)px;
    p = p + 2; //@@@ skip the pad at the beginning of the data structure.
    memcpy(src, (uint8_t *)p, 13);
    p += 14;
    src += 13;
    memcpy(src, (uint8_t *)p, 16);
    return 29;
}
void pwr_seq_update_railstate(uint8_t *src, uint16_t data_len)
{
    //@@@   power_controller_SEQCFG_t *p = &PowerController.rails[rail_index].cfg->SEQ_config;
    //@@@  parameter check of data_len
    uint8_t *p = (uint8_t *)&PowerController.cfg->railstate;
    p = p + 2; //@@@ skip the pad at the beginning of the data structure.
    memcpy((uint8_t *)p, src, 34);
}
uint16_t pwr_seq_read_railstate(uint8_t *src)
{
    //@@@   power_controller_SEQCFG_t *p = &PowerController.rails[rail_index].cfg->SEQ_config;
    //@@@  parameter check of data_len
    uint8_t *p = (uint8_t *)&PowerController.cfg->railstate;
    p = p + 2; //@@@ skip the pad at the beginning of the data structure.
    memcpy(src, (uint8_t *)p, 34);
    return 34;
}
void pwr_seq_process_operation(uint8_t rail_index,uint8_t op)
{
    uint8_t ID = PowerController.rails[rail_index].cfg->SEQ_config.ID;
    uint8_t conf = (PowerController.rails[rail_index].cfg->SEQ_config.ID_other & 0x07);

    PowerController.ctrl->operation = op;
    /*
          op:7 ON/OFF State
             6 Turn off behavior
             5:4 Voltage command source
             3:2 Marin fault response
             1  Transistion control
             0  RSVD
    */
   if (op&0x80) {               /* BIT 7: ON/OFF State */
     /* turn the rail ON*/
     switch(op&0x30) { /* BIT 5:4 Voltage command source */
       case 0x00: /* PMBus VOUT_COMMAND */
         break;
       case 0x10: /* PMBus VOUT_MARGIN_LOW */
         break;
       case 0x20: /* PMBus VOUT_MARGIN_HIGH */
         break;
       case 0x30: /* PMBus AVS_VOUT */
         break;
     }
     switch(op&0x0C) { /* BIT 3:2 Marin fault response */
       case 0x00: /* No action */
         break;
       case 0x04: /* Margining fault response */
         break;
       case 0x08: /* Margining fault response */
         break;
       case 0x0C: /* Margining fault response */
         break;
     }
   } else
   {
       if (op&0x40) {            /* BIT 6: Turn off behavior */
         /* use T_ON and T_OFF*/
       } else
       {
         /* turn off IMMEDIATELY */
       }
    }
}