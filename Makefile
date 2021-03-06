#Declaring the Variables
CC=g++

#Declaring the flags
CFLAGS =-c -std=c++11 

all		:forwardSim

forwardSim :simulation.o Race.o Terran.o Protoss.o
		$(CC) simulation.o Race.o Terran.o Protoss.o -o forwardSim

simulation.o 	:simulation.cpp
		$(CC) $(CFLAGS) simulation.cpp

Race.o		: Race.cpp
		$(CC) $(CFLAGS) Race.cpp

Terran.o	:Terran.cpp
		$(CC) $(CFLAGS) Terran.cpp

Protoss.o	:Protoss.cpp
		$(CC) $(CFLAGS) Protoss.cpp

clean		:
		rm -rf *o forwardSim

