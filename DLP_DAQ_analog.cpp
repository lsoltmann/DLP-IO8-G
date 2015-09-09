/*
 DLP_DAQ_analog.cpp
 
 Description: Basic data logging program for use with DLP-IO8-G DAQ module
 
 Revision History
 08 Sep 2015 - Created and debugged
 
 Author: Lars Soltmann
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/mman.h>
#include <math.h>
#include "DLP_IO8_G.h"

double sampTime;
double sleeptime, dt1, dt2;
struct timeval tv;
unsigned long t1, t2, tstart;
time_t current_time;


DLP_COM daq;


int main(int argc, char* argv[]){
    // Make sure input was given correctly, should be: sample rate(Hz) <space> active port # <space> active port # ... etc
    // for example ./DAQ 2 1 2 3 4 means sample at 2Hz on ports 1, 2, 3, and 4
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " sampleRate(Hz) activePort# activePort# ... etc" << std::endl;
        return 1;
    }
    
    sampTime=1.0/std::stoi(argv[1]);
    
    // Begin initialization of DAQ module
    if(daq.connect()!=0){
        return 1;
    }
    if(daq.configure()!=0){
        return 1;
    }
    if(daq.checkDevice()!=0){
        return 1;
    }
    // Set DAQ module to binary output
    daq.changeSettings(1,0);
    
    
    // Setup logfile
    FILE *logf;
    current_time=time(NULL);
    struct tm *t=std::localtime(&current_time);
    char logfile[30];
    char filetime[30];
    char filedate[30];
    strftime(logfile,sizeof(logfile),"logfile_%m%d%y_%H%M.txt",t);
    strftime(filedate,sizeof(filedate),"%c",t);
    logf=fopen(logfile,"w");
    if (logf==0) {
        printf("Failed to setup logfile!\n");
        return 1;
    }
    fprintf(logf,"Device: DLP-IO8-G\n\n");
    fprintf(logf,"%s\n\n",filedate);
    fprintf(logf,"Sampling Frequency (Hz):\n");
    fprintf(logf,"%d\n\n",std::stoi(argv[1]));
    fprintf(logf,"Channels Active:\n");
    for (int i = 2; i < argc; ++i) {
        fprintf(logf,"%d ",std::stoi(argv[i]));
    }
    fprintf(logf,"\n\n");
    fprintf(logf,"Time(s) ");
    for (int i = 2; i < argc; ++i) {
        fprintf(logf,"CH%d(V) ",std::stoi(argv[i]));
    }
    fflush(logf);
    
    printf("\n\n\n");
    printf("Beginning data acquisition...\n");
    
    // Get time before main data loop starts running, that way we can get a complete time history of everything.
    gettimeofday(&tv,NULL);
    tstart = 1000000 * tv.tv_sec + tv.tv_usec;
    
    while (1) {
        // Reference time point
        gettimeofday(&tv,NULL);
        t1 = 1000000 * tv.tv_sec + tv.tv_usec;
        for (int i = 2; i < argc; ++i) {
            printf("Channel %d: %.3f\n",std::stoi(argv[i]),daq.getVoltage(std::stoi(argv[i])));
        }
        printf("\n");
        // Now that all data has been taken, determin how long to sleep before next loop.
        gettimeofday(&tv,NULL);
        t2 = 1000000 * tv.tv_sec + tv.tv_usec;
        dt1 = (t2 - t1) / 1000000.0;
        dt2 = (t2 - tstart) / 1000000.0;
        
        // WRITE TO FILE HERE - currently just prints data to screen in real time
        
        if (dt1 < sampTime) {
            sleeptime=(sampTime-dt1)*1000000.0;
            usleep(sleeptime);
        }
    }

    return 0;
}
