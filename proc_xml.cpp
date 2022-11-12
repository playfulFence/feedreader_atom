/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_xml.cpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "proc_xml.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>


/* Set NULL didn't help, so need to use this non-smart solution, but at least it works... */
void xmlGenericReplacer(void *ctx, const char *msg, ...)
{
	(void) ctx;
	(void) msg;
}

void xmlStructuredReplacer(void *userData, xmlErrorPtr error)
{
	(void) userData;
	(void) error;
}

Feed::Feed(Connection connection, Arguments arguments, UrlList urlList)
{

    /* Turn debuging functions off to avoid extra outputs */
    xmlSetGenericErrorFunc(NULL, xmlGenericReplacer);
    xmlSetStructuredErrorFunc(NULL, xmlStructuredReplacer);

    xmlDocPtr doc; 
    
    /* Actually not a smartest solution, but I did it when the whole code structure
        was ready, so yeah, sorry :S */
    bool firstXml = true;
    for(int i = 0; i < connection.xmls.size(); i++)
    {
        /* initialize xml document */
        doc = xmlParseDoc((const xmlChar *) (*connection.getXml(i)).c_str());

        if(!doc)
        {
            std::cerr << "XML document from " << *urlList.getRecord(i)->getFullUrl() << " wasn't parsed. Next...\n";
            /* Free globals */
            xmlCleanupParser();
            continue;
        }

        xmlNodePtr root = xmlDocGetRootElement(doc); 

        if(!root)
        {
            std::cout << "Can't get root element from " << *urlList.getRecord(i)->getFullUrl() << " XML doc. Probably invalid formatting. Next...\n";
            xmlFreeDoc(doc);
            xmlCleanupParser();
            continue;
        }

        /* It's Atom feed */
        if (!xmlStrcmp(root->name, (const xmlChar*)"feed")) 
        {
            xmlNodePtr rootChild = root->children;
            char* allocated = NULL;
            /* First of all, find the common title to write it out in triple "*" sign */
            while (rootChild)
            {   
                if(!xmlStrcmp(rootChild->name, (const xmlChar*)"title"))
                {   
                    /* title = (char*) cur->content not working! */
                    allocated = (char*)xmlNodeGetContent(rootChild);
                    break;
                }
                rootChild = rootChild->next;
            }
            if(!allocated)
            {
                std::cerr << "Can't get feed title. Next...\n";
                xmlFreeDoc(doc);
                xmlCleanupParser();
                continue;
            }
            /* Correct formatting stuff */
            if(firstXml) firstXml = false;
            else std::cout << std::endl;
            std::cout << "*** " << allocated << " ***" << std::endl;
            free(allocated);

            /* Get back to root childrens to search "entries" */
            rootChild = root->children;
            bool firstRun = true;
            while (rootChild)
            {
                if(!xmlStrcmp(rootChild->name, (const xmlChar*)"entry"))
                {   
                    if(firstRun) firstRun = false;
                    /* To be correctly formatted and separated */
                    else if(arguments.writeAuthor() || arguments.writeTime() || arguments.writeUrl())
                        std::cout << std::endl;

                    xmlNodePtr entryChild = rootChild->children;

                    /* Actually not a smartest solution, but I did it when the whole code structure
                        was ready, so yeah, sorry :S */
                    bool updateViewed = false;
                    while (entryChild)
                    {
                        /* Find entries titles (actual news, topics titles (if I understood it correctly)) */
                        if (!xmlStrcmp(entryChild->name, (const xmlChar*)"title"))
                        {   
                            allocated = (char*)xmlNodeGetContent(entryChild);
                            std::cout << allocated << std::endl;
                            free(allocated);
                        }
                        /* If author printing is required */                       
                        if (arguments.writeAuthor())
                        {
                            if(!xmlStrcmp(entryChild->name, (const xmlChar*)"author"))
                            {
                                /* Need to go deeper into "author" node, since in Atom name and 
                                    email are separated to two more nodes */
                                xmlNodePtr authorChild = entryChild->children;
                                bool nameIsSet = false;
                                while (authorChild)
                                {
                                    if(!xmlStrcmp(authorChild->name, (const xmlChar*)"name"))
                                    {
                                        nameIsSet = true;
                                        allocated = (char*)xmlNodeGetContent(authorChild);
                                        std::cout << "Autor: " << allocated << std::endl;
                                        free(allocated);
                                    }
                                    if(!xmlStrcmp(authorChild->name, (const xmlChar*)"email") && !nameIsSet)
                                    {
                                        allocated = (char*)xmlNodeGetContent(authorChild);
                                        std::cout << "Autor: " << allocated << std::endl;
                                        free(allocated);
                                    }
                                    authorChild = authorChild->next;
                                }
                                
                            }
                        }
                        /* Similar with time */
                        if (arguments.writeTime())
                        {
                            if(!xmlStrcmp(entryChild->name, (const xmlChar*)"updated"))
                            {
                                updateViewed = true;
                                allocated = (char*)xmlNodeGetContent(entryChild);
                                std::cout << "Aktualizace: " << allocated << std::endl;
                                free(allocated);
                            }
                            if (!xmlStrcmp(entryChild->name, (const xmlChar*)"published") && !updateViewed)
                            {
                                allocated = (char*)xmlNodeGetContent(entryChild);
                                std::cout << "Aktualizace: " << allocated << std::endl;
                                free(allocated);
                            }
                        }
                        /* And finally corresponding URL */
                        if (arguments.writeUrl())
                        {
                            if(!xmlStrcmp(entryChild->name, (const xmlChar*)"link"))
                                std::cout << "URL: " << xmlGetProp(entryChild, (const xmlChar*)"href") << std::endl;
                        }

                        entryChild = entryChild->next;
                    }
                    
                }
                rootChild = rootChild->next;
            }
        }


        /* It's RSS2.0 feed*/
        else if (!xmlStrcmp(root->name, (const xmlChar*)"rss"))
        {
            xmlNodePtr rootChild = root->children;
            char* allocated;
            /* Almost same here, but to find common title, we need to deepen into "channel" node*/
            while(rootChild) 
            {   
                if(!xmlStrcmp(rootChild->name, (const xmlChar*)"channel"))
                {   
                    xmlNodePtr channelChild = rootChild->children;
                    
                    while(channelChild)
                    {
                        /* Here it is! */
                        if(!xmlStrcmp(channelChild->name, (const xmlChar*)"title"))
                        {   
                            /* title = (char*) cur->content not working! */
                            allocated = (char*)xmlNodeGetContent(channelChild);
                            break;
                        }
                        channelChild = channelChild->next;
                    }

                    if(!allocated)
                    {
                        std::cerr << "Can't get feed title. Next...\n";
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                        continue;
                    }
                    /* Correct formatting stuff */
                    if(firstXml) firstXml = false;
                    else std::cout << std::endl;

                    std::cout << "*** " << allocated << " ***" << std::endl;
                    free(allocated);
                }
                rootChild = rootChild->next;
            }
            rootChild = root->children;
            bool firstRun = true;
            /* Dive into the XML tree again*/
            while (rootChild)
            {
                if(!xmlStrcmp(rootChild->name, (const xmlChar*)"channel"))
                {   
                    xmlNodePtr channelChild = rootChild->children;
                    while (channelChild)
                    {
                        /* Items contain all required data for each seperate topic */
                        if(!xmlStrcmp(channelChild->name, (const xmlChar*)"item"))
                        {
                            xmlNodePtr itemChild = channelChild->children;

                            if(firstRun) firstRun = false;
                            /* To be correctly formatted and separated */
                            else if(arguments.writeAuthor() || arguments.writeTime() || arguments.writeUrl())
                                std::cout << std::endl;

                            /* So much similar to Atom parsing in this part, so no need to explain here, I guess */
                            while(itemChild)
                            {
                                if(!xmlStrcmp(itemChild->name, (const xmlChar*)"title"))
                                {
                                    allocated = (char*)xmlNodeGetContent(itemChild);
                                    std::cout << allocated << std::endl;
                                    free(allocated);
                                }                    
                                if (arguments.writeAuthor())
                                {
                                    if(!xmlStrcmp(itemChild->name, (const xmlChar*)"author"))
                                    {
                                        allocated = (char*)xmlNodeGetContent(itemChild);
                                        std::cout << "Autor: " << allocated << std::endl;                 
                                        free(allocated);
                                    }
                                }
                                if (arguments.writeTime())
                                {
                                    if(!xmlStrcmp(itemChild->name, (const xmlChar*)"pubDate"))
                                    {
                                        allocated = (char*)xmlNodeGetContent(itemChild);
                                        std::cout << "Aktualizace: " << allocated << std::endl;                 
                                        free(allocated);
                                    }
                                }

                                if (arguments.writeUrl())
                                {
                                    if(!xmlStrcmp(itemChild->name, (const xmlChar*)"link"))
                                    {
                                        allocated = (char*)xmlNodeGetContent(itemChild);
                                        std::cout << "URL: " << allocated << std::endl;                 
                                        free(allocated);
                                    }
                                }

                                itemChild = itemChild->next;
                            }
                        }

                        channelChild = channelChild->next;
                    }
                }
                rootChild = rootChild->next;
            }
        }
        else 
        {
            std::cout << "Unsupported feedtype for " << *urlList.getRecord(i)->getFullUrl() << std::endl;
            continue;
        }

        /* Clean XML stuff */    
        xmlFreeDoc(doc);
        xmlCleanupParser();
    }
    
}

void clean(Connection connection, Arguments arguments, UrlList urlList)
{
    if(arguments.url) delete arguments.url;
    if(arguments.feed) delete arguments.feed;
    if(arguments.certFile) delete arguments.certFile;
    if(arguments.certPath) delete arguments.certPath;

    for(int i = 0; i < urlList.urls.size(); i++)
    {
        if(urlList.getRecord(i)->fullUrl) delete urlList.getRecord(i)->fullUrl;
        if(urlList.getRecord(i)->scheme) delete urlList.getRecord(i)->scheme;
        if(urlList.getRecord(i)->authority) delete urlList.getRecord(i)->authority;
        if(urlList.getRecord(i)->port) delete urlList.getRecord(i)->port;
        if(urlList.getRecord(i)->path) delete urlList.getRecord(i)->path;
        delete urlList.getRecord(i);
    }

    for(int i = 0; i < connection.xmls.size(); i++) delete connection.getXml(i);
}
