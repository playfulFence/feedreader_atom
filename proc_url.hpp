#ifndef __PROC_URL__H__
#define __PROC_URL__H__

#include <vector>
#include <fstream>
#include "proc_args.hpp"
class Url {
    public: 
        Url(Feedreader);
        std::vector<std::string> urls;
};


#endif //__PROC_URL__H__
