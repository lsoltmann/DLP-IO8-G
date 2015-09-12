# DLP_DAQ_analog.py
#    
# Description: Basic data logging script for use with DLP-IO8-G DAQ module
#        
# Revision History
# 11 Sep 2015 - Created and debugged
#                
# Author: Lars Soltmann

import sys
import time
import DLP_IO8_G as DAQ

# Import the sample rate and active channels from the command line and make sure there are enough inputs
if len(sys.argv)<3:
    print("Usage: "+sys.argv[0]+" sampleRate(Hz) activePort# activePort# ... etc")
    sys.exit(0)

sampTime=1/int(sys.argv[1])

#Begin device initialization
[USB,ef]=DAQ.connect("/dev/tty.usbserial-12345678")
if ef != 0:
    sys.exit(0)
ef=DAQ.checkDevice(USB)
if ef != 0:
    sys.exit(0)
DAQ.changeSettings(USB,"B","F")

timestr = time.strftime("%m%d%Y_%H%M")
logf = open('logfile_'+timestr+'.txt', 'w')

logf.write("Device: DLP-IO8-G\n\n")
logf.write(time.strftime("%c")+"\n\n")
logf.write("Sampling Frequency (Hz):\n")
logf.write(sys.argv[1])
logf.write("\n\n")
logf.write("Channels Active:\n")
for i in range(2,len(sys.argv)):
    logf.write(sys.argv[i].rstrip('\n')+" ")
logf.write("\n\n")
logf.write("Time(s) ")
for i in range(2,len(sys.argv)):
    logf.write("CH"+sys.argv[i]+"(V) ".rstrip('\n'))

print(" ")
print("Beginning data acquisition...")

tstart=time.perf_counter()

while True:
    t1=time.perf_counter()
    for i in range(2,len(sys.argv)):
        print("Channel "+sys.argv[i]+"(V): {0:.3f}".format(DAQ.getVoltage(USB,int(sys.argv[i]))))
    print(" ")

    # WRITE TO FILE HERE - currently just prints data to screen in real time

    t2=time.perf_counter()
    dt1=(t2-t1)
    dt2=(t2-tstart)
    if dt1<sampTime:
        time.sleep(sampTime-dt1)
