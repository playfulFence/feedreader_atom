/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : main.cpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <iostream>
#include "proc_args.hpp"
#include "proc_url.hpp" 
#include "proc_conn.hpp"
#include "proc_xml.hpp"

int main(int argc, char** argv)
{
    /* Parse arguments */
    Arguments arguments(argc, argv);
    /* Create list of structured data about URLs */
    UrlList urlList(arguments);
    /* Set connection to all URLs from the list above with all cert. checks (if required) and send request for XML */
    Connection connection(urlList, arguments);
    /* Parse XMLs and print out formatted */
    Feed feed(connection, arguments, urlList);

    clean(connection, arguments, urlList);

    return 0;
}
