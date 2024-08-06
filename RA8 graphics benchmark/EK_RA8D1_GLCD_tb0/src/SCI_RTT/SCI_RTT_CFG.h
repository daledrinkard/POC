/*
 * SCI_RTT_CFG.h
 *
 *  Created on: May 23, 2023
 *      Author: daled
 */

#ifndef SCI_RTT_SCI_RTT_CFG_H_
#define SCI_RTT_SCI_RTT_CFG_H_

#define SCI_RXBUFFER_SIZE (32)
#define SCI_ECHO_RX (1)
#if defined (BOARD_RA8D1_EK)
#define SCI_UART_B (1)
#else
#define SCI_UART_B (0)
#endif

#endif /* SCI_RTT_SCI_RTT_CFG_H_ */
