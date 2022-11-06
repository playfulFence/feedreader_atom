/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_url.cpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "proc_url.hpp"

UrlDesc::UrlDesc(std::string toPush)
{
    std::smatch sm;
    std::regex_match(toPush, sm, std::regex("^([^:/?#]+)://([^/?:#]*)?:?([0-9]+)?([^?#]*)(\\?([^#]*))?(#(.*))?"));
                                           /* 1            2            3        4       5   6        7*/

    fullUrl = new std::string(sm[0]);
    scheme = new std::string(sm[1]);
    authority = new std::string(sm[2]);
    port = (sm[3] != "") ? new std::string(sm[3]) : nullptr;
    path = new std::string(sm[4]);    

    /* If port isn't in URL, setting it up manually accorgind to used protocol*/
    if(port == nullptr) port = !strcmp(scheme->c_str(), "http") ? new std::string("80") : new std::string("443");
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

UrlList::UrlList(Arguments arguments)
{   
    /* If feedfile, but not specific URL was set*/
    if (!arguments.getUrl() && arguments.getFeed())
    { 
        std::ifstream feedFile(*arguments.feed);
        if (!feedFile.is_open())
        {
            std::cerr << "Unable to open feedfile, bad path : " << *arguments.feed << std::endl;
            exit(2);
        }

        std::string tmp;
        for (int i = 1; getline(feedFile, tmp); i++)
        {
            /* Skip comments (#-starting lines) and empty lines at feedfile */
            if(tmp[0] == '#' || tmp.empty()) continue;
            else if(std::regex_match(tmp, std::regex("((http|https)://)(www.)?[-a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)")))
                {
                    UrlDesc* newRecord = new UrlDesc(tmp);
                    urls.push_back(newRecord);       
                }
                
            else std::cerr << "WARNING: Link " << tmp << " (line " << i << " of feedfile) is wrong format! Ignoring...\n\n";
        }

        feedFile.close();
    }
    /* If only single URL was set */
    else 
    {
        UrlDesc* newRecord = new UrlDesc(*arguments.url);
        urls.push_back(newRecord); 
    }
}

/* get Url on a specific index */
UrlDesc* UrlList::getRecord(size_t index)
{
    return urls[index];
}