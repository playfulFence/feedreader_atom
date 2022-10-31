/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_xml.cpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "proc_xml.hpp"

#include <libxml/parser.h>
#include <libxml/tree.h>

Feed::Feed(Connection connection, Arguments arguments, UrlList urlList)
{
    xmlDocPtr doc;
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
            std::string title;
            /* First of all, find the common title to write it out in triple "*" sign */
            while (rootChild)
            {   
                if(!xmlStrcmp(rootChild->name, (const xmlChar*)"title"))
                {   
                    /* title = (char*) cur->content not working! */
                    title = (char*)xmlNodeGetContent(rootChild);
                    break;
                }
                rootChild = rootChild->next;
            }
            if(title.empty())
            {
                std::cerr << "Can't get feed title. Next...\n";
                xmlFreeDoc(doc);
                xmlCleanupParser();
                continue;
            }
            std::cout << "*** " << title << " ***" << std::endl;

            /* Get back to root childrens to search "entries" */
            rootChild = root->children;
            while (rootChild)
            {
                if(!xmlStrcmp(rootChild->name, (const xmlChar*)"entry"))
                {   
                    xmlNodePtr entryChild = rootChild->children;
                    while (entryChild)
                    {
                        /* Find entries titles (actual news, topics titles (if I understood it correctly)) */
                        if (!xmlStrcmp(entryChild->name, (const xmlChar*)"title"))
                            std::cout << xmlNodeGetContent(entryChild) << std::endl;

                        /* If author printing is required */                       
                        if (arguments.writeAuthor())
                        {
                            if(!xmlStrcmp(entryChild->name, (const xmlChar*)"author"))
                            {
                                /* Need to go deeper into "author" node, since in Atom name and 
                                    email are separated to two more nodes */
                                xmlNodePtr authorChild = entryChild->children;
                                while (authorChild)
                                {
                                    if(!xmlStrcmp(authorChild->name, (const xmlChar*)"name"))
                                        std::cout << "Author's name : " << xmlNodeGetContent(authorChild) << std::endl;
                                    if(!xmlStrcmp(authorChild->name, (const xmlChar*)"email"))
                                        std::cout << "Author's email: " << xmlNodeGetContent(authorChild) << std::endl;

                                    authorChild = authorChild->next;
                                }
                                
                            }
                        }
                        /* Similar with time */
                        if (arguments.writeTime())
                        {
                            if(!xmlStrcmp(entryChild->name, (const xmlChar*)"updated"))
                                std::cout << "Last updated: " << xmlNodeGetContent(entryChild) << std::endl;
                            
                            if (!xmlStrcmp(entryChild->name, (const xmlChar*)"published"))
                                std::cout << "Published: " << xmlNodeGetContent(entryChild) << std::endl;
                        }
                        /* And finally corresponding URL */
                        if (arguments.writeUrl())
                        {
                            if(!xmlStrcmp(entryChild->name, (const xmlChar*)"link"))
                                std::cout << "URL: " << xmlGetProp(entryChild, (const xmlChar*)"href") << std::endl;
                        }

                        entryChild = entryChild->next;
                    }
                    
                    /* To be correctly formatted and separated */
                    std::cout << std::endl;
                }
                rootChild = rootChild->next;
            }
        }


        /* It's RSS2.0 feed*/
        else if (!xmlStrcmp(root->name, (const xmlChar*)"rss"))
        {
            xmlNodePtr rootChild = root->children;

            /* Almost same here, but to find common title, we need to deepen into "channel" node*/
            while(rootChild) 
            {   
                if(!xmlStrcmp(rootChild->name, (const xmlChar*)"channel"))
                {   
                    xmlNodePtr channelChild = rootChild->children;
                    std::string title;
                    while(channelChild)
                    {
                        /* Here it is! */
                        if(!xmlStrcmp(channelChild->name, (const xmlChar*)"title"))
                        {   
                            /* title = (char*) cur->content not working! */
                            title = (char*)xmlNodeGetContent(channelChild);
                            break;
                        }
                        channelChild = channelChild->next;
                    }

                    if(title.empty())
                    {
                        std::cerr << "Can't get feed title. Next...\n";
                        xmlFreeDoc(doc);
                        xmlCleanupParser();
                        continue;
                    }
                    std::cout << "*** " << title << " ***" << std::endl;
                }
                rootChild = rootChild->next;
            }
            rootChild = root->children;
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

                            /* So much similar to Atom parsing in this part, so no need to explain here, I guess */
                            while(itemChild)
                            {
                                if(!xmlStrcmp(itemChild->name, (const xmlChar*)"title"))
                                    std::cout << xmlNodeGetContent(itemChild) << std::endl;      


                                if (arguments.writeAuthor())
                                {
                                    if(!xmlStrcmp(itemChild->name, (const xmlChar*)"author"))
                                    {
                                        std::cout << "Author: " << xmlNodeGetContent(itemChild) << std::endl;                   
                                    }
                                }
                                if (arguments.writeTime())
                                {
                                    if(!xmlStrcmp(itemChild->name, (const xmlChar*)"pubDate"))
                                        std::cout << "Published: " << xmlNodeGetContent(itemChild) << std::endl;                                
                                }

                                if (arguments.writeUrl())
                                {
                                    if(!xmlStrcmp(itemChild->name, (const xmlChar*)"link"))
                                        std::cout << "URL: " << xmlNodeGetContent(itemChild) << std::endl;
                                }

                                itemChild = itemChild->next;
                            }
                            /* To be correctly formatted and separated */
                            std::cout << std::endl;
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
        
    }
    
    /* Clean XML stuff */    
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

