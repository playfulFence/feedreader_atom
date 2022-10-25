CC = g++
CPPFLAGS = -std=c++17 -g -Wall -O2 -L/opt/homebrew/opt/openssl@1.1/lib -I/opt/homebrew/opt/openssl@1.1/include -L/opt/homebrew/opt/libxml2/lib -I/opt/homebrew/opt/libxml2/include -fsanitize=address
all: feedreader

feedreader : main.o proc_args.o proc_url.o proc_conn.o
			$(CC) $(CPPFLAGS) -lcrypto -lssl -lxml2 `xml2-config --cflags` `xml2-config --libs` main.o proc_args.o proc_url.o proc_conn.o -o feedreader
main.o : main.cpp
			$(CC) $(CPPFLAGS) -c main.cpp
proc_conn.o : proc_conn.cpp proc_conn.hpp proc_url.hpp
			$(CC) $(CPPFLAGS) -lcrypto -lssl -lxml2 `xml2-config --cflags` `xml2-config --libs` -c proc_conn.cpp
proc_url.o : proc_url.cpp proc_url.hpp proc_args.hpp
			$(CC) $(CPPFLAGS) -c proc_url.cpp
proc_args.o : proc_args.cpp proc_args.hpp
			$(CC) $(CPPFLAGS) -c proc_args.cpp
clean:
			rm *.o feedreader