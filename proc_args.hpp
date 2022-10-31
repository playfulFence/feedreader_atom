/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_args.hpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __PROC_ARGS__H__
#define __PROC_ARGS__H__

#include <iostream>
#include <getopt.h>
#include <string>
#include <regex>

class Arguments{
    public:
        Arguments(int, char**);
        std::string* url;
        std::string* feed;
        std::string* certFile;
        std::string* certPath; 
        bool timeViewFlag;
        bool authorViewFlag;
        bool urlViewFlag;
        
        /* Getters */
        std::string* getUrl();
        std::string* getFeed();
        std::string* getCertFile();
        std::string* getCertPath();
        bool writeTime();
        bool writeAuthor();
        bool writeUrl();                                                                            
};

void printHelp();


#endif // __PROC_ARGS__H__
