## Raspberry Pi Board Bring-up Summary
1. Integrated FreeRTOS to Raspberry Pi
2. Integrated vector basic xcp server stack using FreeRTOS as executive environment

## Key Notes
### Pre-requisites
- Raspberry Pi with Raspbian OS (I used Model B)
- Raspberry Pi connected to internet (I connected to a Modem so I could configure the XCP server IP address as desired in /raspbian_bringup/Configs/Include/FreeRTOSConfig.h)
- Clone /raspbian_bringup in your Raspberry Pi
- Ensure libpcap is installed on your Raspberry Pi (can install using `sudo apt-get install libpcap-dev`)
- Another Device (other than Raspberry Pi) that supports python3
### Build and Testing Instruction
1. Clone /raspbian_bringup into your Raspberry Pi File system
2. Manually /raspbian_bringup/Configs/Include/FreeRTOSConfig.h with a valid IP address for your local network Netmask
3. in your Raspbian prompt, run `make clean` and then run `make` to build XCP TCP/IP server. This generates the build directory in /raspbian_bringup
4. run `cd build` to go into the build directory
5. run `sudo ./posix_demo` to start the tcp server -- you should see the TCP server start with configured IP address of your TCP server and a stream of captured TCP packets
6. run `python3 /raspbian_bringup/xcp_tcp_client.py` Using another device (other than your Raspberry Pi) as a TCP client. Ensure `User Defined` block of `/raspbian_bringup/xcp_tcp_client.py` is updated per comment before running it.
7. observe configuration of DAQ and DAQ monitoring on the TCP Client

### Known Issues
- Yet to test sending a request to server while DAQ transmission is running
- Remove FreeRTOS files not relevant to realizing concept.

## Folders/Files
### [Configs Folder](/raspbian_bringup/Configs)
- Has xcp application code to define required application entity exercising xcp
- Configured tcp driver as transport layer for XCP server
- Has test utilities definition to exercise unit testing of integration efforts
- Has test cases for DAQ:
	- 3 DAQ list
	- 3 ODT Entry per DAQ list
	- 1st ODT has 2 byte timestamp and 1 ODT Entry 
		- 1 byte data with values 0x99, 0x88, 0x77 for each DAQ list respectively
	- 2nd ODT has 2 ODT Entry
		- 4 byte data with values 0xffffffff, 0xeeeeeeee, 0xdddddddd for each DAQ list respectively
		- 2 byte data with values 0xcccc, 0xbbbb, 0xaaaa for each DAQ list respectively
### [FreeRTOS Folder](/raspbian_bringup/FreeRTOS)
- Has stack used for RTOS port
### [Vector Folder](/raspbian_bringup/Vector)
- Has vector xcp application protocol stack
### [Makefile](/raspbian_bringup/Makefile)
- Contains BOM of relevant artifacts

