CC = g++
CFLAGS = -g -Wall -O2

all: feedreader

feedreader : main.o proc_args.o
			$(CC) $(CFLAGS) main.o proc_args.o -o feedreader
main.o : main.cpp
			$(CC) $(CFLAGS) -c main.cpp
proc_args.o : proc_args.cpp proc_args.hpp
			$(CC) $(CFLAGS) -c proc_args.cpp
clean:
			rm *.o feedreader