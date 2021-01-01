/*
 * xcp_application.c
 *
 *  Created on: Jul 25, 2020
 *      Author: Ebenezer O.
 */


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include <FreeRTOS.h>
#include <task.h>
#include "XcpBasic.h"
#include "xcp_driver.h"
#include "xcp_application.h"
#include "tester_utilities.h"


/***************************************************************************/
/* Version check                                                           */
/***************************************************************************/
#if ( CP_XCP_VERSION < 0x0125u )
  #error "XCP Protocol Layer and XCP template are inconsistent!"
#endif
#if ( CP_XCP_VERSION < 0x00 )
 #error "XCP Protocol Layer and XCP on CAN Transport Layer are not consistent!"
#endif


/***************************************************************************/
/* Defines                                                                 */
/***************************************************************************/
/* DAQ runtime supervision threshold value in timer ticks */
#define DAQ_RS_THRESHOLD 1234
/* macro which is used to return the current timer ticks from the hw timer */
#define HW_TIMER_TICKS 0

DEFINE_XCP_DRIVER;
DEFINE_XCP_DRIVER_UNIT_TEST(uint8, 4, uint16, 4,uint32, 4,uint64, 4)
DEFINE_TESTER_UTILITIES_SIMULATION(NULL, 0, NULL, 0, &XCP_DRIVER_CBLK,test_xcp_command_generator, NULL)
/*****************************************************************************/
/* Constants                                                                 */
/*****************************************************************************/


/*****************************************************************************/
/* Local data definitions                                                    */
/*****************************************************************************/

#if defined ( XCP_ENABLE_CALIBRATION_PAGE )
static vuint8 xcpCalPage; /* 0-Ram,1-Flash */
#endif

#if defined ( XCP_ENABLE_SEED_KEY )
vuint8 xcpResource;
#endif



//extern tXcpData xcp;

/* Variable used to store the intermdiate timer value */



/*****************************************************************************/
/*                                                                           */
/* Implementation                                                            */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************
| NAME:             test_xcp_command_handler
| CALLED BY:
| PRECONDITIONS:
| INPUT PARAMETERS:
| RETURN VALUES:    none
| DESCRIPTION:
******************************************************************************/
static void test_xcp_command_handler(struct xcp_driver_cblk* cblk, volatile xcp_driver_cto_t* pCmd)
{
    switch (CRO_CMD)
    {
        case CC_CONNECT:
            CRO_CONNECT_MODE       = 0x00;

            cblk->test_var.cmd_len = CRO_CONNECT_LEN;
            break;
        case CC_SET_MTA:
            CRO_SET_MTA_EXT        = 0x00;
            CRO_SET_MTA_ADDR       = (uint32_t)&cblk->test_var.read_param;

            cblk->test_var.cmd_len = CRO_SET_MTA_LEN;
            break;
        case CC_UPLOAD:
            CRO_UPLOAD_SIZE        = sizeof(cblk->test_var.read_param);

            cblk->test_var.cmd_len = CRO_UPLOAD_LEN;
            break;
        case CC_SHORT_UPLOAD:
            CRO_SHORT_UPLOAD_SIZE = sizeof(cblk->test_var.read_param);
            CRO_SHORT_UPLOAD_EXT  = 0x00;
            CRO_SHORT_UPLOAD_ADDR = (uint32_t)&cblk->test_var.read_param;

            cblk->test_var.cmd_len = CRO_SHORT_UPLOAD_LEN;
            break;
        case CC_FREE_DAQ:
            cblk->test_var.cmd_len = CRO_FREE_DAQ_LEN;
            break;
        case CC_ALLOC_DAQ:
            CRO_ALLOC_DAQ_COUNT    = 0x03; // requires 3 event channel

            cblk->test_var.cmd_len = CRO_ALLOC_DAQ_LEN;
            break;
        case CC_ALLOC_ODT:
            CRO_ALLOC_ODT_DAQ   = cblk->test_var.daq_list_idx;
            CRO_ALLOC_ODT_COUNT = cblk->test_var.daq_list_odt_count;

            cblk->test_var.cmd_len = CRO_ALLOC_ODT_LEN;
            break;
        case CC_ALLOC_ODT_ENTRY:
            CRO_ALLOC_ODT_ENTRY_DAQ   = cblk->test_var.daq_list_idx;
            CRO_ALLOC_ODT_ENTRY_ODT   = cblk->test_var.daq_list_odt_idx;
            CRO_ALLOC_ODT_ENTRY_COUNT = cblk->test_var.daq_list_odt_entry_count;

            cblk->test_var.cmd_len = CRO_ALLOC_ODT_ENTRY_LEN;
            break;
        case CC_GET_DAQ_LIST_MODE:
            CRO_GET_DAQ_LIST_MODE_DAQ = cblk->test_var.daq_list_idx;

            cblk->test_var.cmd_len = CRO_GET_DAQ_LIST_MODE_LEN;
            break;
        case CC_SET_DAQ_LIST_MODE:
            CRO_SET_DAQ_LIST_MODE_DAQ          = cblk->test_var.daq_list_idx;
            CRO_SET_DAQ_LIST_MODE_PRIORITY     = 0xFF;// Process daq as fast as possible
            CRO_SET_DAQ_LIST_MODE_PRESCALER    = 1;
            CRO_SET_DAQ_LIST_MODE_EVENTCHANNEL = cblk->test_var.daq_list_idx;
            CRO_SET_DAQ_LIST_MODE_MODE = (uint8_t)1 << 4;//DAQ, Timestamp on, PID ON, Alternating display off

            cblk->test_var.cmd_len = CRO_SET_DAQ_LIST_MODE_LEN;
            break;
        case CC_SET_DAQ_PTR:
            CRO_SET_DAQ_PTR_DAQ  = cblk->test_var.daq_list_idx;
            CRO_SET_DAQ_PTR_ODT  = cblk->test_var.daq_list_odt_idx;
            CRO_SET_DAQ_PTR_IDX  = cblk->test_var.daq_list_odt_entry_idx;

            cblk->test_var.cmd_len = CRO_SET_DAQ_PTR_LEN;
            break;
        case CC_WRITE_DAQ:
            CRO_WRITE_DAQ_BITOFFSET = 0xFF; // Ignore bit offset
            CRO_WRITE_DAQ_SIZE      = cblk->test_var.daq_list_odt_entry_size;
            CRO_WRITE_DAQ_EXT       = 0x00;
            CRO_WRITE_DAQ_ADDR      = TEST_XCP_VARIABLE_ADDR(cblk->test_var.obj_type, cblk->test_var.obj_idx);

            cblk->test_var.cmd_len = CRO_WRITE_DAQ_LEN;
            break;
        case CC_START_STOP_DAQ_LIST:
            CRO_START_STOP_DAQ  = cblk->test_var.daq_list_idx;
            CRO_START_STOP_MODE = cblk->test_var.daq_list_start_stop_select;

            cblk->test_var.cmd_len = CRO_START_STOP_LEN;
            break;
        case CC_START_STOP_SYNCH:
            CRO_START_STOP_SYNC_MODE = cblk->test_var.daq_list_allstop_sel_start_stop;

            cblk->test_var.cmd_len = CRO_START_STOP_SYNC_LEN;
            break;
        default:
            break;
    }
    // transmit
    memcpy(&cblk->transmit_buffer[0], (void*)pCmd, cblk->test_var.cmd_len);
    cblk->transmit_length = cblk->test_var.cmd_len;

#if defined(XCP_DRIVER_LOOPBACK_TEST_MODE)
     // Precondition: set tx length, buffer value
     cblk->test_trigger_tx = 1;
#endif
    return;
}
 /*****************************************************************************
| NAME:             test_xcp_daq
| CALLED BY:        
| PRECONDITIONS:    
| INPUT PARAMETERS: 
| RETURN VALUES:    none
| DESCRIPTION:      
******************************************************************************/
void test_xcp_daq(struct xcp_driver_cblk* cblk)
{
  volatile xcp_driver_cto_t* pCmd;
  pCmd = (xcp_driver_cto_t*)(&cblk->test_var.cmd.b[0]);
  
    //connect and delay
    CRO_CMD = CC_CONNECT;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // Free DAQ and delay
    CRO_CMD = CC_FREE_DAQ;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
        
    // Alloc 3 DAQ list and delay
    CRO_CMD = CC_ALLOC_DAQ;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // Alloc 2 ODT per daq list and delay
    CRO_CMD = CC_ALLOC_ODT;
    cblk->test_var.daq_list_idx = 0;
    cblk->test_var.daq_list_odt_count = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));

    cblk->test_var.daq_list_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    cblk->test_var.daq_list_idx = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
     
    // Alloc 3 ODT entry per daq  and delay
    CRO_CMD = CC_ALLOC_ODT_ENTRY;
    cblk->test_var.daq_list_idx = 0;
    cblk->test_var.daq_list_odt_idx = 0;
    cblk->test_var.daq_list_odt_entry_count = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));

    cblk->test_var.daq_list_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));

    cblk->test_var.daq_list_idx = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));

    cblk->test_var.daq_list_idx = 0;
    cblk->test_var.daq_list_odt_idx = 1;
    cblk->test_var.daq_list_odt_entry_count = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    cblk->test_var.daq_list_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    cblk->test_var.daq_list_idx = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
      
    // Set 1st daq and its first odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_idx = 0;
    cblk->test_var.daq_list_odt_idx = 0;
    cblk->test_var.daq_list_odt_entry_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 1st daq and its first odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 1;
    cblk->test_var.obj_type = UINT8_TYPE;
    cblk->test_var.obj_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Set 1st daq and its 2nd odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_odt_idx = 1;
    cblk->test_var.daq_list_odt_entry_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 1st daq and its 2nd odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 4;
    cblk->test_var.obj_type = UINT32_TYPE;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Set 1st daq and its 3rd odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_odt_idx = 1;
    cblk->test_var.daq_list_odt_entry_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 1st daq and its 3rd odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 2;
    cblk->test_var.obj_type = UINT16_TYPE;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    // ===========================
    // Set 2nd daq and its first odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_idx = 1;
    cblk->test_var.daq_list_odt_idx = 0;
    cblk->test_var.daq_list_odt_entry_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 2nd daq and its first odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 1;
    cblk->test_var.obj_type = UINT8_TYPE;
    cblk->test_var.obj_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Set 2nd daq and its 2nd odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_odt_idx = 1;
    cblk->test_var.daq_list_odt_entry_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 2nd daq and its 2nd odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 4;
    cblk->test_var.obj_type = UINT32_TYPE;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Set 2nd daq and its 3rd odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_odt_idx = 1;
    cblk->test_var.daq_list_odt_entry_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 2nd daq and its 3rd odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 2;
    cblk->test_var.obj_type = UINT16_TYPE;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // ===========================
    // Set 3rd daq and its first odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_idx = 2;
    cblk->test_var.daq_list_odt_idx = 0;
    cblk->test_var.daq_list_odt_entry_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 3rd daq and its first odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 1;
    cblk->test_var.obj_type = UINT8_TYPE;
    cblk->test_var.obj_idx = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Set 3rd daq and its 2nd odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_odt_idx = 1;
    cblk->test_var.daq_list_odt_entry_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 3rd daq and its 2nd odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 4;
    cblk->test_var.obj_type = UINT32_TYPE;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Set 3rd daq and its 3rd odt entry pointer and delay
    CRO_CMD = CC_SET_DAQ_PTR;
    cblk->test_var.daq_list_odt_idx = 1;
    cblk->test_var.daq_list_odt_entry_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // Write 3rd daq and its 3rd odt entry pointer and delay
    CRO_CMD = CC_WRITE_DAQ;
    cblk->test_var.daq_list_odt_entry_size = 2;
    cblk->test_var.obj_type = UINT16_TYPE;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));

    // ============================================================================
    // Set daq list mode and delay
    CRO_CMD = CC_SET_DAQ_LIST_MODE;
    cblk->test_var.daq_list_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));  

    CRO_CMD = CC_SET_DAQ_LIST_MODE;
    cblk->test_var.daq_list_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000)); 
    
    CRO_CMD = CC_SET_DAQ_LIST_MODE;
    cblk->test_var.daq_list_idx = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));   
    
    // Get daq list mode and delay
    CRO_CMD = CC_GET_DAQ_LIST_MODE;
    cblk->test_var.daq_list_idx = 0;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000)); 

    CRO_CMD = CC_GET_DAQ_LIST_MODE;
    cblk->test_var.daq_list_idx = 1;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000)); 
    
    CRO_CMD = CC_GET_DAQ_LIST_MODE;
    cblk->test_var.daq_list_idx = 2;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000)); 

    // select 1st daq list and delay
    CRO_CMD = CC_START_STOP_DAQ_LIST;
    cblk->test_var.daq_list_idx = 0;
    cblk->test_var.daq_list_start_stop_select = 0x02;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // select 2nd daq list and delay
    CRO_CMD = CC_START_STOP_DAQ_LIST;
    cblk->test_var.daq_list_idx = 1;
    cblk->test_var.daq_list_start_stop_select = 0x02;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // select 3rd daq list and delay
    CRO_CMD = CC_START_STOP_DAQ_LIST;
    cblk->test_var.daq_list_idx = 2;
    cblk->test_var.daq_list_start_stop_select = 0x02;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000));
    // start daq list and delay    
    CRO_CMD = CC_START_STOP_SYNCH;
    cblk->test_var.daq_list_allstop_sel_start_stop = 0x01;
    test_xcp_command_handler(cblk, pCmd);
    vTaskDelay(pdMS_TO_TICKS(5000)); 
    
    return;
}

 /*****************************************************************************
| NAME:             test_xcp_command
| CALLED BY:        Test Utilities
| PRECONDITIONS:    Transmit buffer is available in cblk
| INPUT PARAMETERS: pCmd : generated XCP message to be received as CTO.
| RETURN VALUES:    none
| DESCRIPTION:      
******************************************************************************/
void test_xcp_command_generator(void* arg)
{
  xcp_driver_cto_t* pCmd;
  struct xcp_driver_cblk* cblk = (struct xcp_driver_cblk*) arg;

  pCmd = (xcp_driver_cto_t*)(&cblk->test_var.cmd.b[0]);

  if (cblk->test_var.execute_daq_test)
  {
      test_xcp_daq(cblk);
      cblk->test_var.execute_daq_test = 0;
  }
  else /*catch all other test*/
  {
     /* XCP Command Handler */
      test_xcp_command_handler(cblk, pCmd);
   }     
  return;
 }
 
void xcp_app_send_flush(void)
{
    // TODO:
    return;
}



#if defined ( XCP_ENABLE_SEED_KEY ) || defined ( XCP_ENABLE_CALIBRATION_PAGE )

void ApplXcpInitTemplate( void )
{
  #if defined ( XCP_ENABLE_SEED_KEY )
  /* Part of the example implementation for Seed & Key. */
  xcpResource = 0;
  #endif


  #if defined ( XCP_ENABLE_CALIBRATION_PAGE )
  /* Part of the example implementation for page switching */
  xcpCalPage = 0;                         /* 0-Ram,1-Flash */
  #endif
}

#endif



#if defined (XCP_ENABLE_DAQ)



  #if defined ( XCP_ENABLE_DAQ_TIMESTAMP )
/****************************************************************************/
/* DAQ Timestamp                                                            */
/****************************************************************************/
 XcpDaqTimestampType xcp_app_timer(void)
{
    uint32_t timestamp;
    
    timestamp = (uint32_t)xTaskGetTickCount();
    
    if (timestamp > UINT16_MAX)
    {
        timestamp = timestamp >> 16;
    }
    return timestamp;
}
  #endif /* XCP_ENABLE_DAQ_TIMESTAMP */

#endif /* XCP_ENABLE_DAQ */

 /****************************************************************************/
 /* Init                                                           */
 /****************************************************************************/
  void xcp_app_init(void)
 {
     xcp_driver_init(&XCP_DRIVER_CBLK);
     XcpInit();
     return;
 }

#if defined ( XCP_ENABLE_CALIBRATION_PAGE )

/*****************************************************************************/
/* Page switching                                                            */
/* This is just an example how the page switching could be implemented.      */
/*****************************************************************************/

/*****************************************************************************
| NAME:             ApplXcpGetCalPage
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment :
|                   mode    :
| RETURN VALUES:
| DESCRIPTION:
******************************************************************************/
vuint8 ApplXcpGetCalPage( vuint8 segment, vuint8 mode )
{
  return (vuint8)xcpCalPage;
}

/*****************************************************************************
| NAME:             ApplXcpSetCalPage
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment :
|                   page    :
|                   mode    : CAL_ECU and/or CAL_XCP or CAL_ALL
| RETURN VALUES:
| DESCRIPTION:
******************************************************************************/
vuint8 ApplXcpSetCalPage( vuint8 segment, vuint8 page, vuint8 mode )
{
  if (segment!=0)
  {
    return CRC_OUT_OF_RANGE; /* Only one segment supported */
  }
  if (page>1)
  {
    return CRC_PAGE_NOT_VALID;
  }
  if ((mode&0x03)!=0x03)
  {
    return CRC_PAGE_MODE_NOT_VALID; /* Only one segment supported */
  }
  xcpCalPage = page;

  return (vuint8)0;
}


  #if defined ( XCP_ENABLE_PAGE_COPY )
/*****************************************************************************
| NAME:             ApplXcpCopyCalPage
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: srcSeg   :
|                   srcPage  :
|                   destSeg  :
|                   destPage :
| RETURN VALUES:
| DESCRIPTION:
******************************************************************************/
vuint8 ApplXcpCopyCalPage(vuint8 srcSeg, vuint8 srcPage, vuint8 destSeg, vuint8 destPage)
{

  /* something has to be done here */

  return (vuint8)1u;
}
  #endif

#endif /* XCP_ENABLE_CALIBRATION_PAGE */


#if defined ( XCP_ENABLE_PAGE_FREEZE )
/*****************************************************************************
| NAME:             ApplXcpSetFreezeMode
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment :
|                   mode    :
| RETURN VALUES:
| DESCRIPTION:
******************************************************************************/
void ApplXcpSetFreezeMode( vuint8 segment, vuint8 mode )
{
  /* something has to be done here */
}

/*****************************************************************************
| NAME:             ApplXcpGetFreezeMode
| CALLED BY:        XcpCommand
| PRECONDITIONS:    RAM / ROM switching enabled
| INPUT PARAMETERS: segment :
| RETURN VALUES: mode
| DESCRIPTION:
******************************************************************************/
vuint8 ApplXcpGetFreezeMode( vuint8 segment )
{
  /* something has to be done here */

  return (vuint8)0u;
}
#endif /* XCP_ENABLE_PAGE_FREEZE */






/*****************************************************************************/
/* Platform and implementation specific functions for XCP                    */
/*****************************************************************************/

/*****************************************************************************
| NAME:             ApplXcpGetPointer
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr_ext : 8  bit address extension
|                   addr     : 32 bit address
| RETURN VALUES:    Pointer to the address specified by the parameters.
| DESCRIPTION:      Convert a memory address from XCP format 8/32bit into a C pointer.
|                   Used for memory transfers like DOWNLOAD, UPLOAD (MTA)
******************************************************************************/
#if defined ( ApplXcpGetPointer )
 /* defined as macro */
#else
MTABYTEPTR ApplXcpGetPointer( vuint8 addr_ext, vuint32 addr )
{

  #if defined ( V_ENABLE_USE_DUMMY_STATEMENT )
  /* avoid compiler warning due to unused parameters */
    addr_ext = 0;
    return (MTABYTEPTR) ( addr + addr_ext );
  #endif

  /* Example C16x: DDP1 used for XCP_RAM/XCP_ROM selection */
  #if defined ( CANBOX ) || defined ( PHYTEC_MM167 )
    #if defined ( XCP_ENABLE_CALIBRATION_PAGE )
  if (xcpCalPage==1 && addr>=0x14000 && addr<0x18000)  /* CALRAM */
  {
    return (MTABYTEPTR) ( addr + 0x30000UL );
  }
    #endif
  #endif

}

#endif /* ApplXcpGetPointer */


#if defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL )

/*****************************************************************************
| NAME:             ApplXcpRead
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : 32 bit address
| RETURN VALUES:    Value specified by the address parameters.
| DESCRIPTION:      Read a single byte from the memory.
******************************************************************************/
vuint8 ApplXcpRead( vuint32 addr )
{
  #if defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
  /* X: Higher Word Address */
  /* D: Lower Word Address */

  /* Return: B */
    #asm
      tfr   ccrw,y
      sei
      exg   d,x
      ldaa  $10
      stab  $10
      gldab 0,x
      staa  $10
      tfr   y,ccrw
    #endasm
  #else
    #if defined ( C_COMP_MTRWRKS_MCS12X_MSCAN12 )
  return *((vuint8* __far)addr);
    #else
  return *((vuint8*)addr);
    #endif
  #endif
}

/*****************************************************************************
| NAME:             ApplXcpWrite
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : 32 bit address
|                   data : data to be written to memory
| RETURN VALUES:    none
| DESCRIPTION:      Write a single byte to RAM.
******************************************************************************/
void XCP_FAR ApplXcpWrite( vuint32 addr, vuint8 data )
{
  #if defined ( C_COMP_COSMIC_MCS12X_MSCAN12 )
  /* X: Higher Word Address */
  /* D: Lower Word Address */
  /* Stack: Data */
    #asm
      tfr   ccrw,y
      sei
      exg   d,x
      ldaa  $10
      stab  $10
    #if defined ( __CSMC__ ) && ( (__CSMC__ & 0x01) == 0x01 )
      ldab  3,s
    #else
      ldab  4,s
    #endif
      gstab 0,x
      staa  $10
      tfr   y,ccrw
    #endasm
  #else
    #if defined ( C_COMP_MTRWRKS_MCS12X_MSCAN12 )
  *((vuint8* __far)addr) = data;
    #else
  *((vuint8*)addr) = data;
    #endif
  #endif
}
#endif

#if defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) || defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL )
  #if defined ( XCP_ENABLE_CHECKSUM )
     #if ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD22 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD24 ) || \
         ( kXcpChecksumMethod == XCP_CHECKSUM_TYPE_ADD44 ) || \
         defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL )
/*****************************************************************************
| NAME:             ApplXcpReadChecksumValue
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : 32 bit address
| RETURN VALUES:    Pointer to the address specified by the parameters.
| DESCRIPTION:      Read and return memory with size of tXcpChecksumAddType.
******************************************************************************/
tXcpChecksumAddType ApplXcpReadChecksumValue( vuint32 addr )
{
  return *((tXcpChecksumAddType*)addr);
}
    #endif
  #endif
#endif

#if defined ( XCP_ENABLE_CHECKSUM ) && defined ( XCP_ENABLE_CUSTOM_CRC )
/*****************************************************************************
| NAME:             ApplXcpCalculateChecksum
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: pMemArea : Pointer to memory area for checksum calculation
|                   pRes : Pointer to resulting XCP response string
|                   length : Length of memory area
| RETURN VALUES:    XCP_CMD_OK: CRC calculation performed, value stored in pRes
|                   XCP_CMD_PENDING: Pending response, triggered by call of Xcp_SendCrm
|                   XCP_CMD_DENIED: CRC calculation not possible
| DESCRIPTION:
******************************************************************************/
vuint8 ApplXcpCalculateChecksum( ROMBYTEPTR pMemArea, BYTEPTR pRes, vuint32 length )
{
  /* Calculate CRC synchronously and return XCP_CMD_OK, keep runtime and possible timeouts in mind! */
  /* or */
  /* Trigger calculation of CRC asynchronously and trigger response by call of Xcp_SendCrm after calculation */
  vuint32 crc;
  /* crc = Crc_CalculateCRC32(pMemArea, length, 0x00); */
  pRes[0] = 0xFF; /* Positive Response */
  pRes[1] = XCP_CHECKSUM_TYPE_CRC32;
  pRes[2] = 0x00; /* Reserved */
  pRes[3] = 0x00; /* Reserved */
  /* Consider endianess, this is little endian architecture */
  pRes[4] = (vuint8)crc;
  pRes[5] = (vuint8)(crc >> 8);
  pRes[6] = (vuint8)(crc >> 16);
  pRes[7] = (vuint8)(crc >> 24);
  return(XCP_CMD_DENIED);
  /* return(XCP_CMD_OK); */
}
#endif


#if defined ( XCP_ENABLE_CALIBRATION_MEM_ACCESS_BY_APPL ) && !defined ( XCP_ENABLE_MEM_ACCESS_BY_APPL )
/*****************************************************************************
| NAME:             ApplXcpCalibrationWrite
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : destination address
|                   size : length of data to write
|                   data : data to write
| RETURN VALUES:    XCP_CMD_OK or XCP_CMD_ERROR
| DESCRIPTION:      Write memory with size
******************************************************************************/
vuint8 ApplXcpCalibrationWrite(MTABYTEPTR addr, vuint8 size, const BYTEPTR data)
{
  while(size > (vuint8)0)
  {
    *addr = *data;
    addr++;
    data++;
    size--;
  }
  return (vuint8)XCP_CMD_OK;
}
/*****************************************************************************
| NAME:             ApplXcpCalibrationRead
| CALLED BY:        XCP Protocol Layer
| PRECONDITIONS:    none
| INPUT PARAMETERS: addr : source address
|                   size : length of data to read
|                   data : read data
| RETURN VALUES:    XCP_CMD_OK or XCP_CMD_ERROR
| DESCRIPTION:      Read and return memory with size
******************************************************************************/
vuint8 ApplXcpCalibrationRead(MTABYTEPTR addr, vuint8 size, BYTEPTR data)
{
  while(size > (vuint8)0)
  {
    *data = *addr;
    addr++;
    data++;
    size--;
  }
  return (vuint8)XCP_CMD_OK;
}
#endif

/*****************************************************************************/
/* Seed & Key                                                                */
/* This is just an example how seed & key could be implemented.              */
/*****************************************************************************/
#if defined ( XCP_ENABLE_SEED_KEY )

/*****************************************************************************
| NAME:             ApplXcpGetSeed
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: resource : resource
|                              (either RM_CAL_PAG or RM_DAQ or RM_STIM or RM_PGM)
|                   seed     : pointer to the generated seed.
| RETURN VALUES:    the length of the seed.
| DESCRIPTION:      Generates a seed.
|                   Attention: The seed has a maximum length of MAX_CTO-2 bytes.
******************************************************************************/
vuint8 ApplXcpGetSeed( const vuint8 resource, vuint8 *seed )
{
  /* Store resource mask */
  xcpResource = resource;

  /* Generate a seed */
  seed[0] = 0;
  seed[1] = 1;
  seed[2] = 2;
  seed[3] = 3;
  seed[4] = 4;
  seed[5] = 5;

  return (vuint8)6u;
}

/*****************************************************************************
| NAME:             ApplXcpUnlock
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: key    : pointer to the key
|                   length : length of the key
| RETURN VALUES:    0 : if the key is not valid
|                   RM_CAL_PAG, RM_DAQ, RM_STIM, RM_PGM : for valid keys
| DESCRIPTION:      Check key and return the resource that has to be unlocked.
******************************************************************************/
vuint8 ApplXcpUnlock( const vuint8 *key, const vuint8 length )
{
  /*
    Ckeck the key
    key[0],key[1],key[2],key[3],key[4],key[5]
  */

  return xcpResource;
}

#endif /* XCP_ENABLE_SEED_KEY */


/****************************************************************************/
/* GET_ID service                                                           */
/****************************************************************************/
#if defined ( XCP_ENABLE_GET_ID_GENERIC )
/*****************************************************************************
| NAME:             ApplXcpGetIdData
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: pData : pointer to pointer of the MAP file names
|                   id: Identifier of requested information
| RETURN VALUES:    length of the MAP file names
| DESCRIPTION:      Returns a pointer to a pointer of MAP file names
******************************************************************************/
vuint32 ApplXcpGetIdData( MTABYTEPTR *pData, vuint8 id )
{
  switch( id )
  { /* Example code, fit to your needs */
    case IDT_ASCII: *pData = (MTABYTEPTR)&asciiString; /* Handle ASCII text */
                    return (vuint32)sizeof(asciiString)-1;
                    break;
    case IDT_ASAM_NAME: *pData = (MTABYTEPTR)&asamName; /* Handle ASAM-MC2 filename without path and extension */
                        return (vuint32)sizeof(asamName)-1;
                        break;
    case IDT_ASAM_PATH: *pData = (MTABYTEPTR)&asamPath; /* Handle ASAM-MC2 filename with path and extension */
                        return (vuint32)sizeof(asamPath)-1;
                        break;
    case IDT_VECTOR_MAPNAMES: *pData = (MTABYTEPTR)&mapName; /* Handle map file name */
                              return (vuint32)sizeof(mapName)-1;
                              break;
    default:
      return (vuint32)0u; /* Id not available */
  }
}
#endif /* XCP_ENABLE_GET_ID_GENERIC */


/****************************************************************************/
/* User defined service                                                     */
/****************************************************************************/
#if defined ( XCP_ENABLE_USER_COMMAND )
/*****************************************************************************
| NAME:             ApplXcpUserService
| CALLED BY:        XcpCommand
| PRECONDITIONS:    none
| INPUT PARAMETERS: pCmd : pointer to command string
| RETURN VALUES:    XCP_CMD_OK      : command ok
|                   XCP_CMD_SYNTAX  : command not accepted
|                   XCP_CMD_PENDING : command in progress, call $$Xcp_SendCrm when done
| DESCRIPTION:      Implemented user command
******************************************************************************/
vuint8 ApplXcpUserService( const BYTEPTR pCmd )
{

  /* something has to be done here */

  return (vuint8)XCP_CMD_OK;
}

#endif


/****************************************************************************/
/* Open Command Interface                                                   */
/****************************************************************************/



#if defined ( XCP_ENABLE_TESTMODE )
/****************************************************************************/
/* Test                                                                     */
/****************************************************************************/

  #if defined ( ApplXcpPrint )
 /* ApplXcpPrint is a macro */
  #else

void ApplXcpPrint( const vsint8 *str, ... )
{
  /* something has to be done here */
}

  #endif

#endif /* XCP_ENABLE_TESTMODE */


/****************************************************************************/
/* ApplXcpSendStall                                                             */
/****************************************************************************/

#if defined ( XCP_ENABLE_SEND_EVENT ) || defined ( XCP_ENABLE_SERV_TEXT_PUTCHAR )
  #if defined ( ApplXcpSendStall )
 /* ApplXcpSendStall is already defined or a macro */
  #else
vuint8 ApplXcpSendStall( void )
{
  /* something has to be done here */

  return( (vuint8) 1u );
}
  #endif
#endif

/****************************************************************************/
/* ApplXcpSend                                                            */
/****************************************************************************/
void ApplXcpSend(uint8_t len, const uint8_t* msg )
{
    xcp_driver_cblk.transmit_length = len;
    memcpy(XCP_DRIVER_CBLK.transmit_buffer,msg,len);
    xcp_driver_transmit(&xcp_driver_cblk);
}

/****************************************************************************/
/* Flush the transmit buffer if there is one implemented in ApplXcpSend.
 * The function can be overwritten by the macro ApplXcpSendStall()          */
/****************************************************************************/
//void ApplXcpSendFlush(void)
//{
//    // TODO: Flush transmit buffer as needed
//    return;
//}

