# DLP_IO8_G_py.py
#        
# Description: Python 3 module for communication with USB DLP-IO8-G DAQ module
#            
# Revision History
# 11 Sep 2015 - Created and debugged
# 15 Dec 2016 - Updated code to work on both Raspberry Pi and Mac
#                    
# Author: Lars Soltmann
#                        
# NOTE: - Settings allow user to switch between ASCII and binary output but functions rely on binary output (A=ASCII, B=binary)
#       - Written for Python3



import serial
import sys


class DLP:
    def __init__(self,device,voltageCal_scale=1.0,voltageCal_offset=0.0): #Optional input are calibrations for voltage scale and offset for analog input measurements
        
        ## Voltage calibration constants
        self.voltageCal_offset= voltageCal_offset
        self.voltageCal_scale= voltageCal_scale
        
        ## Device name
        self.device_name=device
        
        ## Error flag for catching errors during initialization
        self.error_flag=0
        
        ## Create 2D list for DLP-IO8-G commands
        self.DLP_CMDS = [[0 for x in range(9)] for x in range(5)]

        ## Commands from DLP-IO8-G data sheet
        self.DLP_CMDS[0][1]= 0x31 #ch1_setH
        self.DLP_CMDS[1][1]= 0x51 #ch1_setL
        self.DLP_CMDS[2][1]= 0x41 #ch1_digIN
        self.DLP_CMDS[3][1]= 0x5A #ch1_volts
        self.DLP_CMDS[4][1]= 0x39 #ch1_temp

        self.DLP_CMDS[0][2]= 0x32 #ch2_setH
        self.DLP_CMDS[1][2]= 0x57 #ch2_setL
        self.DLP_CMDS[2][2]= 0x53 #ch2_digIN
        self.DLP_CMDS[3][2]= 0x58 #ch2_volts
        self.DLP_CMDS[4][2]= 0x30 #ch2_temp

        self.DLP_CMDS[0][3]= 0x33 #ch3_setH
        self.DLP_CMDS[1][3]= 0x45 #ch3_setL
        self.DLP_CMDS[2][3]= 0x44 #ch3_digIN
        self.DLP_CMDS[3][3]= 0x43 #ch3_volts
        self.DLP_CMDS[4][3]= 0x2D #ch3_temp

        self.DLP_CMDS[0][4]= 0x34 #ch4_setH
        self.DLP_CMDS[1][4]= 0x52 #ch4_setL
        self.DLP_CMDS[2][4]= 0x46 #ch4_digIN
        self.DLP_CMDS[3][4]= 0x56 #ch4_volts
        self.DLP_CMDS[4][4]= 0x3D #ch4_temp

        self.DLP_CMDS[0][5]= 0x35 #ch5_setH
        self.DLP_CMDS[1][5]= 0x54 #ch5_setL
        self.DLP_CMDS[2][5]= 0x47 #ch5_digIN
        self.DLP_CMDS[3][5]= 0x42 #ch5_volts
        self.DLP_CMDS[4][5]= 0x4F #ch5_temp

        self.DLP_CMDS[0][6]= 0x36 #ch6_setH
        self.DLP_CMDS[1][6]= 0x59 #ch6_setL
        self.DLP_CMDS[2][6]= 0x48 #ch6_digIN
        self.DLP_CMDS[3][6]= 0x4E #ch6_volts
        self.DLP_CMDS[4][6]= 0x50 #ch6_temp

        self.DLP_CMDS[0][7]= 0x37 #ch7_setH
        self.DLP_CMDS[1][7]= 0x55 #ch7_setL
        self.DLP_CMDS[2][7]= 0x4A #ch7_digIN
        self.DLP_CMDS[3][7]= 0x4D #ch7_volts
        self.DLP_CMDS[4][7]= 0x5B #ch7_temp

        self.DLP_CMDS[0][8]= 0x38 #ch8_setH
        self.DLP_CMDS[1][8]= 0x49 #ch8_setL
        self.DLP_CMDS[2][8]= 0x4B #ch8_digIN
        self.DLP_CMDS[3][8]= 0x2C #ch8_volts
        self.DLP_CMDS[4][8]= 0x5D #ch8_temp

        self.DLP_CMDS[0][0]= 0x60 #setup_returnASCII
        self.DLP_CMDS[1][0]= 0x5C #setup_returnBinary
        self.DLP_CMDS[2][0]= 0x4C #setup_degF
        self.DLP_CMDS[3][0]= 0x3B #setup_degC
        self.DLP_CMDS[4][0]= 0x27 #ping_device
    
    
    def initialize(self):
        ## Run through initialization process
        try:
            self.connect()
            self.checkDevice()
            return 0
        except:
            return 1
    


    def connect(self):
        print("Openning port... ", end='')
        ## Open connected device at specificed USB port
        self.USB=serial.Serial(self.device_name, 115200)
        ## Check to make sure the device opened properly
        port_check=self.USB.isOpen()
        if port_check:
            print("done")
            return 0
        else:
            return 1


    def checkDevice(self):
        print("Checking device... ", end='')
        ## Send the check device command and ensure that the correct identifier is returned
        self.USB.write(bytes([self.DLP_CMDS[4][0]]))
        [deviceCheck]=self.USB.read(1)
        if deviceCheck==0x51:
            print("done")
            return 0
        else:
            return 1


    def changeSettings(self,A_or_B,F_or_C):
        print("Applying settings... ", end='')
        ## Set the device to output ASCII or binary
        if A_or_B=='A':
            self.USB.write(bytes([self.DLP_CMDS[0][0]]))
        elif A_or_B=='B':
            self.USB.write(bytes([self.DLP_CMDS[1][0]]))
        else:
            print("Invalid input for returned output type ... using default ASCII output")
            self.USB.write(bytes([self.DLP_CMDS[0][0]]))
        ## Set the device to output degrees F or C
        if F_or_C=='F':
            self.USB.write(bytes([self.DLP_CMDS[2][0]]))
        elif F_or_C=='C':
            self.USB.write(bytes([self.DLP_CMDS[3][0]]))
        else:
            print("Invalid input for output temperature units ... using default degF")
            self.USB.write(bytes([self.DLP_CMDS[2][0]]))
        print("done")


    def setDigitalOutput(self,ch_num,level):
        ## Set digial output as either H or L, level should be '0' or '1'
        if ch_num>8 or ch_num<1 or level>1 or level<0:
            print("Channel number (1-8) or digital output value (0 or 1) out of range!")
        else:
            self.USB.write(bytes([self.DLP_CMDS[-level+1][ch_num]]))


    def getDigitalInput(self,ch_num):
        ## Get the digital input level for the current channel
        if ch_num>8 or ch_num<1:
            print("Channel number (1-8) out of range")
            digitalInputValue=-1
        else:
            self.USB.write(bytes([self.DLP_CMDS[2][ch_num]]))
            [digitalInputValue]=self.USB.read(1)
        return digitalInputValue


    def getVoltage(self,ch_num):
        ## Read the analog value of the current channel
        if ch_num>8 or ch_num<1:
            print("Channel number (1-8) out of range")
            voltageValue=-1
        else:
            self.USB.write(bytes([self.DLP_CMDS[3][ch_num]]))
            voltageCount=self.USB.read(2)
            voltageValue=(5/1023)*(voltageCount[1] + (voltageCount[0] << 8))
        return voltageValue


    ## NOTE: Temperature function is incomplete! Don't know/can't find the transfer function from binary/hex to decimal/understandable value.
    def getTemperature(self,ch_num):
        if ch_num>8 or ch_num<1:
            print("Channel number (1-8) out of range")
            temperatureValue=-1
        else:
            self.USB.write(bytes([self.DLP_CMDS[4][ch_num]]))
            temperatureCount=self.USB.read(2)
            temperatureValue=(temperatureCount[1] + (temperatureCount[0] << 8))
        return temperatureValue


    def disconnect(self):
        self.USB.close()
