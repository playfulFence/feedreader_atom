#include <iostream>
#include "proc_args.hpp"
#include "proc_url.hpp"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

int main(int argc, char** argv)
{
    Feedreader feedreader(argc, argv);

    UrlList urlList(feedreader);

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
        /* Secured connection */
        if(*urlList.getRecord(i)->getScheme() == "https")
        {
            /* Create new SSL certificates (CTX) object */
            ctx = SSL_CTX_new(SSLv23_client_method());
            
            /* For status storing */
            int ca_load_status = 0; 

            /* Setting up certificates accoring to application flags (if no -c nor -C flag was set, use default path) */
            if (feedreader.getCertPath() && feedreader.getCertFile()) 
                ca_load_status = SSL_CTX_load_verify_locations(ctx, feedreader.getCertFile()->c_str(), feedreader.getCertPath()->c_str());

            else if (!feedreader.getCertPath() && feedreader.getCertFile()) 
                ca_load_status = SSL_CTX_load_verify_locations(ctx, feedreader.getCertFile()->c_str(), NULL);

            else if (feedreader.getCertPath() && !feedreader.getCertFile()) 
                ca_load_status = SSL_CTX_load_verify_locations(ctx, NULL, feedreader.getCertPath()->c_str());

            else if (!feedreader.getCertPath() && !feedreader.getCertFile())
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
        else bio = BIO_new_connect(urlList.getRecord(i)->getAuthority()->c_str());

        /* Check if BIO object has been created somehow */
        if (!bio) 
        {
            std::cerr << "Can't create connection with " << *urlList.getRecord(i)->getFullUrl() << ". Ignoring...\n";
            continue;
        }

        /* Create an authority of a correct format for following connecting attempt and HTTP request */
        std::string hostPort = *urlList.getRecord(i)->getAuthority() + ":" + *urlList.getRecord(i)->getPort();

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

        std::string httpRequest("GET " + *urlList.getRecord(i)->getPath() + " HTTP/1.1\r\n"
                              "Host: " + hostPort + "\r\n"
                              "Connection: close\n\n" // close by default since there's no need in "keep-alive" for us 
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

        char buf[8192] = {'\0'};
        bool readDone = false;
        std::string responce;
        bool firstRead = true;
        int read = 0;
        
        do {
            read = BIO_read(bio, buf, 8191);
            if (read == 0 && !firstRead) break;
            else if (read < 0)
            {
                firstRead = false;
                if(BIO_should_retry(bio)) continue;
                else break;
            }
            else
            {
                readDone = true;
                buf[read] = '\0';
                responce += buf;
            }
        } while (!readDone);


        if (!readDone && !read)
        {
            std::cerr << "Connection to " << *urlList.getRecord(i)->getFullUrl() << " closed. Next...\n";
            continue;
        }
        else if(!readDone)
        {
            std::cerr << "Receiving response from " << *urlList.getRecord(i)->getFullUrl() << " failed. Ignoring...\n";
            continue;
        }

        std::cout << *urlList.getRecord(i)->getFullUrl() << " response :\n";
        std::cout << buf << "\n\n\n";

    }


    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}
