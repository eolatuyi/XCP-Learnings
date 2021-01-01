/*
 * xcp_application.h
 *
 *  Created on: Jul 25, 2020
 *      Author: gertrude
 */

#ifndef CONFIGS_INCLUDES_XCP_APPLICATION_H_
#define CONFIGS_INCLUDES_XCP_APPLICATION_H_

#include <stdint.h>
#include "xcp_driver.h"

void xcp_app_init(void);
void xcp_app_send_flush(void);
uint16_t xcp_app_timer(void);
void xcp_app_server_thread(void *arg);

void test_xcp_command_generator(void* arg);

#endif /* CONFIGS_INCLUDES_XCP_APPLICATION_H_ */
