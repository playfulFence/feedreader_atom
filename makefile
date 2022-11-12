CC = g++ -static-libstdc++

XMLLDFLAGS!=pkg-config --libs libxml-2.0
XMLLDFLAGS?=$(shell pkg-config --libs libxml-2.0)
XMLCFLAGS!=pkg-config --cflags libxml-2.0
XMLCFLAGS?=$(shell pkg-config --cflags libxml-2.0)

CPPFLAGS = -std=c++17 -g -O2 $(XMLCFLAGS)
all: feedreader

feedreader : feedreader.o proc_args.o proc_url.o proc_conn.o proc_xml.o
			$(CC) $(CPPFLAGS) -lcrypto -lssl $(XMLLDFLAGS) feedreader.o proc_args.o proc_url.o proc_conn.o proc_xml.o -o feedreader
feedreader.o : feedreader.cpp
			$(CC) $(CPPFLAGS) -c feedreader.cpp
proc_xml.o : proc_xml.cpp proc_xml.hpp proc_conn.hpp
			$(CC) $(CPPFLAGS) $(XMLLDFLAGS) -c proc_xml.cpp						
proc_conn.o : proc_conn.cpp proc_conn.hpp proc_url.hpp proc_args.hpp
			$(CC) $(CPPFLAGS) -lcrypto -lssl -c proc_conn.cpp
proc_url.o : proc_url.cpp proc_url.hpp proc_args.hpp
			$(CC) $(CPPFLAGS) -c proc_url.cpp
proc_args.o : proc_args.cpp proc_args.hpp
			$(CC) $(CPPFLAGS) -c proc_args.cpp
clean:
			rm *.o feedreader
