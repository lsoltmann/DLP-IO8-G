/*
 DLP_IO8_G.h
 
 Description: Library for communication with DLP-IO8-G DAQ module
 
 Revision History
 08 Sep 2015 - Created and debugged
 
 Author: Lars Soltmann
 */

#ifndef ____DLP_IO8_G__
#define ____DLP_IO8_G__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <iostream>

#define ch1_setH 0x31
#define ch1_setL 0x51
#define ch1_digIN 0x41
#define ch1_volts 0x5A
#define ch1_temp 0x39

#define ch2_setH 0x32
#define ch2_setL 0x57
#define ch2_digIN 0x53
#define ch2_volts 0x58
#define ch2_temp 0x30

#define ch3_setH 0x33
#define ch3_setL 0x45
#define ch3_digIN 0x44
#define ch3_volts 0x43
#define ch3_temp 0x2D

#define ch4_setH 0x34
#define ch4_setL 0x52
#define ch4_digIN 0x46
#define ch4_volts 0x56
#define ch4_temp 0x3D

#define ch5_setH 0x35
#define ch5_setL 0x54
#define ch5_digIN 0x47
#define ch5_volts 0x42
#define ch5_temp 0x4F

#define ch6_setH 0x36
#define ch6_setL 0x59
#define ch6_digIN 0x48
#define ch6_volts 0x4E
#define ch6_temp 0x50

#define ch7_setH 0x37
#define ch7_setL 0x55
#define ch7_digIN 0x4A
#define ch7_volts 0x4D
#define ch7_temp 0x5B

#define ch8_setH 0x38
#define ch8_setL 0x49
#define ch8_digIN 0x4B
#define ch8_volts 0x2C
#define ch8_temp 0x5D

#define setup_returnASCII 0x60
#define setup_returnBinary 0x5C
#define setup_degF 0x4C
#define setup_degC 0x3B

#define ping_device 0x27

// Voltage calibration constants
#define voltageCal_offset 0.0
#define voltageCal_scale 1.0


class DLP_COM {
public:
    DLP_COM(std::string name = "/dev/tty.usbserial-12345678");
    int connect();
    int configure();
    int checkDevice();
    void setDigitalOutput(int ch_num, int H_or_L);
    int getDigitalInput(int ch_num);
    double getVoltage(int ch_num);
    double getTemperature(int ch_num);
    void changeSettings(int A_or_B, int F_or_C);
    
private:
    std::string usb_device_name;
    int fd;
    int init_error;
    int ch_num;
    int H_or_L;
    int digitalInputValue;
    uint8_t voltageCounts[2];
    double voltageValue;
    uint8_t temperatureCounts[2];
    double temperatureValue;
    int A_or_B;
    int F_or_C;
    unsigned char deviceCheck;
    unsigned char digitalOutWrite;
    unsigned char digitalInWrite;
    unsigned char voltageWrite;
    unsigned char temperatureWrite;
    unsigned char deviceSettingsWrite;
    unsigned char deviceSettingsWrite2;

};
#endif /* defined(____DLP_IO8_G__) */
