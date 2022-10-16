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
            ctx = SSL_CTX_new(SSLv23_client_method());
            
            int ca_load_status = 0; 

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
            bio = BIO_new_ssl_connect(ctx);
        }
        /* Unsecured connection */
        else bio = BIO_new_connect(urlList.getRecord(i)->getAuthority()->c_str());

        if (!bio) 
        {
            std::cerr << "Can't create connection with " << *urlList.getRecord(i)->getFullUrl() << ". Ignoring...\n";
            continue;
        }

        if(*urlList.getRecord(i)->getScheme() == "https")
        {
            BIO_get_ssl(bio, &ssl);
            SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
            std::string hostPort = *urlList.getRecord(i)->getAuthority() + ":" + *urlList.getRecord(i)->getPort();
            BIO_set_conn_hostname(bio, hostPort.c_str());
        }

        if(BIO_do_connect(bio) <= 0)
        {
            std::cerr << "Connection with " << *urlList.getRecord(i)->getFullUrl() << " failed. Ignoring...\n";
            continue;
        }


    }


    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}
