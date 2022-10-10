#include <iostream>
#include "proc_args.hpp"
#include <fstream>
#include <openssl/ssl.h>


int main(int argc, char** argv)
{
    Feedreader feedreader(argc, argv);

    if (feedreader.getUrl() && !feedreader.getCertAddr())
    {
        
    }

    return 0;
}
