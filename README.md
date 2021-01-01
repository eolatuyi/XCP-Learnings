# XCP-Learnings
An evaluation of XCP protocol while integrating 3rd party sw - vector xcp server stack and FreeRTOS on TIVA C Series board (XCP on CAN use case) and Raspberry Pi (XCP on TCP/IP use case) board

## RaspberryPi Board Bring-up Summary
[/raspbian_bringup/README](/raspbian_bringup/README.md) has keynotes regarding Testing, Building and Source description related to XCP on TCP/IP use cases. Contain XCP on TCP/IP server executing on a POSIX-GCC FreeRTOS port (aka a simulation port) running on a Raspbian OS

## Tiva Board Bring-up Summary
[/tiva_board_bringup/README](/tiva_board_bringup/README.md) has keynotes regarding Testing, Building and Source description related to XCP on CAN use cases.

## Concepts
- [Porting FreeRTOS to TIVA-C- Cortex M4-f Notes](/PortingFreeRTOStoTIVA-C-CortexM4-f.pdf)
- [XCP Server App High Level Sketch](/XCPServerAppConcept.pdf)
- Example of XCP on TCP Messages Raspberry Pi outputs when exercised
  ![Example of XCP on TCP Messages Raspberry Pi outputs when exercised](/XCPonTCPDAQDemoOutputSnip.gif)
- Example of XCP on CAN Messages TIVA board outputs when exercised
  ![Example of XCP on CAN Messages TIVA board outputs when exercised](/XCPOnCANOutputSnip.png)


## Future work
### Improve Testability
- [ ] Simplify bringup instructions
- [ ] integrate third party unit/regression testing framework for continuous integration

### Platform extension
- [ ] Cortex M4 Ethernet driver integration with XCP server
- [ ] CAN Tranceiver device driver (transition from loopback to actual standalone XCP on CAN server)

### Performance measures
- [ ] Performance evaluation of FreeRTOS application hard timing requirement
- [ ] AUTOSAR pointered method 
- [ ] XCP DAQ monitoring (fastest logging rates)

### Improve documentation
- [ ] use good read me templates described in https://github.com/PurpleBooth/a-good-readme-template/blob/main/README.md 

