#include "proc_url.hpp"

UrlDesc::UrlDesc(std::string toPush)
{
    std::smatch sm;
    std::regex_match(toPush, sm, std::regex("^([^:/?#]+)://([^/?:#]*)?:?([0-9]+)?([^?#]*)(\\?([^#]*))?(#(.*))?"));
                                           /* 1            2            3        4       5   6        7*/
    
    fullUrl = new std::string(sm[0]);
    scheme = new std::string(sm[1]);
    authority = new std::string(sm[2]);
    port = new std::string(sm[3]);
    path = new std::string(sm[4]);    

    if(port->empty()) port = !strcmp(scheme->c_str(), "http") ? new std::string("80") : new std::string("443");
}

std::string* UrlDesc::getFullUrl()
{
    return fullUrl;
}

std::string* UrlDesc::getScheme()
{
    return scheme;
}

std::string* UrlDesc::getAuthority()
{
    return authority;
}

std::string* UrlDesc::getPort()
{
    return port;
}

std::string* UrlDesc::getPath()
{
    return path;
}

UrlList::UrlList(Feedreader feedreader)
{
    if (!feedreader.getUrl() && feedreader.getFeed())
    { 
        std::ifstream feedFile(*feedreader.feed);
        if (!feedFile.is_open())
        {
            std::cerr << "Unable to open feedfile, bad path : " << *feedreader.feed << std::endl;
            exit(2);
        }

        std::string tmp;
        for (int i = 1 ;getline(feedFile, tmp); i++)
        {
            if(tmp[0] == '#' || tmp.empty()) continue;
            else if(std::regex_match(tmp, std::regex("^([^:/?#]+)://([^/?#]*)?([^?#]*)(\\?([^#]*))?(#(.*))?")))
                {
                    UrlDesc* newRecord = new UrlDesc(tmp);
                    urls.push_back(newRecord);       
                }
                
            else std::cerr << "WARNING: Link " << tmp << " (line " << i << " of feedfile) is wrong format! Ignoring...\n\n";
        }

        feedFile.close();
    }
    else 
    {
        UrlDesc* newRecord = new UrlDesc(*feedreader.url);
        urls.push_back(newRecord); 
    }
}

UrlDesc* UrlList::getRecord(size_t index)
{
    return urls[index];
}