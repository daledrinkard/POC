/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @addtogroup AT25
 * @{
 **********************************************************************************************************************/

#ifndef R_AT25_H
#define R_AT25_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include <string.h>
#include "r_at25_cfg.h"
#include "r_at25_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define READ_ALL_STATUS_REGS      0x65
#define WRITE_ENABLE              0x06
#define WRITE_STATUS_REG_1        0x01
#define WRITE_DISABLE             0x04
#define WRITE_STATUS_REG_2        0x31
#define READ_STATUS_REG_1         0x05
#define READ_STATUS_REG_2         0x35
#define READ_STATUS_REG_3         0x15
#define AT25FF_DEVICE_ID          0x47
#define JEDEC_ID                  0x9F
#define AT25FF_MANUFACTURER_ID    0x1F


#if 0
#define RESET_ENABLE              0x66
#define RESET                     0x99
#define READ_CONFIG_REG           0x15
#define WRITE_STATUS_REG_3        0x11
#define WRITE_ENABLE_VOL          0x50
#define REMS                      0x90

#define AT25FF_DEVICE_SIZE       (4096 * 1024)
/* sector size of QSPI flash device */
#define AT25FF_SECTOR_SIZE       (4096U)
#define AT25FF_BLOCK_SIZE        (64 * 1024)
#define AT25FF_32K_SIZE          (32 * 1024)
/* QSPI flash page Size */
#define PAGE_WRITE_SIZE                 (256U)
//#define QSPI_DEVICE_START_ADDRESS    (0x60000000)
#endif
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Instance control block. DO NOT INITIALIZE.  Initialization occurs when @ref spi_flash_api_t::open is called */
typedef struct st_at25_instance_ctrl
{
    at25_cfg_t* p_cfg;            // Pointer to initial configuration
    spi_flash_instance_t  * p_qspi;
    uint8_t status_reg[6];
    uint8_t jedec_id[5];
    uint32_t                open;             // Whether or not driver is open
    uint8_t protect;
} at25_instance_ctrl_t;

typedef void at25_ctrl_t;
/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const at25_api_t g_at25_on_at25;

/** @endcond */

fsp_err_t R_AT25_Open(at25_ctrl_t * p_ctrl, at25_cfg_t const * const p_cfg);
fsp_err_t R_AT25_Close(at25_ctrl_t * p_ctrl);
fsp_err_t R_AT25_DirectWrite(at25_ctrl_t    * p_ctrl,
                             uint8_t const * const p_src,
                             uint32_t const        bytes,
                             bool const            read_after_write);
fsp_err_t R_AT25_DirectRead(at25_ctrl_t * p_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t R_AT25_SpiProtocolSet(at25_ctrl_t * p_ctrl, spi_flash_protocol_t spi_protocol);
fsp_err_t R_AT25_XipEnter(at25_ctrl_t * p_ctrl);
fsp_err_t R_AT25_XipExit(at25_ctrl_t * p_ctrl);
fsp_err_t R_AT25_Write(at25_ctrl_t    * p_ctrl,
                       uint8_t const * const p_src,
                       uint8_t * const       p_dest,
                       uint32_t              byte_count);
fsp_err_t R_AT25_Erase(at25_ctrl_t * p_ctrl, uint8_t * const p_device_address, uint32_t byte_count);
fsp_err_t R_AT25_StatusGet(at25_ctrl_t * p_ctrl, spi_flash_status_t * const p_status);
fsp_err_t R_AT25_BankSet(at25_ctrl_t * p_ctrl, uint32_t bank);
fsp_err_t R_AT25_DirectTransfer(at25_ctrl_t                  * p_ctrl,
                                spi_flash_direct_transfer_t * const p_transfer,
                                spi_flash_direct_transfer_dir_t     direction);
fsp_err_t R_AT25_AutoCalibrate(at25_ctrl_t * p_ctrl);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end defgroup QSPI)
 **********************************************************************************************************************/
