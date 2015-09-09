CC = g++
//LIBS = /home/pi/Libraries
INCLUDES = -I ../..

all:
	$(CC) $(INCLUDES) DLP_DAQ_analog.cpp \
	$ DLP_IO8_G.cpp -o DAQ

clean:
	rm DAQ
