/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_conn.cpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "proc_conn.hpp"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

Connection::Connection(UrlList urlList, Arguments arguments)
{
    /*Initialize OpenSSL*/
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    /* Initialize BIO to handle communication */    
    BIO* bio = nullptr;
    SSL_CTX* ctx = nullptr; 
    SSL* ssl = nullptr;

   

    for(int i = 0; i < urlList.urls.size(); i++)
    {

        /* Create an authority of a correct format for following connecting attempt and HTTP request */
        std::string hostPort = *urlList.getRecord(i)->getAuthority() + ":" + *urlList.getRecord(i)->getPort();

        /* Secured connection */
        if(*urlList.getRecord(i)->getScheme() == "https")
        {
            /* Create new SSL certificates (CTX) object */
            ctx = SSL_CTX_new(SSLv23_client_method());
            
            /* For status storing */
            int ca_load_status = 0; 

            /* Setting up certificates according to application flags (if no -c nor -C flag was set, use default path) */
            if (arguments.getCertPath() && arguments.getCertFile()) 
                ca_load_status = SSL_CTX_load_verify_locations(ctx, arguments.getCertFile()->c_str(), arguments.getCertPath()->c_str());

            else if (!arguments.getCertPath() && arguments.getCertFile()) 
                ca_load_status = SSL_CTX_load_verify_locations(ctx, arguments.getCertFile()->c_str(), NULL);

            else if (arguments.getCertPath() && !arguments.getCertFile()) 
                ca_load_status = SSL_CTX_load_verify_locations(ctx, NULL, arguments.getCertPath()->c_str());

            else if (!arguments.getCertPath() && !arguments.getCertFile())
                ca_load_status = SSL_CTX_set_default_verify_paths(ctx);

            if(!ca_load_status)
            { 
                std::cerr << "Can't get certificates for " << *urlList.getRecord(i)->getFullUrl() << ". Ignoring...\n";
                continue;
            }

            /* Create BIO object based on found certificates */
            bio = BIO_new_ssl_connect(ctx);
        }
        /* Unsecured connection */
        else bio = BIO_new_connect(hostPort.c_str());

        /* Check if BIO object has been created somehow */
        if (!bio) 
        {
            std::cerr << "Can't create connection with " << *urlList.getRecord(i)->getFullUrl() << ". Ignoring...\n";
            continue;
        }

        

        /* For secured connection */
        if(*urlList.getRecord(i)->getScheme() == "https")
        {
            /* Setting up the BIO object */
            BIO_get_ssl(bio, &ssl);
            SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

            /* Attempt to connect via hostname and port */
            BIO_set_conn_hostname(bio, hostPort.c_str());
        }


        /* Check if connection opened and perform a handshake */
        if(BIO_do_connect(bio) <= 0)
        {
            std::cerr << "Connection with " << *urlList.getRecord(i)->getFullUrl() << " failed. Ignoring...\n";
            continue;
        }

        /* Check if certificate is valid*/
        if(SSL_get_verify_result(ssl) != X509_V_OK)
        {
            std::cerr << "Certificates verification failed for " << *urlList.getRecord(i)->getFullUrl() << ". Ignoring...\n";
            continue;
        }

        /* Building a HTTP request to get XML (at least as expected) */
        std::string httpRequest("GET " + *urlList.getRecord(i)->getPath() + " HTTP/1.0\r\n"
                              "Host: " + hostPort + "\r\n"
                              "\r\n");

        
        bool writeDone = false;
        /* Write HTTP request to connection (to required site in our case) */
        while(!writeDone)
        {
            /* returns count of writen bytes is succeded or < 0 values in 
                case of some kind of an error (no need to differ them in our case) */
            if(BIO_write(bio, httpRequest.c_str(), httpRequest.size()) <= 0)
            {
                /* returns true if operation should be retried */
                if(BIO_should_retry(bio)) continue;
                /* or false, if an error ocured */
                else break;
            }
            else writeDone = true;
        }      
        
        /* another dumb solution from me, yeah... welp, dat's my hobbie tho */
        if (!writeDone)
        {
            std::cerr << "Sending request to " << *urlList.getRecord(i)->getFullUrl() << " failed. Ignoring...\n\n";
            continue;
        }
        
        /* I will receive "portions" of 512 chars since.
           No smart reason, just not so big, not so small... */
        char buf[512];
        bool readDone = false;
        std::string response;
        int read = 0;
        
        while (!readDone) {
            
            /* returns amount of data read successfully, 0 when nothing to read, < 0 when error */
            read = BIO_read(bio, buf, 511);
            if (read == 0) readDone = true;
            else if (read < 0)
            {
                if(BIO_should_retry(bio)) continue;
                else break;
            }
            else if (read > 0) 
            {
                response += buf;
                /* refresh buffer */
                std::fill_n(buf, 512, '\0');
            }
        }

        /* read variable might be 0 after only one while loop and jump out of it, so
            i need to check, if it was 0 because there's nothing to read (=> response is empty) or beacause
            the response was read whole successfully. */
        if(!readDone || response.empty())
        {
            std::cerr << "Receiving response from " << *urlList.getRecord(i)->getFullUrl() << " failed. Ignoring...\n";
            continue;
        }

        /* Rudely find the xml file, since we're not interested in another content.
            If not found - ignore and process next link. */
        int xmlStartPos = response.find("<?xml");
        if (xmlStartPos == std::string::npos)
        {
            std::cerr << "Couldn't identificate XML-feed in " << *urlList.getRecord(i)->getFullUrl() << ". Next...\n";
            continue;        
        }
        /* push received and filtered XML to the vector of XMLs */
        xmls.push_back(new std::string(response.substr(xmlStartPos, response.length()))); 
    }


    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}

std::string* Connection::getXml(size_t index)
{
    return xmls[index];
}