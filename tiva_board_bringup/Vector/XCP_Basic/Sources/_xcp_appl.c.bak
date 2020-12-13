

/*****************************************************************************
| Project Name:   XCP application callback function template
|    File Name:   _xcp_appl.c
|
|  Description: 
|    Template for the XCP application callback functions.
|    This is just an example and may be modified .
|
|  Contains:
|    - Examples for FLASH programming, EEPROM programming, CalPage switching
|
|-----------------------------------------------------------------------------
|               D E M O
|-----------------------------------------------------------------------------
|
|       Please note, that the demo and example programs 
|       only show special aspects of the software. 
|       With regard to the fact that these programs are meant 
|       for demonstration purposes only,
|       Vector Informatik's liability shall be expressly excluded in cases 
|       of ordinary negligence, to the extent admissible by law or statute.
|
|-----------------------------------------------------------------------------
|               C O P Y R I G H T
|-----------------------------------------------------------------------------
| Copyright (c) 2016 by Vector Informatik GmbH.           All rights reserved.
|
|       This software is copyright protected and 
|       proporietary to Vector Informatik GmbH.
|       Vector Informatik GmbH grants to you only
|       those rights as set out in the license conditions.
|       All other rights remain with Vector Informatik GmbH.
| 
|       Diese Software ist urheberrechtlich geschuetzt. 
|       Vector Informatik GmbH raeumt Ihnen an dieser Software nur 
|       die in den Lizenzbedingungen ausdruecklich genannten Rechte ein.
|       Alle anderen Rechte verbleiben bei Vector Informatik GmbH.
|
|-----------------------------------------------------------------------------
|               A U T H O R   I D E N T I T Y
|-----------------------------------------------------------------------------
| Initials     Name                      Company
| --------     ---------------------     -------------------------------------
| Tri          Frank Triem               Vector Informatik GmbH
| Hr           Andreas Herkommer         Vector Informatik GmbH
| Svh          Sven Hesselmann           Vector Informatik GmbH
|-----------------------------------------------------------------------------
|               R E V I S I O N   H I S T O R Y
|-----------------------------------------------------------------------------
|  Date       Version  Author  Description
| ----------  -------  ------  -----------------------------------------------
| 2004-12-30  1.00.00   Tri    ESCAN00010708: Create template _xcp_appl.c
| 2005-02-21  1.01.00   Tri    Rename XcpApplInit to ApplXcpInitTemplate
| 2005-05-19  1.02.00   Tri    Support data paging on Star12X
| 2005-12-14  1.03.00   Tri    ESCAN00013899: Data acquisition not possible during cold start
| 2006-03-17  1.04.00   Tri    Compiler switch for paged memory model added
| 2006-05-30  1.05.00   Tri    Demo disclaimer added
| 2007-01-18  1.06.00   Tri    ESCAN00018808: Support data paging on Star12X / Cosmic
| 2007-07-09  1.06.01   Hr     Support AUTOSAR Memory Mapping
| 2008-04-03  1.07.00   Hr     ESCAN00025754: Missing function stubs
| 2008-07-21  1.08.00   Hr     Added new callbacks
| 2008-09-10  1.09.00   Svh    Added new callback ApplXcpReadChecksumValue
| 2010-03-24  1.10.00   Hr     Added new callback ApplXcpCalibrationWrite/Read
| 2013-12-09  1.11.00   Hr     ESCAN00072503: Support custom CRC Cbk
|                       Hr     ESCAN00072505: Support Generic GET_ID
| 2015-03-26  1.12.00   Hr     ESCAN00082098: Time Check for DAQ lists
| 2016-11-18  1.12.01   Hr     ESCAN00091528: Compiler Warning: C++ Comment style used
|***************************************************************************/


/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include "XcpBasic.h"


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



extern tXcpData xcp;

/* Variable used to store the intermdiate timer value */



/*****************************************************************************/
/*                                                                           */
/* Implementation                                                            */
/*                                                                           */
/*****************************************************************************/


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

XcpDaqTimestampType ApplXcpGetTimestamp( void )
{
  /* Adapt this function to return a valid timestamp and remove the error message */
  #error "The function ApplXcpGetTimestamp must return a valid timestamp"
  return (XcpDaqTimestampType)0u;
}
  #endif /* XCP_ENABLE_DAQ_TIMESTAMP */

#endif /* XCP_ENABLE_DAQ */



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
  addr_ext = addr_ext;
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





#if defined ( XCP_ENABLE_DAQ )



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


