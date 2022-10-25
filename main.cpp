#include <iostream>
#include "proc_args.hpp"
#include "proc_url.hpp"
#include "proc_conn.hpp"


#include <libxml/parser.h>
#include <libxml/tree.h>

int main(int argc, char** argv)
{
    Feedreader feedreader(argc, argv);

    UrlList urlList(feedreader);

    Connection connection(urlList, feedreader);

    std::cout << *connection.getXml(0);

    // xmlDocPtr doc = xmlParseDoc((const xmlChar *) xmlDoc.c_str());

    //     if(!doc)
    //     {
    //         std::cerr << "XML document from " << *urlList.getRecord(i)->getFullUrl() << " wasn't parsed. Next...\n";
    //         /* Free globals */
    //         xmlCleanupParser();
    //         continue;
    //     }

    //     xmlNodePtr cur = xmlDocGetRootElement(doc); 

    //     if(!cur)
    //     {
    //         std::cout << "Can't get root element from " << *urlList.getRecord(i)->getFullUrl() << " XML doc. Probably invalid formatting. Next...\n";
    //         xmlFreeDoc(doc);
    //         xmlCleanupParser();
    //         continue;
    //     }

    //     bool isAtom = false;

    //     if (cur->name == (const xmlChar*)"feed") isAtom = true;
    //     else if (cur->name == (const xmlChar*)"rss") isAtom = false;
}
