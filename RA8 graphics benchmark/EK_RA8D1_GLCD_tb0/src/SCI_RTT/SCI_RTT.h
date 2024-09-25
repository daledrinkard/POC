/*
 * SCI_RTT.h
 *
 *  Created on: May 23, 2023
 *      Author: daled
 */
#ifndef SCI_RTT_SCI_RTT_H_
#define SCI_RTT_SCI_RTT_H_
#include "bsp_api.h"
#include "SCI_RTT_CFG.h"
fsp_err_t SCI_RTT_Open(void *p_sci, void *p_semaphore, void *p_user_callback);

#endif /* SCI_RTT_SCI_RTT_H_ */
