/*
 * mipi.h
 *
 *  Created on: Aug 6, 2024
 *      Author: daled
 */

#ifndef MIPI_MIPI_H_
#define MIPI_MIPI_H_
#include "r_mipi_dsi_api.h"
#define MIPI_DSI_DISPLAY_CONFIG_DATA_DELAY_FLAG      ((mipi_dsi_cmd_id_t) 0xFE)
#define MIPI_DSI_DISPLAY_CONFIG_DATA_END_OF_TABLE    ((mipi_dsi_cmd_id_t) 0xFD)
typedef struct
{
    unsigned char        size;
    unsigned char        buffer[10];
    mipi_dsi_cmd_id_t    cmd_id;
    mipi_dsi_cmd_flag_t flags;
} lcd_table_setting_t;
void mipi_dsi_push_table (const lcd_table_setting_t *table);
#endif /* MIPI_MIPI_H_ */
