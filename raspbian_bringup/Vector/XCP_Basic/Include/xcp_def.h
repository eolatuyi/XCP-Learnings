/*----------------------------------------------------------------------------
| File:
|   XCP_DEF.H
|
| Project:
|   XCP samples
|
| Description
|   XCP default settings
|   Don't change this file
|
 ----------------------------------------------------------------------------*/
#if defined ( __XCP_DEF_H__ )
#else
#define __XCP_DEF_H__


/*------------------------------------------------------------------------------------*/
/* Default settings */

/* XCP transport layer */
#if !defined(XCP_TRANSPORT_LAYER_VERSION)
  #define XCP_TRANSPORT_LAYER_VERSION 0x0100
#endif
#if !defined(kXcpMaxCTO)
  #define kXcpMaxCTO 8      /* Maximum CTO Message Lenght */
#endif
#if !defined(kXcpMaxDTO)
  #define kXcpMaxDTO 8      /* Maximum DTO Message Lenght */
#endif

/* ROM memory qualifiers */
#if !defined(MEMORY_ROM)
  #define MEMORY_ROM const
#endif
#if !defined(V_MEMROM0)
  #define V_MEMROM0
#endif
#if !defined(MEMORY_CONST)
  #define MEMORY_CONST
#endif


/* Alignment requirements */
/* Specify, whether the microcontroller allows unaligned memory access or not */
#if !defined(XCP_ENABLE_UNALIGNED_MEM_ACCESS) && !defined(XCP_DISABLE_UNALIGNED_MEM_ACCESS)
  #define XCP_DISABLE_UNALIGNED_MEM_ACCESS
#endif

/* General settings */ 
#if !defined(XCP_ENABLE_PARAMETER_CHECK) && !defined(XCP_DISABLE_PARAMETER_CHECK)
  #define XCP_DISABLE_PARAMETER_CHECK
#endif
#if !defined(XCP_ENABLE_COMM_MODE_INFO) && !defined(XCP_DISABLE_COMM_MODE_INFO)
  #define XCP_ENABLE_COMM_MODE_INFO
#endif
#if !defined(XCP_ENABLE_USER_COMMAND) && !defined(XCP_DISABLE_USER_COMMAND)
  #define XCP_DISABLE_USER_COMMAND
#endif

/* Block transfer */ 
#if !defined(XCP_ENABLE_BLOCK_UPLOAD) && !defined(XCP_DISABLE_BLOCK_UPLOAD)
  #define XCP_DISABLE_BLOCK_UPLOAD
#endif
#if !defined(XCP_ENABLE_BLOCK_DOWNLOAD) && !defined(XCP_DISABLE_BLOCK_DOWNLOAD)
  #define XCP_DISABLE_BLOCK_DOWNLOAD
#endif

/* Enable transmission of event messages */
#if !defined(XCP_ENABLE_SEND_EVENT) && !defined(XCP_DISABLE_SEND_EVENT)
  #define XCP_DISABLE_SEND_EVENT
#endif

/* Service request message */ 
#if !defined(XCP_ENABLE_SERV_TEXT) && !defined(XCP_DISABLE_SERV_TEXT)
  #define XCP_DISABLE_SERV_TEXT
#endif
#if !defined(XCP_ENABLE_SERV_TEXT_PUTCHAR) && !defined(XCP_DISABLE_SERV_TEXT_PUTCHAR)
  #define XCP_DISABLE_SERV_TEXT_PUTCHAR
#endif
#if !defined(XCP_ENABLE_SERV_TEXT_PRINTF) && !defined(XCP_DISABLE_SERV_TEXT_PRINTF)
  #define XCP_DISABLE_SERV_TEXT_PRINTF
#endif

/* Disable/Enable Interrupts */
/* Has to be defined only if xcpSendCallBack may interrupt xcpEvent */
#if !defined(XcpInterruptDisable)                
  #define XcpInterruptDisable()                
#endif
#if !defined(XcpInterruptEnable)
  #define XcpInterruptEnable()                 
#endif

/* Custom initialization not needed */
#if !defined(ApplXcpInit)
  #define ApplXcpInit()
#endif

/* Custom background processing not needed */
#if !defined(ApplXcpBackground)
  #define ApplXcpBackground()
#endif

/* Flush of transmit queue not needed */
#if !defined(ApplXcpSendFlush)
  #define ApplXcpSendFlush()
#endif

/* XCP page switching */
#if !defined ( XCP_ENABLE_CALIBRATION_PAGE ) && !defined ( XCP_DISABLE_CALIBRATION_PAGE )
  #define XCP_DISABLE_CALIBRATION_PAGE
#endif

/* XCP protocol data acquisition parameters (DAQ) */
#if defined(XCP_DISABLE_DAQ)
    #define XCP_DISABLE_SEND_QUEUE
    #define XCP_ENABLE_SEND_DIRECT
#else
  #if !defined(XCP_ENABLE_DAQ) && !defined(XCP_DISABLE_DAQ)
    #define XCP_ENABLE_DAQ
  #endif
  #if !defined(XCP_ENABLE_SEND_DIRECT) && !defined(XCP_ENABLE_SEND_QUEUE)
    #define XCP_ENABLE_SEND_QUEUE
    #define XCP_DISABLE_SEND_DIRECT
  #endif  
  #if defined(XCP_ENABLE_SEND_QUEUE)
    #define XCP_DISABLE_SEND_DIRECT
  #endif  
  #if !defined(XCP_ENABLE_DAQ_HDR_ODT_DAQ) && !defined(XCP_DISABLE_DAQ_HDR_ODT_DAQ)
    #define XCP_DISABLE_DAQ_HDR_ODT_DAQ
  #endif
  #if !defined(kXcpDaqMemSize)
    #define kXcpDaqMemSize 256
  #endif
  #if !defined(kXcpStiOdtCount)
    #define kXcpStiOdtCount 1
  #endif
  #if !defined(XCP_ENABLE_DAQ_PROCESSOR_INFO) && !defined(XCP_DISABLE_DAQ_PROCESSOR_INFO)
    #define XCP_ENABLE_DAQ_PROCESSOR_INFO
  #endif
  #if !defined(XCP_ENABLE_DAQ_RESOLUTION_INFO) && !defined(XCP_DISABLE_DAQ_RESOLUTION_INFO)
    #define XCP_ENABLE_DAQ_RESOLUTION_INFO
  #endif
  #if !defined(XCP_ENABLE_DAQ_PRESCALER) && !defined(XCP_DISABLE_DAQ_PRESCALER)
    #define XCP_DISABLE_DAQ_PRESCALER
  #endif
  #if !defined(XCP_ENABLE_DAQ_OVERRUN_INDICATION) && !defined(XCP_DISABLE_DAQ_OVERRUN_INDICATION)
    #define XCP_ENABLE_DAQ_OVERRUN_INDICATION
  #endif
  #if !defined(XCP_ENABLE_DAQ_RESUME) && !defined(XCP_DISABLE_DAQ_RESUME)
    #define XCP_DISABLE_DAQ_RESUME
  #endif
  #if !defined(XCP_ENABLE_DAQ_TIMESTAMP) && !defined(XCP_DISABLE_DAQ_TIMESTAMP)
    #define XCP_DISABLE_DAQ_TIMESTAMP
  #endif
  #if !defined(XCP_ENABLE_DAQ_EVENT_INFO) && !defined(XCP_DISABLE_DAQ_EVENT_INFO)
    #define XCP_DISABLE_DAQ_EVENT_INFO
  #endif
#endif
#if !defined(XCP_ENABLE_STIM) && !defined(XCP_DISABLE_STIM)
  #define XCP_DISABLE_STIM
#endif



#endif
