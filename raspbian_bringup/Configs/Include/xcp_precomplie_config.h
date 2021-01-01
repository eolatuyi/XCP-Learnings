/*
 * xcp_precomplie_config.h
 *
 *  Created on     : Jul 25, 2020
 *      Author     : Ebenezer O.
 *      Description: Compiler switches for XCP Basic optional features
 */

#ifndef CONFIGS_INCLUDES_XCP_PRECOMPLIE_CONFIG_H_
#define CONFIGS_INCLUDES_XCP_PRECOMPLIE_CONFIG_H_

// General includes
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//#include <windows.h>
#include <FreeRTOS.h>
#include <stdint.h>

#include "xcp_application.h"

/*----------------------------------------------------------------------------*/
/* Test instrumentation */

/* Turn on screen logging and assertions */

#define XCP_ENABLE_TESTMODE
#ifdef XCP_ENABLE_TESTMODE
  #define ApplXcpPrint printf
  #define XCP_ASSERT(x) if (!(x)) ApplXcpPrint("Assertion failed\n");
  #define XCP_PARAMETER_CHECKS
#endif

//#define XCP_ENABLE_DPRAM

// Activate autom. map name detection
#define XCP_ENABLE_VECTOR_MAPNAMES

/*----------------------------------------------------------------------------*/
/* XCP Transport Layer */


/*----------------------------------------------------------------------------*/
/* Platform specific definitions */

  /* 8-Bit qualifier */
typedef uint8_t  vuint8;
typedef int8_t   vsint8;

/* 16-Bit qualifier */
typedef uint16_t  vuint16;
typedef int16_t   vsint16;

/* 32-Bit qualifier */
typedef uint32_t   vuint32;
typedef int32_t    vsint32;

/* Byte order */
//#define C_CPUTYPE_BIGENDIAN  /* Motorola */
#define C_CPUTYPE_LITTLEENDIAN /* Intel */



/*----------------------------------------------------------------------------*/
/* XCP protocol parameters */


/* XCP message length */
#define kXcpMaxCTO     8      /* Maximum CTO Message Lenght */
#define kXcpMaxDTO     8      /* Maximum DTO Message Lenght */


/*----------------------------------------------------------------------------*/
/* XCP protocol data acquisition parameters */

/* Enable DAQ and set the memory size reserved for DAQ lists */
#define XCP_ENABLE_DAQ
//#define XCP_ENABLE_DAQ_EVENT_INFO
#if defined(kXcpDaqMemSize)
    #undef  kXcpDaqMemSize
    #define kXcpDaqMemSize 1800
#endif
#define kXcpSendQueueMinSize 0x7F

//#define kXcpMaxEvent   3
//#define XCP_ENABLE_DAQ_PRESCALER
#define XCP_DISABLE_DAQ_OVERRUN_INDICATION

#define V_ENABLE_USE_DUMMY_STATEMENT /* use ApplXcpGetPointer as is */

/* Flush the transmit buffer after a command response */
//#define ApplXcpSendFlush xcp_app_send_flush

/* Enable DAQ Timestamps */
//typedef vuint16 XcpDaqTimestampType;
#define XCP_ENABLE_DAQ_TIMESTAMP
#define kXcpDaqTimestampSize                 2
#define kXcpDaqTimestampUnit                 DAQ_TIMESTAMP_UNIT_10MS
#define kXcpDaqTimestampTicksPerUnit         10 // 10 ticks per 10 MS due to Free RTOS tick config.
#define ApplXcpGetTimestamp()                (XcpDaqTimestampType)xcp_app_timer()
#define ApplXcpDaqGetTimestamp()             (XcpDaqTimestampType)xcp_app_timer()

#define ApplXcpInterruptEnable()             vPortExitCritical()
#define ApplXcpInterruptDisable()            vPortEnterCritical()


#define XCP_DISABLE_DAQ_HDR_ODT_DAQ
#undef XCP_ENABLE_DPRAM
#define XCP_DISABLE_SERV_TEXT_PRINT

#endif /* CONFIGS_INCLUDES_XCP_PRECOMPLIE_CONFIG_H_ */
