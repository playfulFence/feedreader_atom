#ifndef __PROC_CONN__H__
#define __PROC_CONN__H__

#include "proc_url.hpp"
#include "proc_args.hpp"

class Connection
{
    public:
        Connection(UrlList, Feedreader);
        std::vector<std::string*> xmls;
        std::string* getXml(size_t);
};



#endif
