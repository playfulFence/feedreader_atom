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


Feedreader::Feedreader(int argc, char** argv)
{
    if(argc == 1) printHelp();

    this->authorViewFlag = false; 
    this->timeViewFlag = false;
    this->urlViewFlag = false;

    for(int i = 1; i < argc; i++)
    {  
        if(std::regex_match(argv[i], std::regex("http[s]?://[a-zA-Z_0-9]+\\.[a-z]+")))
        {
            this->url = std::string(argv[i]);
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
        switch (getopt(argc, argv, "f:c:C:Tua"))
        {
            case 'T':
                if (this->timeViewFlag)
                {
                    std::cerr << "You can set -T flag only once!\n";
                    exit(1);
                }
                this->timeViewFlag = true;
                continue;
            case 'a':
                if (this->authorViewFlag)
                {
                    std::cerr << "You can set -a flag only once!\n";
                    exit(1);
                }
                this->authorViewFlag = true;
                continue;
            case 'u':
                if (this->urlViewFlag)
                {
                    std::cerr << "You can set -u flag only once!\n";
                    exit(1);
                } 
                this->urlViewFlag = true;
                continue;
            case 'f':
                if (!this->url.empty() || !this->feed.empty())
                {
                    std::cerr << "You can set only URL or feedfile via -f flag.\nNOTE: \"" << optarg << "\" is a problematic place\n";
                    exit(1);
                }
                this->feed = std::string(optarg);
                continue;
            case 'c':
                if (!this->certFile.empty())
                {
                    std::cerr << "You can set certfile (-c flag) only once.\n";
                    exit(1);
                } else this->certFile = std::string(optarg);
                continue;
            case 'C':
                if (!this->certAddr.empty())
                {
                    std::cerr << "You can set certificate directory(-C flag) only once.\n";
                    exit(1);
                } 
                this->certAddr = std::string(optarg);
                continue;
            case -1:
                break; // end of arguments line
            default:
                printHelp();
                break;
        }
        break;
    }
    if (this->url.empty() && this->feed.empty())
    {
        std::cerr << "ERROR! URL or feedfile(-f <feedfile>) must be set!\n";
        exit(1);
    }
}
