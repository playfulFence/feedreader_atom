/* * * * * * * * * * * * * * * * * * * * * * * * * * 

Project : Feedreader of Atom and RSS feeds with TLS support

File : proc_args.cpp

Author : Mikhailov Kirill (xmikha00)
* * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "proc_args.hpp"

void printHelp()
{
    std::cout << "Usage : ./feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]\nWhere :\n";
    std::cout << "\t -URL is a required source or a feedfile(path to it after a -f flag) which is a text file with contains\n";
    std::cout << "\t\t a list of source adresses (one on each row) YOU CAN USE ONLY URL OR -f FLAG IN ONE EXECUTION!\n";
    std::cout << "\t - Optional flag -c with a following file with certificates which are used to validate server's SSL/TLS certificate\n";
    std::cout << "\t - Optional flag -C with a following path to a folder, where the certificates for server's SSL/TLS certificate validation will be searched\n";
    std::cout << "\t - With optional flag -T feedreader will show you time of last change and creation(if downloaded file includes) of each record\n";
    std::cout << "\t - With optional flag -a feedreader will show you the author's name and e-mail address(if downloaded file includes) of each record\n";
    std::cout << "\t - With optional flag -u feedreader will show you corresponding URL (if downloaded file includes) of each record\n";
    std::cout << "\n\n Note : if -c nor -C flag wasn't set, feedreader will use certificates from path which is returned by SSL_CTX_set_default_verify_paths() function\n";
    exit(1);
}


/* Class builder */
Arguments::Arguments(int argc, char** argv)
{   
    /* No arguments - print help */
    if(argc == 1) printHelp();

    authorViewFlag = false; 
    timeViewFlag = false;
    urlViewFlag = false;
    url = nullptr;
    feed = nullptr;
    certPath = nullptr;
    certFile = nullptr;

    /* for check if user set only one URL (if without -f flag) */
    bool catched = false;

    for(int i = 1; i < argc; i++)
    {  
        if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            printHelp();
            exit(0);
        }
        
        if(std::regex_match(argv[i], std::regex("((http|https)://)(www.)?[-a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)")))
        {
            if(catched == true)
            {
                std::cerr << "Error! You can pass only one URL. If you want to see feed of more pages, add them to feedfile and use -f flag.\n";
                exit(1);            
            }
            catched = true;
            url = new std::string(argv[i]);

            /* if URL was found - just "cutting" it off from the arguments line */
            for(int x = i; x < argc; x++)
            {
                if (argv[x+1]) argv[x] = argv[x+1];
                else {
                    argc--;
                    argv[x] = nullptr;
                }
            }
        }
    }
        
    while (1)
    {   
        /* catching Tua flags WITHOUT flags and f, c, C flags WITH flags */
        switch (getopt(argc, argv, "f:c:C:Tua"))
        {
            case 'T':
                if (timeViewFlag)
                {
                    std::cerr << "You can set -T flag only once!\n";
                    exit(1);
                }
                timeViewFlag = true;
                continue;
            case 'a':
                if (authorViewFlag)
                {
                    std::cerr << "You can set -a flag only once!\n";
                    exit(1);
                }
                authorViewFlag = true;
                continue;
            case 'u':
                if (urlViewFlag)
                {
                    std::cerr << "You can set -u flag only once!\n";
                    exit(1);
                } 
                urlViewFlag = true;
                continue;
            case 'f':
                /* if URL or feed was already set, user can't set another one */
                if (url || feed)
                {
                    std::cerr << "You can set only URL or feedfile via -f flag.\n";
                    printHelp();
                }
                feed = new std::string(optarg);
                continue;
            case 'c':
                if (certFile)
                {
                    std::cerr << "You can set certfile (-c flag) only once.\n";
                    exit(1);
                } else certFile = new std::string(optarg);
                continue;
            case 'C':
                if (certPath)
                {
                    std::cerr << "You can set certificate directory(-C flag) only once.\n";
                    exit(1);
                } 
                certPath = new std::string(optarg);
                continue;
            case -1:
                break; // end of arguments line
            default:
                printHelp();
                break;
        }
        break;
    }
    if (!url && !feed)
    {
        std::cerr << "ERROR! URL or feedfile(-f <feedfile>) must be set!\n";
        printHelp();
    }
}

/* GETTERS */
std::string* Arguments::getUrl()
{
    return url;
}

std::string* Arguments::getFeed()
{
    return feed;
}

std::string* Arguments::getCertFile()
{
    return certFile;
}

std::string* Arguments::getCertPath()
{
    return certPath;
}

bool Arguments::writeTime()
{
    return timeViewFlag;
}

bool Arguments::writeAuthor()
{
    return authorViewFlag;
}

bool Arguments::writeUrl()
{
    return urlViewFlag;
}
