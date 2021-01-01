/*
 * xcp_driver.h
 *
 *  Created on: Jul 25, 2020
 *      Author: gertrude
 */

#ifndef CONFIGS_INCLUDES_XCP_DRIVER_H_
#define CONFIGS_INCLUDES_XCP_DRIVER_H_

#include <stdbool.h>
#include <stdint.h>

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "xcp_precomplie_config.h"


#define XCP_DRIVER_LOOPBACK_TEST_MODE 1

#define XCP_DRIVER_UNIT_TEST 1

#define XCP_DRIVER_TX_BUFFER_LEN 12 // For TCP, must be 4 greater than MAX XCP MSG Length
#define XCP_DRIVER_RX_BUFFER_LEN 12 // For TCP, must be 4 greater than MAX XCP MSG Length

#if defined(XCP_DRIVER_LOOPBACK_TEST_MODE)
#define XCP_DRIVER_CAN_FRAME_LEN 4 // rx id: 0x1001, tx id: 0x2001, tester tx id: 0x1001, tester rx id: 0x2001
#else
#define XCP_DRIVER_CAN_FRAME_LEN 2 // rx id: 0x1001, tx id: 0x2001
#endif 

#define DECLARE_XCP_DRIVER extern struct xcp_driver_cblk xcp_driver_cblk
#define DEFINE_XCP_DRIVER struct xcp_driver_cblk xcp_driver_cblk
#define XCP_DRIVER_CBLK xcp_driver_cblk

typedef enum 
{
    UINT8_TYPE = 0,
    UINT16_TYPE,
    UINT32_TYPE,
    UINT64_TYPE,
} test_obj_t;
// Note: *_param_count should be >= 4
#define DEFINE_XCP_DRIVER_UNIT_TEST(uint8_param_id,  uint8_param_count, uint16_param_id, uint16_param_count, uint32_param_id, uint32_param_count, uint64_param_id, uint64_param_count)\
    uint8_t  test_xcp_##uint8_param_id[uint8_param_count]   = {0x99, 0x88, 0x77};                     \
    uint16_t test_xcp_##uint16_param_id[uint16_param_count] = {0xcccc, 0xbbbb, 0xaaaa};               \
    uint32_t test_xcp_##uint32_param_id[uint32_param_count] = {0xffffffff, 0xeeeeeeee, 0xdddddddd};  \
    uint64_t test_xcp_##uint64_param_id[uint64_param_count] = {0xdeadbeefdeadbeef};                   \
                                                                               \
    static uint32_t  test_xcp_variable_addr(test_obj_t type, uint16_t obj_idx) \
    {                                                                          \
        switch (type)                                                          \
        {                                                                      \
            case UINT8_TYPE:                                                   \
                return (uint32_t)&test_xcp_##uint8_param_id[obj_idx];          \
            case UINT16_TYPE:                                                  \
                return (uint32_t)&test_xcp_##uint16_param_id[obj_idx];         \
            case UINT32_TYPE:                                                  \
                return (uint32_t)&test_xcp_##uint32_param_id[obj_idx];         \
            case UINT64_TYPE:                                                  \
                return (uint32_t)&test_xcp_##uint64_param_id[obj_idx];         \
            default:                                                           \
                break;                                                         \
        }                                                                      \
        return 0;                                                              \
    }

#define TEST_XCP_VARIABLE_ADDR test_xcp_variable_addr

typedef union {
  /* There might be a loss of up to 3 bytes. */
  uint8_t  b[ ((kXcpMaxCTO + 3) & 0xFFC)      ];
  uint16_t w[ ((kXcpMaxCTO + 3) & 0xFFC) / 2  ];
  uint32_t dw[ ((kXcpMaxCTO + 3) & 0xFFC) / 4 ];
} xcp_driver_cto_t; // from XcpBasic.h - had to redefine here to avoid circular dependency - TODO:

struct xcp_driver_cblk
{
    uint8_t transmit_buffer[XCP_DRIVER_TX_BUFFER_LEN];
    union
    {
        uint32_t   command;
        uint8_t    buffer[XCP_DRIVER_RX_BUFFER_LEN];
    }receive_buffer;

    /* variables */
    uint8_t transmit_length;
    volatile uint8_t rx_flag;
#if defined(XCP_DRIVER_LOOPBACK_TEST_MODE)
    volatile uint8_t test_rx_flag;
    volatile uint8_t test_trigger_tx; // pre-condition- set transmit_length and buffer value
#endif
    volatile uint8_t tx_flag;
    volatile uint8_t error_flag;
    volatile uint32_t rx_count;
    volatile uint32_t tx_count;
    struct 
    {
        // set test payload
        volatile xcp_driver_cto_t  cmd;
        uint8_t                    cmd_len;
        
        // set before executing read sequence
        volatile uint32_t read_param;   
        
        // DAQ list related
        volatile uint16_t daq_list_idx;            // zero based - also used for get daq list mode, also for event channel #
        volatile uint8_t  daq_list_odt_count; 
        volatile uint8_t  daq_list_odt_idx;        // zero based
        volatile uint8_t  daq_list_odt_entry_count;
        volatile uint8_t  daq_list_odt_entry_idx;  // zero based
        volatile uint8_t  daq_list_odt_entry_size; // in bytes
        
        volatile uint8_t  execute_daq_test;
        
        // test objects identifiers to log for DAQ
        volatile test_obj_t obj_type;
        volatile uint16_t   obj_idx;
        
        volatile uint8_t  daq_list_start_stop_select;      // 0-start, 1-stop, 2-select
        volatile uint8_t  daq_list_allstop_sel_start_stop; // 0- stop all, selected start, 2-selected stop
    }test_var;

    volatile Socket_t connected_sock;

};

DECLARE_XCP_DRIVER;

void xcp_driver_init(struct xcp_driver_cblk* const cblk);
void xcp_driver_transmit(struct xcp_driver_cblk* const xcp_driver_cblk);
void xcp_driver_transmit_complete(struct xcp_driver_cblk* const cblk);

#endif /* CONFIGS_INCLUDES_XCP_DRIVER_H_ */
