#!/usr/bin/env python
import socket
import binascii

## -- User Defined
TCP_IP = '10.0.0.71' # Check IP address configured in /raspbian_bringup/Configs/Include/FreeRTOSConfig.h for value
TCP_PORT = 7

uint8_addr  = 0x000681d8 # Check the /build/posix_demo.map for value 
uint16_addr = 0x000681dc # Check the /build/posix_demo.map for value 
uint32_addr = 0x000681e4 # Check the /build/posix_demo.map for value 

MAX_RECV_DAQ_PACKETS = 500 # This value indicate how many DAQ messages will be received before this client terminates
## --

BUFF_LEN = 1024

tx_counter = 0
rx_counter = 0
loop_count = 0

def convert_be_addr_2_le_str(hex_addr):
    a = hex_addr.to_bytes(4, byteorder='little')
    out = ""
    for b in a: out = out + "{:02x}".format(b)
    return out

def print_rx_msg(data):
    global loop_count

    # parse xcp packet
    len_idx = 0
    ctr_idx = 2
    for i, elem in enumerate(data):
        if i == len_idx:
            msg_len = int(binascii.hexlify(data[i:i+2]), 16) # intel big endianess
        if i == ctr_idx:
            ctr = int(binascii.hexlify(data[i:i+2]), 16) # intel big endianess
            msg = bytearray()
        if i > ctr_idx + 1:
            msg = msg + bytes([data[i]])
            if len(msg) == msg_len:
                len_idx = i + 1
                ctr_idx = len_idx + 2
                msg = bytes([0]) + bytes([msg_len]) + bytes([0]) + bytes([ctr]) + msg
                print_xcp_msg("RX", msg)
                loop_count = loop_count + 1
                if loop_count >= MAX_RECV_DAQ_PACKETS:return

def print_xcp_msg(msg_type, msg):
    if msg_type == "TX":
        out = "TX: 0x"
    if msg_type == "RX":
        out = "RX: 0x"
    for m in msg: out = out + "{:02x} ".format(m)
    print(out)

     
def xcp_transaction(sock, reqst, recv_buff_len):
    global tx_counter
    if len(reqst) > 255:
        print("Error: length of xcp request too long")
        print_xcp_msg("XX", msg)
        return
     
    reqst = "{:04x}".format(len(reqst)) + "{:04x}".format(tx_counter) + reqst
    tx_counter = (tx_counter + 1) % 256   
    msg = bytearray.fromhex(reqst)
    print_xcp_msg("TX", msg)
    sock.sendall(msg)
    #input("Press Enter to continue...")
    data = s.recv(recv_buff_len)
    print_rx_msg(data)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

#                0x000671d8                test_xcp_uint8[3]
#                0x000671dc                test_xcp_uint16[3]
#                0x000671e4                test_xcp_uint32[3]
#xcp_transaction(s, "f6000000d8710600", 1024) # set MTA test_xcp_uint8[0]
#xcp_transaction(s, "f501", 1024) # read test_xcp_uint8[0]
#xcp_transaction(s, "f501", 1024) # read test_xcp_uint8[1]
#xcp_transaction(s, "f501", 1024) # read test_xcp_uint8[2]

#xcp_transaction(s, "f6000000dc710600", 1024) # set MTA test_xcp_uint16[0]
#xcp_transaction(s, "f502", 1024) # read test_xcp_uint16[0]
#xcp_transaction(s, "f502", 1024) # read test_xcp_uint16[1]
#xcp_transaction(s, "f502", 1024) # read test_xcp_uint16[2]

xcp_transaction(s, "ff00", BUFF_LEN) # connect

xcp_transaction(s, "d6", BUFF_LEN) # FREE_DAQ

xcp_transaction(s, "d5000300", BUFF_LEN) # ALLOC_DAQ count=3

xcp_transaction(s, "d400000002", BUFF_LEN) # ALLOC_ODT daq=0, count=2
xcp_transaction(s, "d400010002", BUFF_LEN) # ALLOC_ODT daq=1, count=2
xcp_transaction(s, "d400020002", BUFF_LEN) # ALLOC_ODT daq=2, count=2

xcp_transaction(s, "d30000000001", BUFF_LEN) # ALLOC_ODT_ENTRY daq=0, odt=0, count=1
xcp_transaction(s, "d30001000001", BUFF_LEN) # ALLOC_ODT_ENTRY daq=1, odt=0, count=1
xcp_transaction(s, "d30002000001", BUFF_LEN) # ALLOC_ODT_ENTRY daq=2, odt=0, count=1
xcp_transaction(s, "d30000000102", BUFF_LEN) # ALLOC_ODT_ENTRY daq=0, odt=1, count=2
xcp_transaction(s, "d30001000102", BUFF_LEN) # ALLOC_ODT_ENTRY daq=1, odt=1, count=2
xcp_transaction(s, "d30002000102", BUFF_LEN) # ALLOC_ODT_ENTRY daq=2, odt=1, count=2
    
xcp_transaction(s, "e20000000000", BUFF_LEN) # SET_DAQ_PTR daq=0,odt=0,idx=0
xcp_transaction(s, "e1ff0100" + convert_be_addr_2_le_str(uint8_addr), BUFF_LEN) # WRITE_DAQ size=1,addr=000671d8h,00h
xcp_transaction(s, "e2ff00000100", BUFF_LEN) # SET_DAQ_PTR daq=0,odt=1,idx=0
xcp_transaction(s, "e1ff0400" + convert_be_addr_2_le_str(uint32_addr), BUFF_LEN) # WRITE_DAQ size=4,addr=000671e4h,00h
xcp_transaction(s, "e2ff00000101", BUFF_LEN) # SET_DAQ_PTR daq=0,odt=1,idx=1
xcp_transaction(s, "e1ff0200" + convert_be_addr_2_le_str(uint16_addr), BUFF_LEN) # WRITE_DAQ size=2,addr=000671dch,00h

xcp_transaction(s, "e2ff01000000", BUFF_LEN) # SET_DAQ_PTR daq=1,odt=0,idx=0
xcp_transaction(s, "e1ff0100" + convert_be_addr_2_le_str(uint8_addr + 1), BUFF_LEN) # WRITE_DAQ size=1,addr=000671d9h,00h
xcp_transaction(s, "e2ff01000100", BUFF_LEN) # SET_DAQ_PTR daq=1,odt=1,idx=0
xcp_transaction(s, "e1ff0400" + convert_be_addr_2_le_str(uint32_addr + 4), BUFF_LEN) # WRITE_DAQ size=4,addr=000671e8h,00h
xcp_transaction(s, "e2ff01000101", BUFF_LEN) # SET_DAQ_PTR daq=1,odt=1,idx=1
xcp_transaction(s, "e1ff0200" + convert_be_addr_2_le_str(uint16_addr + 2), BUFF_LEN) # WRITE_DAQ size=2,addr=000671deh,00h

xcp_transaction(s, "e2ff02000000", BUFF_LEN) # SET_DAQ_PTR daq=2,odt=0,idx=0
xcp_transaction(s, "e1ff0100" + convert_be_addr_2_le_str(uint8_addr + 2*1), BUFF_LEN) # WRITE_DAQ size=1,addr=000671dah,00h
xcp_transaction(s, "e2ff02000100", BUFF_LEN) # SET_DAQ_PTR daq=2,odt=1,idx=0
xcp_transaction(s, "e1ff0400" + convert_be_addr_2_le_str(uint32_addr + 2*4), BUFF_LEN) # WRITE_DAQ size=4,addr=000671ech,00h
xcp_transaction(s, "e2ff02000101", BUFF_LEN) # SET_DAQ_PTR daq=2,odt=1,idx=1
xcp_transaction(s, "e1ff0200" + convert_be_addr_2_le_str(uint16_addr + 2*2), BUFF_LEN) # WRITE_DAQ size=2,addr=000671e0h,00h

xcp_transaction(s, "e0100000000001ff", BUFF_LEN) # SET_DAQ_LIST_MODE daq=0, mode=10h, prescaler=1, eventchannel=0
xcp_transaction(s, "e0100100010001ff", BUFF_LEN) # SET_DAQ_LIST_MODE daq=1, mode=10h, prescaler=1, eventchannel=1
xcp_transaction(s, "e0100200020001ff", BUFF_LEN) #SET_DAQ_LIST_MODE daq=2, mode=10h, prescaler=1, eventchannel=2

xcp_transaction(s, "df100000", BUFF_LEN) # GET_DAQ_LIST_MODE daq=0
xcp_transaction(s, "df100100", BUFF_LEN) # GET_DAQ_LIST_MODE daq=1
xcp_transaction(s, "df100200", BUFF_LEN) # GET_DAQ_LIST_MODE daq=2

xcp_transaction(s, "de020000", BUFF_LEN) # START_STOP mode=02h, daq=0
xcp_transaction(s, "de020100", BUFF_LEN) # START_STOP mode=02h, daq=1
xcp_transaction(s, "de020200", BUFF_LEN) # START_STOP mode=02h, daq=2

xcp_transaction(s, "dd01", BUFF_LEN) # CC_START_STOP_SYNCH mode=01h


loop_count = 0
while 1:
    data = s.recv(BUFF_LEN) 
    print_rx_msg(data)
    if loop_count >= MAX_RECV_DAQ_PACKETS: break
        
s.close() 
