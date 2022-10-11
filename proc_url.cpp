#include "proc_url.hpp"

Url::Url(Feedreader feedreader)
{
    if (!feedreader.getUrl() && feedreader.getFeed())
    { 
        std::ifstream feedFile(*feedreader.feed);
        if (!feedFile.is_open())
        {
            std::cerr << "Unable to open feedfile, bad path : " << feedreader.feed << std::endl;
            exit(2);
        }

        std::string tmp;
        while(getline(feedFile, tmp))
        {
            if(tmp[0] == '#' || tmp.empty()) continue;
            else urls.push_back(tmp);
        }

        feedFile.close();
    }
}