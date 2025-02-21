/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_STORAGE_INTERFACES
 * @defgroup SPI_FLASH_API SPI Flash Interface
 * @brief Interface for accessing external SPI flash devices.
 *
 * @section SPI_FLASH_API_SUMMARY Summary
 * The SPI flash API provides an interface that configures, writes, and erases sectors in SPI flash devices.
 * @{
 **********************************************************************************************************************/

#ifndef R_AT25_API_H
#define R_AT25_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_spi_flash_api.h"


/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/


/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
 typedef enum e_at25_device
{
    AT25_DEVICE_1M             = 1, ///< Standard Read Mode (no dummy cycles)
    AT25_DEVICE_2M             = 2, ///< Fast Read Mode (dummy cycles between address and data)
    AT25_DEVICE_4M             = 4, ///< Fast Read Dual Output Mode (data on 2 lines)
    AT25_DEVICE_8M             = 8, ///< Fast Read Dual I/O Mode (address and data on 2 lines)
    AT25_DEVICE_16M            = 16, ///< Fast Read Quad Output Mode (data on 4 lines)
    AT25_DEVICE_32M            = 32, ///< Fast Read Quad I/O Mode (address and data on 4 lines)
    AT25_DEVICE_64M            = 64, ///< Fast Read Quad I/O Mode (address and data on 4 lines)
    AT25_DEVICE_128M            = 128, ///< Fast Read Quad I/O Mode (address and data on 4 lines)
    AT25_DEVICE_256M            = 256, ///< Fast Read Quad I/O Mode (address and data on 4 lines)
    AT25_DEVICE_512M            = 512 ///< Fast Read Quad I/O Mode (address and data on 4 lines)
} at25_device_t;
typedef struct at25_cfg_s {
    spi_flash_instance_t *p_qspi;
	at25_device_t device;
}at25_cfg_t;
/** SPI flash control block.  Allocate an instance specific control block to pass into the SPI flash API calls.
 */
typedef void at25_ctrl_t;



/** SPI flash implementations follow this API. */
typedef struct st_at25_api
{
    /** Open the SPI flash driver module.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_cfg                Pointer to a configuration structure
     **/
    fsp_err_t (* open)(at25_ctrl_t * const p_ctrl, at25_cfg_t const * const p_cfg);

    /** Write raw data to the SPI flash.
     *
     * @param[in] p_ctrl                Pointer to a driver handle
     * @param[in] p_src                 Pointer to raw data to write, must include any required command/address
     * @param[in] bytes                 Number of bytes to write
     * @param[in] read_after_write      If true, the slave select remains asserted and the peripheral does not return
     *                                  to direct communications mode. If false, the slave select is deasserted and
     *                                  memory mapped access is possible after this function returns if the device
     *                                  is not busy.
     **/
    fsp_err_t (* directWrite)(at25_ctrl_t * const p_ctrl, uint8_t const * const p_src, uint32_t const bytes,
                              bool const read_after_write);

    /** Read raw data from the SPI flash. Must follow a call to @ref at25_api_t::directWrite.
     *
     * @param[in]  p_ctrl               Pointer to a driver handle
     * @param[out] p_dest               Pointer to read raw data into
     * @param[in]  bytes                Number of bytes to read
     **/
    fsp_err_t (* directRead)(at25_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const bytes);

    /** Direct Read/Write raw data to the SPI flash.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_data               Pointer to command, address and data values and lengths
     * @param[in] direction            Direct Read/Write
     **/
    fsp_err_t (* directTransfer)(at25_ctrl_t * const p_ctrl, spi_flash_direct_transfer_t * const p_transfer,
                                 spi_flash_direct_transfer_dir_t direction);

    /** Change the SPI protocol in the driver. The application must change the SPI protocol on the device.
     *
     * @param[in] p_ctrl                Pointer to a driver handle
     * @param[in] spi_protocol          Desired SPI protocol
     **/
    fsp_err_t (* spiProtocolSet)(at25_ctrl_t * const p_ctrl, spi_flash_protocol_t spi_protocol);

    /** Program a page of data to the flash.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_src                The memory address of the data to write to the flash device
     * @param[in] p_dest               The location in the flash device address space to write the data to
     * @param[in] byte_count           The number of bytes to write
     **/
    fsp_err_t (* write)(at25_ctrl_t * const p_ctrl, uint8_t const * const p_src, uint8_t * const p_dest,
                        uint32_t byte_count);

    /** Erase a certain number of bytes of the flash.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] p_device_address     The location in the flash device address space to start the erase from
     * @param[in] byte_count           The number of bytes to erase. Set to SPI_FLASH_ERASE_SIZE_CHIP_ERASE to erase entire
     *                                 chip.
     **/
    fsp_err_t (* erase)(at25_ctrl_t * const p_ctrl, uint8_t * const p_device_address, uint32_t byte_count);

    /** Get the write or erase status of the flash.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[out] p_status            Current status of the SPI flash device stored here.
     **/
    fsp_err_t (* statusGet)(at25_ctrl_t * const p_ctrl, spi_flash_status_t * const p_status);

    /** Enter XIP mode.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* xipEnter)(at25_ctrl_t * const p_ctrl);

    /** Exit XIP mode.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* xipExit)(at25_ctrl_t * const p_ctrl);

    /** Select the bank to access.  See implementation for details.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     * @param[in] bank                 The bank number
     **/
    fsp_err_t (* bankSet)(at25_ctrl_t * const p_ctrl, uint32_t bank);

    /** AutoCalibrate the SPI flash driver module. Expected to be used when auto-calibrating OSPI RAM device.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* autoCalibrate)(at25_ctrl_t * const p_ctrl);

    /** Close the SPI flash driver module.
     *
     * @param[in] p_ctrl               Pointer to a driver handle
     **/
    fsp_err_t (* close)(at25_ctrl_t * const p_ctrl);
} at25_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_at25_instance
{
    at25_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    at25_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    at25_api_t const * p_api;     ///< Pointer to the API structure for this instance
} at25_instance_t;

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*******************************************************************************************************************//**
 * @} (end defgroup SPI_FLASH_API)
 **********************************************************************************************************************/
