/*
 * xcp_driver.c
 *
 *  Created on: Jul 25, 2020
 *      Author: Ebenezer O
 */
#include "XcpBasic.h"
#include "xcp_driver.h"


// TODO: Init driver fields

  static void xcp_driver_can_device_init(struct xcp_driver_cblk* const cblk);
  static void xcp_driver_can_device_io_init(void);

 /****************************************************************************/
 /* Init                                                           */
 /****************************************************************************/
  void xcp_driver_init(struct xcp_driver_cblk* const cblk)
 {
     // TODO: driver init
     xcp_driver_can_device_io_init();
     xcp_driver_can_device_init(cblk);

     return;
 }
 
 /****************************************************************************/
 /* xcp_driver_can_device_io_init                                                           */
 /****************************************************************************/
  static void xcp_driver_can_device_io_init(void)
 {
    // For this example CAN0 is used with RX and TX pins on port B4 and B5.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.
    // GPIO port B needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the GPIO pin muxing to select CAN0 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);

    // Enable the alternate function on the GPIO pins.  The above step selects
    // which alternate function is available.  This step actually enables the
    // alternate function instead of GPIO for these pins.
    // TODO: change this to match the port/pin you are using
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // The GPIO port and pins have been set up for CAN.  The CAN peripheral
    // must be enabled.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

     return;
 }
 
 /****************************************************************************/
 /* xcp_driver_can_device_init                                                           */
 /****************************************************************************/
  static void xcp_driver_can_device_init(struct xcp_driver_cblk* const cblk)
 {
    // Initialize the CAN controller
    CANInit(CAN0_BASE);

    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.  You can achieve more control
    // over the CAN bus timing by using the function CANBitTimingSet() instead
    // of this one, if needed.
    // In this example, the CAN bus is set to 500 kHz.  In the function below,
    // the call to SysCtlClockGet() or ui32SysClock is used to determine the 
    // clock rate that is used for clocking the CAN peripheral.  This can be 
    // replaced with a  fixed value if you know the value of the system clock, 
    // saving the extra function call.  For some parts, the CAN peripheral is 
    // clocked by a fixed 8 MHz regardless of the system clock in which case 
    // the call to SysCtlClockGet() or ui32SysClock should be replaced with 
    // 8000000.  Consult the data sheet for more information about CAN 
    // peripheral clocking.
#if defined(TARGET_IS_TM4C129_RA0) ||                                         \
    defined(TARGET_IS_TM4C129_RA1) ||                                         \
    defined(TARGET_IS_TM4C129_RA2)
    CANBitRateSet(CAN0_BASE, ui32SysClock, 500000);
#else
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
#endif

    // Enable interrupts on the CAN peripheral.  This example uses static
    // allocation of interrupt handlers which means the name of the handler
    // is in the vector table of startup code.  If you want to use dynamic
    // allocation of the vector table, then you must also call CANIntRegister()
    // here.
    //
    // CANIntRegister(CAN0_BASE, CANIntHandler); // if using dynamic vectors
    
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    // Enable the CAN interrupt on the processor (NVIC).
    IntEnable(INT_CAN0);
    
    // TODO: Set Loop back mode
#if defined(XCP_DRIVER_LOOPBACK_TEST_MODE)  
    // Test Mode set as precondition for Loop back
    HWREG(CAN0_BASE + CAN_O_CTL) |= CAN_CTL_TEST;
    while (!(HWREG(CAN0_BASE + CAN_O_CTL) & CAN_CTL_TEST)){}
    
    // set loop back 
    HWREG(CAN0_BASE + CAN_O_TST) |= CAN_TST_LBACK;
    while (!(HWREG(CAN0_BASE + CAN_O_TST) & CAN_TST_LBACK)){}    
#endif

    // Enable the CAN for operation.
    CANEnable(CAN0_BASE);

    // Initialize a message object to receive CAN messages with ID 0x1001.
    // The expected ID must be set along with the mask to indicate that all
    // bits in the ID must match.
    cblk->can_frame[0].ui32MsgID     = 0x1001;
    cblk->can_frame[0].ui32MsgIDMask = 0xffff;
    cblk->can_frame[0].ui32Flags  = (MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER |
                                     MSG_OBJ_EXTENDED_ID);
    cblk->can_frame[0].ui32MsgLen = 2;

    // TODO: Need unique CAN frame for tx and rx
    // Initialize message object 2 to be able to send CAN message 2.  This
    // message object is not shared so it only needs to be initialized one
    // time, and can be used for repeatedly sending the same message ID.
    cblk->can_frame[1].ui32MsgID = 0x2001;
    cblk->can_frame[1].ui32MsgIDMask = 0;
    cblk->can_frame[1].ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    //cblk->can_frame[1].ui32MsgLen = sizeof(cblk->receive_buffer.buffer[0]);
    //cblk->can_frame[0].pui8MsgData = &cblk->transmit_buffer[0];
    
#if defined(XCP_DRIVER_LOOPBACK_TEST_MODE)    
    cblk->can_frame[2].ui32MsgID     = 0x1001;
    cblk->can_frame[2].ui32MsgIDMask = 0;
    cblk->can_frame[2].ui32Flags     = MSG_OBJ_TX_INT_ENABLE;
    
    cblk->can_frame[3].ui32MsgID     = 0xffff;
    cblk->can_frame[3].ui32MsgIDMask = 0;
    cblk->can_frame[3].ui32Flags     = (MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER |
                                        MSG_OBJ_EXTENDED_ID);
    cblk->can_frame[3].ui32MsgLen = 2;
    CANMessageSet(CAN0_BASE, 4, &cblk->can_frame[3], MSG_OBJ_TYPE_RX);                                    
#endif
    // Now load the message object into the CAN peripheral message object 1.
    // Once loaded the CAN will receive any messages with this CAN ID into
    // this message object, and an interrupt will occur.
    CANMessageSet(CAN0_BASE, 1, &cblk->can_frame[0], MSG_OBJ_TYPE_RX);
    
    //Set config for transmit msg when ready to transmit
     return;
 }
  /****************************************************************************/
  /* xcp_driver_can_device_isr                                                           */
  /****************************************************************************/
void xcp_driver_can_device_isr(void)
{
    uint32_t status;
    struct xcp_driver_cblk* cblk = &XCP_DRIVER_CBLK;

    // Read the CAN interrupt status to find the cause of the interrupt
    status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
    
    if(status == CAN_INT_INTID_STATUS) // If the cause is a controller status interrupt, then get the status
    {
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.
        status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        cblk->error_flag = 1;  // Set a flag to indicate some errors may have occurred.
    }
    else if(status == 1) // Check if the cause is message object 1.
    {
        // Getting to this point means that the RX interrupt occurred on
        // message object 1, and the message reception is complete.  Clear the
        // message object interrupt.
        CANIntClear(CAN0_BASE, 1);

        // Increment a counter to keep track of how many messages have been
        // received.  In a real application this could be used to set flags to
        // indicate when a message is received.
        cblk->rx_count++;
        
        // Set flag to indicate received message is pending for this message
        // object.
        cblk->rx_flag = 1;

        // Since a message was received, clear any error flags.
        cblk->error_flag = 0;
    }
    else if(status == 2) // Check if the cause is message object 2.
    {
        CANIntClear(CAN0_BASE, 2);
        cblk->tx_count++;
        cblk->tx_flag = 1;
        cblk->error_flag = 0;
    }    
    else if (status == 3)
    {
        CANIntClear(CAN0_BASE, 3);
    }
    else if (status == 4)
    {
        CANIntClear(CAN0_BASE, 4);
        cblk->test_rx_flag = 1;
    }
    else // Spurious interrupt handling can go here.
    {

    }
}

//*****************************************************************************
//
// This function prints some information about the CAN message to the
// serial port for information purposes only.
//
//*****************************************************************************
void xcp_driver_print_can_frame_info(tCANMsgObject *psCANMsg, uint32_t ui32MsgObj, uint8_t rx_tx)
{
    unsigned int uIdx;

    if (rx_tx == 0) // RX
    {
        UARTprintf("RX msg: obj=%d ID=0x%04X msg=0x", ui32MsgObj,
               psCANMsg->ui32MsgID);
    }
    else
    {
        UARTprintf("TX msg: obj=%d ID=0x%04X msg=0x", ui32MsgObj,
               psCANMsg->ui32MsgID);
    }
    
    for(uIdx = 0; uIdx < psCANMsg->ui32MsgLen; uIdx++)
    {
        UARTprintf("%02X ", psCANMsg->pui8MsgData[uIdx]);
    }
    UARTprintf("\n");
}

   //****************************************************************************
   // xcp_driver_transmit -  Call by Xcp Protocol stack
   // Precondition - transmit_length and buffer variable set by client
   //****************************************************************************
   void xcp_driver_transmit(struct xcp_driver_cblk* const cblk)
   {
       // TODO: Populate driver transmit buffer with XCP packet
        cblk->can_frame[1].ui32MsgLen = cblk->transmit_length;
        cblk->can_frame[1].pui8MsgData = &cblk->transmit_buffer[0];

        // Clear the flag that indicates that tx msg has been sent.  This
        // flag will be set in the interrupt handler when a message has been
        // sent using tx msg.
        cblk->tx_flag = 0;

        // Now send tx msg using CAN controller tx msg.  This is
        // the first message sent using this message object.  The
        // CANMessageSet() function will cause the message to be sent right
        // away.
        xcp_driver_print_can_frame_info(&cblk->can_frame[1], 2, 1);
        CANMessageSet(CAN0_BASE, 2, &cblk->can_frame[1], MSG_OBJ_TYPE_TX);
   }

   /****************************************************************************/
   /* xcp_driver_command_transmit                                                            */
   /****************************************************************************/
   void xcp_driver_transmit_complete(struct xcp_driver_cblk* const cblk)
   {
       // TODO: Signal driver transmit complete -- call tx complete for error handling
       
        // Check the error flag to see if errors occurred
        if(cblk->error_flag)
        {
            UARTprintf(" error - cable connected?\n");
        }
        else
        {
            // If no errors then print the count of message sent
            UARTprintf(" total count = %u\n", cblk->rx_count + cblk->tx_count);
        }
        
       XcpSendCallBack();
       return;
   }
