## Tiva Board Bring-up Summary
1. Integrated FreeRTOS to TIVA C- Series - CORTEX M4-F
2. Integrated vector basic xcp server stack using FreeRTOS as executive environment
3. IDE used TI code composer studio

## Key Notes
### Pre-requisites
- Windows PC 7 and up. (I tested on windows 10)
- Code Composer Studio IDE (I used ccs10)
- EK-TM4C123GXL ARM  Cortex-M4F Based MCU TM4c123G LaunchPad Evaluation Kit (TIVA C Board)
- Connect board as a USB device to ensure device is installed in your PC (Confirm from device manager you see one of the COM port with name "Stellaris Virtual Serial Port")
### Build and Testing Instruction
1. Import [tiva_board_bringup directory](/tiva_board_bringup) as a CCS project
2. Build imported project in CCS
3. Connect TIVA board as a USB device (ensure dip switch is set as debug)
4. Launch Debug on CCS
5. On PC open uart com port tiva device is connected to (115200 baudrate, 8N1) - I used Putty for this
6. add `xcp_driver_cblk.test_var.execute_daq_test` to CCS debugger expression window and set value to `1`
7. add `tester_utilities_stimulator_cblk.trigger_generator` to CCS debugger expression window and set value to `1`
8. observe configuration of DAQ and DAQ monitoring on com port
### Known Issues
- CAN driver only configured for loopback mode in order to exercise xcp on CAN server
- Vector xcp server stack allows configuration of ODT whose size exceeds MAX_DTO for CAN- 
  scenario is when configured timestamp is >= 2byte. This leads to ASSERT failure during DAQ monitoring. 
  Does this mean vector expects application to have prevented such ODT config? I don't agree-- I think Vector stack should handle this.

## Folders/Files
### [Configs Folder](/tiva_board_bringup/Configs)
- Has xcp application code to define required application entity exercising xcp
- Configured CAN driver as transport layer for XCP server
- Has test utilities definition to exercise unit testing of integration efforts
- Unit testing is currently based on UART based monitoring and ccs debugger based test triggers
- Has test cases for DAQ:
	- 3 DAQ list
	- 3 ODT Entry per DAQ list
	- 1st ODT has 2 byte timestamp and 1 ODT Entry 
		- 1 byte data with values 0x99, 0x88, 0x77 for each DAQ list respectively
	- 2nd ODT has 2 ODT Entry
		- 4 byte data with values 0xffffffff, 0xeeeeeeee, 0xdddddddd for each DAQ list respectively
		- 2 byte data with values 0xcccc, 0xbbbb, 0xaaaa for each DAQ list respectively
### [FreeRTOS Folder](/tiva_board_bringup/FreeRTOS)
- Has stack used for RTOS port
### [Vector Folder](/tiva_board_bringup/Vector)
- Has vector xcp application protocol stack
### [TI Folder](/tiva_board_bringup/TI)
- has TI lib for drivers 
### [tm4c123gh6pm_startup_ccs.c File](/tiva_board_bringup/tm4c123gh6pm_startup_ccs.c)
- Configured isr for FreeRTOS and drivers in tm4c123gh6pm_startup_ccs.c
### [tiva_board_bringup.log File](/tiva_board_bringup/tiva_board_bringup.log)
- Has uart output of lastest test log of XCP server app.

