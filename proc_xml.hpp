/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_xml.hpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __PROC_XML__H__
#define __PROC_XML__H__

#include "proc_conn.hpp"
#include "proc_args.hpp"

class Feed
{
    public:
        Feed(Connection, Arguments, UrlList);
};

#endif
