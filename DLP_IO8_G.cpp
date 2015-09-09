/*
 DLP_IO8_G.cpp
 
 Description: Library for communication with DLP-IO8-G DAQ module
 
 Revision History
 08 Sep 2015 - Created and debugged
 
 Author: Lars Soltmann
 
 NOTE: Settings allow user to switch between ASCII and binary output but functions rely on binary output
 
 */

#include "DLP_IO8_G.h"

DLP_COM::DLP_COM(std::string name) : usb_device_name(name) {
    init_error=0;
}

int DLP_COM::connect()
{
    // Open serial port
    printf("Openning port... ");
    fflush(stdout);
    fd=open(usb_device_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    
    if(fd == -1)
    {
        printf("Failed!\n");
        init_error++;
    }
    else
    {
        fcntl(fd, F_SETFL, 0);
        printf("Done\n");
    }
    return init_error;
}

int DLP_COM::configure()
{
    printf("Applying port settings... ");
    fflush(stdout);
    struct termios port_settings;
    
    // Get current port settings
    tcgetattr(fd, &port_settings);
    
    // Flush buffer
    tcflush( fd, TCIOFLUSH );
    
    // Set baud rate
    cfsetispeed(&port_settings, B115200);
    cfsetospeed(&port_settings, B115200);
    
    // Set some port settings
    port_settings.c_cflag &= ~PARENB;           // parity disabled
    port_settings.c_cflag &= ~CSTOPB;           // use one stop bits
    port_settings.c_cflag &= ~CSIZE;
    port_settings.c_cflag |= CS8;
    port_settings.c_cflag &= ~CRTSCTS;          // use no flow control
    port_settings.c_cflag |= CREAD | CLOCAL;    // turn on READ & ignore ctrl lines
    
    // Send settings to device
    if (tcsetattr(fd, TCSANOW, &port_settings)==0){
        printf("Done\n");
    }
    else {
        printf("Failed!\n");
        init_error++;
    }
    // Flush one more time
    tcflush( fd, TCIOFLUSH );
    
    return init_error;
}

int DLP_COM::checkDevice()
{
    printf("Checking device... ");
    fflush(stdout);
    
    deviceSettingsWrite=ping_device;
    int send_check=write(fd, &deviceSettingsWrite, 1);
    if(send_check>0){
    }
    else {
        printf("Send failure!\n");
        init_error++;
    }
    // Data sheet says device will return 0x51 when pinged so check to make sure we have the right device and communication is functioning
    read(fd, &deviceCheck, 1);
    if (deviceCheck==0x51){
        printf("Done\n");
    }
    else {
        printf("Failed!\n");
        init_error++;
    }
    return init_error;
}

void DLP_COM::changeSettings(int A_or_B, int F_or_C){
    // Setting either ASCII or binary output (0 or 1) and setting either degF or degC output (0 or 1) ... giving and input of 0,0 is the same as the default settings
    printf("Applying user settings... ");
    fflush(stdout);
    if (A_or_B==0) {
        deviceSettingsWrite=setup_returnASCII;
    }
    else if (A_or_B==1){
        deviceSettingsWrite=setup_returnBinary;
    }
    else {
        deviceSettingsWrite=setup_returnASCII;
        printf("Invalid input for returned output type ... using default ASCII output.\n");
    }
    if (F_or_C==0) {
        deviceSettingsWrite2=setup_degF;
    }
    else if (F_or_C==1){
        deviceSettingsWrite2=setup_degC;
    }
    else {
        deviceSettingsWrite2=setup_degF;
        printf("Invalid input for output temperature units ... using default degF.\n");
    }
    write(fd, &deviceSettingsWrite, 1);
    write(fd, &deviceSettingsWrite2, 1);
    printf("Done\n");
    
}

void DLP_COM::setDigitalOutput(int ch_num, int H_or_L){
    if (ch_num==1 && H_or_L==1) {
        digitalOutWrite=ch1_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==1 && H_or_L==0) {
        digitalOutWrite=ch1_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==2 && H_or_L==1) {
        digitalOutWrite=ch2_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==2 && H_or_L==0) {
        digitalOutWrite=ch2_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==3 && H_or_L==1) {
        digitalOutWrite=ch3_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==3 && H_or_L==0) {
        digitalOutWrite=ch3_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==4 && H_or_L==1) {
        digitalOutWrite=ch4_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==4 && H_or_L==0) {
        digitalOutWrite=ch4_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==5 && H_or_L==1) {
        digitalOutWrite=ch5_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==5 && H_or_L==0) {
        digitalOutWrite=ch5_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==6 && H_or_L==1) {
        digitalOutWrite=ch6_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==6 && H_or_L==0) {
        digitalOutWrite=ch6_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==7 && H_or_L==1) {
        digitalOutWrite=ch7_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==7 && H_or_L==0) {
        digitalOutWrite=ch7_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==8 && H_or_L==1) {
        digitalOutWrite=ch8_setH;
        write(fd, &digitalOutWrite, 1);
    }
    else if (ch_num==8 && H_or_L==0) {
        digitalOutWrite=ch8_setL;
        write(fd, &digitalOutWrite, 1);
    }
    else {
        printf("Channel number (1-8) or digital output value (0 or 1) out of range!\n");
    }
}

int DLP_COM::getDigitalInput(int ch_num){
    if (ch_num==1) {
        digitalInWrite=ch1_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else if (ch_num==2) {
        digitalInWrite=ch2_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else if (ch_num==3) {
        digitalInWrite=ch3_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else if (ch_num==4) {
        digitalInWrite=ch4_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else if (ch_num==5) {
        digitalInWrite=ch5_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else if (ch_num==6) {
        digitalInWrite=ch6_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else if (ch_num==7) {
        digitalInWrite=ch7_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else if (ch_num==8) {
        digitalInWrite=ch8_digIN;
        write(fd, &digitalInWrite, 1);
        read(fd, &digitalInputValue, 1);
    }
    else {
        printf("Channel number (1-8) out of range!\n");
        digitalInputValue=-1;
    }
    return digitalInputValue;
    
}

double DLP_COM::getVoltage(int ch_num){
    if (ch_num==1) {
        voltageWrite=ch1_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else if (ch_num==2) {
        voltageWrite=ch2_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else if (ch_num==3) {
        voltageWrite=ch3_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else if (ch_num==4) {
        voltageWrite=ch4_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else if (ch_num==5) {
        voltageWrite=ch5_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else if (ch_num==6) {
        voltageWrite=ch6_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else if (ch_num==7) {
        voltageWrite=ch7_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else if (ch_num==8) {
        voltageWrite=ch8_volts;
        write(fd, &voltageWrite, 1);
        read(fd, &voltageCounts, 2);
    }
    else {
        printf("Channel number (1-8) out of range!\n");
        voltageCounts[0]=0;
        voltageCounts[1]=0;
    }
    
    voltageValue=(5.0/1023.0)*(voltageCounts[1] + (voltageCounts[0] << 8));
    voltageValue=voltageValue*voltageCal_scale+voltageCal_offset;
    
    return voltageValue;
}

double DLP_COM::getTemperature(int ch_num){
    
    // **** THIS FUNCTION IS UNTESTED SINCE I DON'T HAVE THE REQUIRED TEMPERATURE SENSOR!
    
    if (ch_num==1) {
        temperatureWrite=ch1_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else if (ch_num==2) {
        temperatureWrite=ch2_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else if (ch_num==3) {
        temperatureWrite=ch3_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else if (ch_num==4) {
        temperatureWrite=ch4_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else if (ch_num==5) {
        temperatureWrite=ch5_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else if (ch_num==6) {
        temperatureWrite=ch6_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else if (ch_num==7) {
        temperatureWrite=ch7_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else if (ch_num==8) {
        temperatureWrite=ch8_temp;
        write(fd, &temperatureWrite, 1);
        read(fd, &temperatureCounts, 2);
    }
    else {
        printf("Channel number (1-8) out of range!\n");
        temperatureCounts[0]=0;
        temperatureCounts[1]=0;
    }
    
    temperatureValue=temperatureCounts[1] + (temperatureCounts[0] << 8);

    return temperatureValue;
}
