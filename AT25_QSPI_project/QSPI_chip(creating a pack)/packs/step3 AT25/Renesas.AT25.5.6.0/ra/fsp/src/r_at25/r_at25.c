/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "bsp_api.h"

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_at25.h"
#include "r_qspi.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if 0
#define QSPI_PRV_OPEN                         (0x51535049)

#define QSPI_PRV_MASK_UPPER_4_BITS_OF_BYTE    (0xF0U)
#define QSPI_PRV_MASK_LOWER_4_BITS_OF_BYTE    (0x0FU)

#define QSPI_PRV_LSB_NIBBLE_CLEARED           (0xEEEEEEEE)
#define QSPI_PRV_EVEN_BITS_CLEARED            (0xAAAA)
#endif
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Number of address bytes in 4 byte address mode. */
#define QSPI_4_BYTE_ADDRESS                   (4U)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static fsp_err_t at25_sub_setup_qspi(at25_ctrl_t * p_ctrl);
static fsp_err_t at25_sub_writeread(spi_flash_ctrl_t *p_ctrl,uint8_t *cmd,uint8_t cmd_len,uint8_t *rdat,uint32_t rdat_len);
static fsp_err_t get_flash_status(spi_flash_ctrl_t * p_ctrl);
#if AT25_CFG_PARAM_CHECKING_ENABLE

static fsp_err_t r_qspi_param_checking_dcom(qspi_instance_ctrl_t * p_instance_ctrl);

static fsp_err_t qspi_program_param_check(qspi_instance_ctrl_t * p_instance_ctrl,
                                          uint8_t const * const  p_src,
                                          uint8_t * const        p_dest,
                                          uint32_t               byte_count);

#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

#if QSPI_CFG_SUPPORT_EXTENDED_SPI_MULTI_LINE_PROGRAM

/* Page program command.  Index by [data_lines].  data_lines is 0 for 1 data line, 1 for 2 data lines, or
 * 2 for 4 data lines. */
static void(*const gp_qspi_prv_byte_write[3]) (uint8_t byte) =
{
    qspi_d0_byte_write_standard,
    qspi_d0_byte_write_dual_mode,
    qspi_d0_byte_write_quad_mode
};
#endif

/*******************************************************************************************************************//**
 * @addtogroup QSPI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

const spi_flash_api_t g_qspi_on_spi_flash =
{
    .open           = R_AT25_Open,
    .directWrite    = R_AT25_DirectWrite,
    .directRead     = R_AT25_DirectRead,
    .directTransfer = R_AT25_DirectTransfer,
    .spiProtocolSet = R_AT25_SpiProtocolSet,
    .write          = R_AT25_Write,
    .erase          = R_AT25_Erase,
    .statusGet      = R_AT25_StatusGet,
    .xipEnter       = R_AT25_XipEnter,
    .xipExit        = R_AT25_XipExit,
    .bankSet        = R_AT25_BankSet,
    .autoCalibrate  = R_AT25_AutoCalibrate,
    .close          = R_AT25_Close,
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Open the QSPI driver module. After the driver is open, the QSPI can be accessed like internal flash memory starting
 * at address 0x60000000.
 *
 * Implements @ref spi_flash_api_t::open.
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       The parameter p_instance_ctrl or p_cfg is NULL.
 * @retval FSP_ERR_ALREADY_OPEN    Driver has already been opened with the same p_instance_ctrl.
 **********************************************************************************************************************/
fsp_err_t R_AT25_Open (spi_flash_ctrl_t * p_ctrl, spi_flash_cfg_t const * const p_cfg)
{
    qspi_instance_ctrl_t * p_instance_ctrl = (qspi_instance_ctrl_t *) p_ctrl;

#if QSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ERROR_RETURN(QSPI_PRV_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT((p_cfg->dummy_clocks >= SPI_FLASH_DUMMY_CLOCKS_3 && p_cfg->dummy_clocks <= SPI_FLASH_DUMMY_CLOCKS_17) ||
               p_cfg->dummy_clocks == SPI_FLASH_DUMMY_CLOCKS_DEFAULT);
#endif

   return gp_mcuboot_xspi_instance->p_api->open(p_ctrl, p_cfg);
   return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Writes raw data directly to the QSPI.
 *
 * Implements @ref spi_flash_api_t::directWrite.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_AT25_DirectWrite (spi_flash_ctrl_t    * p_ctrl,
                              uint8_t const * const p_src,
                              uint32_t const        bytes,
                              bool const            read_after_write)
{
    return gp_mcuboot_xspi_instance->p_api->directWrite(p_ctrl,p_src,bytes,read_after_write);
}

/*******************************************************************************************************************//**
 * Reads raw data directly from the QSPI. This API can only be called after R_QSPI_DirectWrite with read_after_write
 * set to true.
 *
 * Implements @ref spi_flash_api_t::directRead.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function must be called after R_QSPI_DirectWrite with read_after_write set
 *                                     to true.
 **********************************************************************************************************************/
fsp_err_t R_AT25_DirectRead (spi_flash_ctrl_t * p_ctrl, uint8_t * const p_dest, uint32_t const bytes)
{
    return gp_mcuboot_xspi_instance->p_api->directRead(p_ctrl,p_dest,bytes);
}

/*******************************************************************************************************************//**
 * Read/Write raw data directly with the OctaFlash/OctaRAM device. Unsupported by QSPI.
 *
 * Implements @ref spi_flash_api_t::directTransfer.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by QSPI.
 **********************************************************************************************************************/
fsp_err_t R_AT25_DirectTransfer (spi_flash_ctrl_t                  * p_ctrl,
                                 spi_flash_direct_transfer_t * const p_transfer,
                                 spi_flash_direct_transfer_dir_t     direction)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_transfer);
    FSP_PARAMETER_NOT_USED(direction);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Enters XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipEnter.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_AT25_XipEnter (spi_flash_ctrl_t * p_ctrl)
{
    return gp_mcuboot_xspi_instance->p_api->xipEnter(p_ctrl);
}

/*******************************************************************************************************************//**
 * Exits XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipExit.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_AT25_XipExit (spi_flash_ctrl_t * p_ctrl)
{
    return gp_mcuboot_xspi_instance->p_api->xipExit(p_ctrl);
}
/*******************************************************************************************************************//**
 * Program a page of data to the flash.
 *
 * Implements @ref spi_flash_api_t::write.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl, p_dest or p_src is NULL, or byte_count crosses a page boundary.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_AT25_Write (spi_flash_ctrl_t    * p_ctrl,
                        uint8_t const * const p_src,
                        uint8_t * const       p_dest,
                        uint32_t              byte_count)
{
    return gp_mcuboot_xspi_instance->p_api->write(p_ctrl,p_src,p_dest,byte_count);
}

/*******************************************************************************************************************//**
 * Erase a block or sector of flash.  The byte_count must exactly match one of the erase sizes defined in spi_flash_cfg_t.
 * For chip erase, byte_count must be SPI_FLASH_ERASE_SIZE_CHIP_ERASE.
 *
 * Implements @ref spi_flash_api_t::erase.
 *
 * @retval FSP_SUCCESS                 The command to erase the flash was executed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl or p_device_address is NULL, or byte_count doesn't match an erase
 *                                     size defined in spi_flash_cfg_t, or device is in XIP mode.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_AT25_Erase (spi_flash_ctrl_t * p_ctrl, uint8_t * const p_device_address, uint32_t byte_count)
{
    return gp_mcuboot_xspi_instance->p_api->erase(p_ctrl,p_device_address, byte_count);
}

/*******************************************************************************************************************//**
 * Gets the write or erase status of the flash.
 *
 * Implements @ref spi_flash_api_t::statusGet.
 *
 * @retval FSP_SUCCESS                 The write status is in p_status.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl or p_status is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when XIP mode is enabled.
 **********************************************************************************************************************/
fsp_err_t R_AT25_StatusGet (spi_flash_ctrl_t * p_ctrl, spi_flash_status_t * const p_status)
{
    return gp_mcuboot_xspi_instance->p_api->statusGet(p_ctrl,p_status);
}

/*******************************************************************************************************************//**
 * Selects the bank to access. A bank is a 64MB sliding access window into the QSPI device flash memory space. To access
 * chip address 0x4000000, select bank 1, then read from internal flash address 0x60000000. To access chip address
 * 0x8001000, select bank 2, then read from internal flash address 0x60001000.
 *
 * This function is not required for memory devices less than or equal to 512 Mb (64MB).
 *
 * Implements @ref spi_flash_api_t::bankSet.
 *
 * @retval FSP_SUCCESS                 Bank successfully selected.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_AT25_BankSet (spi_flash_ctrl_t * p_ctrl, uint32_t bank)
{
    return gp_mcuboot_xspi_instance->p_api->bankSet(p_ctrl, bank);
}

/*******************************************************************************************************************//**
 * Sets the SPI protocol.
 *
 * Implements @ref spi_flash_api_t::spiProtocolSet.
 *
 * @retval FSP_SUCCESS                SPI protocol updated on MCU peripheral.
 * @retval FSP_ERR_ASSERTION          A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN           Driver is not opened.
 * @retval FSP_ERR_INVALID_ARGUMENT   Invalid SPI protocol requested.
 **********************************************************************************************************************/
fsp_err_t R_AT25_SpiProtocolSet (spi_flash_ctrl_t * p_ctrl, spi_flash_protocol_t spi_protocol)
{
    return gp_mcuboot_xspi_instance->p_api->spiProtocolSet(p_ctrl,spi_protocol);
}

/*******************************************************************************************************************//**
 * Auto-calibrate the OctaRAM device using the preamble pattern. Unsupported by QSPI.
 * Implements @ref spi_flash_api_t::autoCalibrate.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by QSPI
 **********************************************************************************************************************/
fsp_err_t R_AT25_AutoCalibrate (spi_flash_ctrl_t * p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Close the QSPI driver module.
 *
 * Implements @ref spi_flash_api_t::close.
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_instance_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN        Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_AT25_Close (spi_flash_ctrl_t * p_ctrl)
{
    return gp_mcuboot_xspi_instance->p_api->close(p_ctrl);
}


/**
 * at25_sub_setup_qspi  handles initializing the qspi for operation
 * @param p_at25_ctrl  control structure
 * @return error
 */
static fsp_err_t at25_sub_setup_qspi(at25_ctrl_t * p_at25_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    uint8_t write_data[4];
    uint8_t read_data[8];
    uint8_t status_reg  = 0;
    spi_flash_ctrl_t *p_ctrl = p_at25_ctrl->p_qspi_flash->p_ctrl;
    /* Note: The configuration settings in QSPI module is extended SPI mode, Fast Read Quad Output */
    AT25_ASSERT(R_QSPI_Open(p_ctrl, p_at25_ctrl->p_qspi_flash->p_cfg));
    memset(read_data, 0, sizeof(read_data));
    write_data[0] = JEDEC_ID; /* Read ID */
    AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,1,p_at25_ctrl->jedec_id,4)); //@@
    FSP_ASSERT ((AT25FF_MANUFACTURER_ID == p_at25_ctrl->jedec_id[0])&&
                (AT25FF_DEVICE_ID       == p_at25_ctrl->jedec_id[1]));
    /* Read all the status registers */
    write_data[0] = READ_ALL_STATUS_REGS;
    for (uint8_t i = 1; i<6; i++) /* registers are 1 to 5, not 0 to 4, status_reg[0] is a dummy */
    {
        write_data[1] = i;
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,3,&p_at25_ctrl->status_reg[i],1));
    }
    /* check if the protection status is correct */
    /* if it is not, then make it so */
    if ( ((p_at25_ctrl->protect & 0x1F) != ((p_at25_ctrl->status_reg[1] & 0x7C)>>2)) ||
         ((p_at25_ctrl->protect & 0x20) != ((p_at25_ctrl->status_reg[2] & 0x40)>>1))
       )
    {
        p_at25_ctrl->status_reg[1] &= (uint8_t) ~0x7C; // clear the protection fields
        p_at25_ctrl->status_reg[1] |= (p_at25_ctrl->protect & 0x1F)<<2; // set according to protect
        p_at25_ctrl->status_reg[2] &= (uint8_t) ~0x40; // clear CMPRT bit
        p_at25_ctrl->status_reg[2] |= (p_at25_ctrl->protect & 0x20)<<1; // set according to protect
        write_data[0] = WRITE_ENABLE; /* Write volatile and non-volatile status registers */
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,1,NULL,0));
        write_data[0] = WRITE_STATUS_REG_1;  /* writes both status registers 1 and 2 */
        write_data[1] = p_at25_ctrl->status_reg[1];
        write_data[2] = p_at25_ctrl->status_reg[2];
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,3,NULL,0));
        AT25_ASSERT(get_flash_status(p_ctrl));

        write_data[0] = WRITE_DISABLE; /* Write Disable */
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,1,NULL,0));
        /* Read all the status registers again to check success*/
        write_data[0] = READ_ALL_STATUS_REGS;
        for (uint8_t i = 1;i<6;i++)
        {
            write_data[1] = i;
            AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,3,&p_at25_ctrl->status_reg[i],1));
        }
        /* make the same check */
        FSP_ASSERT ( ((p_at25_ctrl->protect & 0x1F) == ((p_at25_ctrl->status_reg[1] & 0x7C)>>2)) &&
                     ((p_at25_ctrl->protect & 0x20) == ((p_at25_ctrl->status_reg[2] & 0x40)>>1))
                   );
    }
    /* Set the QE bit in the status register if not already set */
    if ((1<<1) != (p_at25_ctrl->status_reg[2] & (1<<1)))
    {
        write_data[0] = WRITE_ENABLE; /* Write Enable */
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,1,NULL,0));
        write_data[0] = WRITE_STATUS_REG_2; /* Write Status register */
        write_data[1] = (uint8_t)(status_reg | (1<<1)); /* Set QE bit = 1 */
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,2,NULL,0));
        AT25_ASSERT(get_flash_status(p_ctrl));
        write_data[0] = WRITE_DISABLE; /* Write disable */
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,1,NULL,0));
        /* Read status reg 2 again */
        write_data[0] = READ_STATUS_REG_2;
        AT25_ASSERT(at25_sub_writeread(p_ctrl,write_data,1,&p_at25_ctrl->status_reg[2],1));
        /* Check if Quad mode is enabled */
        FSP_ASSERT ((1<<1) == (p_at25_ctrl->status_reg[2] & (1<<1)));
    }
    /* Close QSPI QSPI module here if opening again in a file system/framework that utilize QSPI */
    /*err = R_QSPI_Close(p_ctrl);
    if (FSP_SUCCESS != err)
    {
      return err;
    }*/
    return err;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup AT25)
 **********************************************************************************************************************/

