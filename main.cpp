#include <iostream>
#include "proc_args.hpp"
#include "proc_url.hpp"
#include <openssl/ssl.h>


int main(int argc, char** argv)
{
    Feedreader feedreader(argc, argv);

    Url url(feedreader);

    

    return 0;
}
