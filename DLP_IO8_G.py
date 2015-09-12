# DLP_IO8_G.py
#        
# Description: Python 3 module for communication with USB DLP-IO8-G DAQ module
#            
# Revision History
# 11 Sep 2015 - Created and debugged
#                    
# Author: Lars Soltmann
#                        
# NOTE: Settings allow user to switch between ASCII and binary output but functions rely on binary output



import serial
import sys

global DLP_CMDS
DLP_CMDS = [[0 for x in range(9)] for x in range(5)]

## Commands from DLP-IO8-G data sheet
DLP_CMDS[0][1]= 0x31 #ch1_setH
DLP_CMDS[1][1]= 0x51 #ch1_setL
DLP_CMDS[2][1]= 0x41 #ch1_digIN
DLP_CMDS[3][1]= 0x5A #ch1_volts
DLP_CMDS[4][1]= 0x39 #ch1_temp

DLP_CMDS[0][2]= 0x32 #ch2_setH
DLP_CMDS[1][2]= 0x57 #ch2_setL
DLP_CMDS[2][2]= 0x53 #ch2_digIN
DLP_CMDS[3][2]= 0x58 #ch2_volts
DLP_CMDS[4][2]= 0x30 #ch2_temp

DLP_CMDS[0][3]= 0x33 #ch3_setH
DLP_CMDS[1][3]= 0x45 #ch3_setL
DLP_CMDS[2][3]= 0x44 #ch3_digIN
DLP_CMDS[3][3]= 0x43 #ch3_volts
DLP_CMDS[4][3]= 0x2D #ch3_temp

DLP_CMDS[0][4]= 0x34 #ch4_setH
DLP_CMDS[1][4]= 0x52 #ch4_setL
DLP_CMDS[2][4]= 0x46 #ch4_digIN
DLP_CMDS[3][4]= 0x56 #ch4_volts
DLP_CMDS[4][4]= 0x3D #ch4_temp

DLP_CMDS[0][5]= 0x35 #ch5_setH
DLP_CMDS[1][5]= 0x54 #ch5_setL
DLP_CMDS[2][5]= 0x47 #ch5_digIN
DLP_CMDS[3][5]= 0x42 #ch5_volts
DLP_CMDS[4][5]= 0x4F #ch5_temp

DLP_CMDS[0][6]= 0x36 #ch6_setH
DLP_CMDS[1][6]= 0x59 #ch6_setL
DLP_CMDS[2][6]= 0x48 #ch6_digIN
DLP_CMDS[3][6]= 0x4E #ch6_volts
DLP_CMDS[4][6]= 0x50 #ch6_temp

DLP_CMDS[0][7]= 0x37 #ch7_setH
DLP_CMDS[1][7]= 0x55 #ch7_setL
DLP_CMDS[2][7]= 0x4A #ch7_digIN
DLP_CMDS[3][7]= 0x4D #ch7_volts
DLP_CMDS[4][7]= 0x5B #ch7_temp

DLP_CMDS[0][8]= 0x38 #ch8_setH
DLP_CMDS[1][8]= 0x49 #ch8_setL
DLP_CMDS[2][8]= 0x4B #ch8_digIN
DLP_CMDS[3][8]= 0x2C #ch8_volts
DLP_CMDS[4][8]= 0x5D #ch8_temp

DLP_CMDS[0][0]= 0x60 #setup_returnASCII
DLP_CMDS[1][0]= 0x5C #setup_returnBinary
DLP_CMDS[2][0]= 0x4C #setup_degF
DLP_CMDS[3][0]= 0x3B #setup_degC
DLP_CMDS[4][0]= 0x27 #ping_device


#Voltage calibration constants
voltageCal_offset= 0.0
voltageCal_scale= 1.0


def connect(device_name):
    print("Openning port... ", end='')
    sys.stdout.flush()
    error_flag=0
    USB=serial.Serial(device_name, 115200)
    port_check=USB.isOpen()
    if port_check:
        print("done")
    else:
        print("failed!")
        error_flag+=1
    return (USB,error_flag)

def checkDevice(USB):
    print("Checking device... ", end='')
    sys.stdout.flush()
    error_flag=0
    USB.write([DLP_CMDS[4][0]])
    [deviceCheck]=USB.read(1)
    if deviceCheck==0x51:
        print("done")
    else:
        print("failed!")
        error_flag+=1
    return error_flag

def changeSettings(USB,A_or_B,F_or_C):
    print("Applying settings... ", end='')
    sys.stdout.flush()
    if A_or_B=='A':
        USB.write([DLP_CMDS[0][0]])
    elif A_or_B=='B':
        USB.write([DLP_CMDS[1][0]])
    else:
        print("Invalid input for returned output type ... using default ASCII output")
        USB.write([DLP_CMDS[0][0]])
    if F_or_C=='F':
        USB.write([DLP_CMDS[2][0]])
    elif F_or_C=='C':
        USB.write([DLP_CMDS[3][0]])
    else:
        print("Invalid input for output temperature units ... using default degF")
        USB.write([DLP_CMDS[2][0]])
    print("done")


def setDigitalOutput(USB,ch_num,level):
    if ch_num>8 or ch_num<1 or level>1 or level<0:
        print("Channel number (1-8) or digital output value (0 or 1) out of range!")
    else:
        USB.write([DLP_CMDS[-level+1][ch_num]])

def getDigitalInput(USB,ch_num):
    if ch_num>8 or ch_num<1:
        print("Channel number (1-8) out of range")
        digitalInputValue=-1
    else:
        USB.write([DLP_CMDS[2][ch_num]])
        [digitalInputValue]=USB.read(1)
    return digitalInputValue

def getVoltage(USB,ch_num):
    if ch_num>8 or ch_num<1:
        print("Channel number (1-8) out of range")
        voltageValue=-1
    else:
        USB.write([DLP_CMDS[3][ch_num]])
        voltageCount=USB.read(2)
        voltageValue=(5/1023)*(voltageCount[1] + (voltageCount[0] << 8))
    return voltageValue

## NOTE: Temperature function is incomplete! Don't know/can't find the transfer function from binary/hex to decimal/understandable value.
def getTemperature(ch_num):
    if ch_num>8 or ch_num<1:
        print("Channel number (1-8) out of range")
        temperatureValue=-1
    else:
        USB.write([DLP_CMDS[4][ch_num]])
        temperatureCount=USB.read(2)
        temperatureValue=(temperatureCount[1] + (temperatureCount[0] << 8))
    return temperatureValue
