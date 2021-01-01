/*
 * xcp_driver.c
 *
 *  Created on: Jul 25, 2020
 *      Author: Ebenezer O
 */
#include "XcpBasic.h"
#include "xcp_driver.h"


// TODO: Init driver fields

// static void xcp_driver_can_device_init(struct xcp_driver_cblk* const cblk);
// static void xcp_driver_can_device_io_init(void);

 /****************************************************************************/
 /* Init                                                           */
 /****************************************************************************/
  void xcp_driver_init(struct xcp_driver_cblk* const cblk)
 {
     // TODO: driver init
     // xcp_driver_can_device_io_init();
     // xcp_driver_can_device_init(cblk);

     cblk->tx_count = 0;

     return;
 }
 
   //****************************************************************************
   // xcp_driver_transmit -  Call by Xcp Protocol stack
   // Precondition - transmit_length and buffer variable set by client
   //****************************************************************************
   void xcp_driver_transmit(struct xcp_driver_cblk* const cblk)
   {
       int32_t lSent      = 0;
	   int32_t lTotalSent = 0;

        // Clear the flag that indicates that tx msg has been sent.  This
        // flag will be set in the interrupt handler when a message has been
        // sent using tx msg.
        cblk->tx_flag = 0;

        // XCP_DRIVER_TX_BUFFER_LEN
        memmove(&cblk->transmit_buffer[4], &cblk->transmit_buffer[0], cblk->transmit_length);
        memset(&cblk->transmit_buffer[0], 0,4);
        cblk->transmit_buffer[1] = cblk->transmit_length; // update frame length
        cblk->transmit_buffer[3] = cblk->tx_count++;      // update counter

        cblk->transmit_length += 4;
        
		/* Call send() until all the data has been sent. */
		while( ( lSent >= 0 ) && ( lTotalSent < cblk->transmit_length ) )
		{
			lSent = FreeRTOS_send( cblk->connected_sock,  &cblk->transmit_buffer[0], cblk->transmit_length - lTotalSent, 0 );
			lTotalSent += lSent;
		}
        xcp_driver_transmit_complete(cblk); // TODO: impact of signaling transmit complete when actual transminssion hasn't commenced- need to confirm tranmission commenced at this point.

   }

   /****************************************************************************/
   /* xcp_driver_command_transmit                                                            */
   /****************************************************************************/
   void xcp_driver_transmit_complete(struct xcp_driver_cblk* const cblk)
   {        
      XcpSendCallBack();
       return;
   }
