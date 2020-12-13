# XCP-Learnings
An evaluation of XCP protocol while integrating 3rd party sw - vector xcp server stack and FreeRTOS on TIVA C Series board (XCP on CAN use case) and Raspberry Pi (XCP on TCP/IP use case) board

## Concepts
- [Porting FreeRTOS to TIVA-C- Cortex M4-f Notes](/PortingFreeRTOStoTIVA-C-CortexM4-f.pdf)
- [XCP Server App High Level Sketch](/XCPServerAppConcept.pdf)
  ![Example of XCP on CAN Messages TIVA board outputs when exercised](/XCPOnCANOutputSnip.png)

## Tiva Board Bring-up Summary
[/tiva_board_bringup/README](/tiva_board_bringup/README.md) has keynotes regarding Testing, Building and Source description related to XCP on CAN use cases.

## RaspberryPi Board Bring-up Summary
- [ ] Will contain XCP on TCP/IP server executing on a POSIX-GCC FreeRTOS port (aka a simulation port) running on a Raspbian OS
- [ ] Install a Rasbian distro