/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_url.hpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __PROC_URL__H__
#define __PROC_URL__H__

#include <vector>
#include <fstream>
#include <regex>
#include <stdlib.h>
#include "proc_args.hpp"

class UrlDesc
{
    public:
        UrlDesc(std::string);
        std::string* fullUrl;
        std::string* scheme;
        std::string* authority;
        std::string* port;
        std::string* path;
        std::string* getFullUrl();
        std::string* getScheme();
        std::string* getAuthority();
        std::string* getPort();
        std::string* getPath(); 
};

class UrlList {
    public: 
        UrlList(Arguments);
        std::vector<UrlDesc*> urls;
        UrlDesc* getRecord(size_t);
};


#endif //__PROC_URL__H__
