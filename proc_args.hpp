#ifndef __PROC_ARGS__H__
#define __PROC_ARGS__H__

#include <iostream>
#include <getopt.h>
#include <string>
#include <regex>

class Feedreader{
    public:
        Feedreader(int, char**);
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
// Feedreader* collectInput(int, char**);


#endif // __PROC_ARGS__H__
