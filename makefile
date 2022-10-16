CC = g++
CPPFLAGS = -std=c++17 -g -Wall -O2 -L/opt/homebrew/opt/openssl@1.1/lib -I/opt/homebrew/opt/openssl@1.1/include -fsanitize=address
all: feedreader

feedreader : main.o proc_args.o proc_url.o
			$(CC) $(CPPFLAGS) -lcrypto -lssl main.o proc_args.o proc_url.o -o feedreader
main.o : main.cpp
			$(CC) $(CPPFLAGS) -c main.cpp
proc_url.o : proc_url.cpp proc_url.hpp proc_args.hpp
			$(CC) $(CPPFLAGS) -c proc_url.cpp
proc_args.o : proc_args.cpp proc_args.hpp
			$(CC) $(CPPFLAGS) -c proc_args.cpp
clean:
			rm *.o feedreader