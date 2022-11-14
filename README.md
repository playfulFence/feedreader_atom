# feedreader_atom

This is my school project for Network Management subject.
The main task was to implement feedreader for Atom and RSS 2.0 formats.
I decided to implement in C++ language since there's a lot of handy libraries and work with strings, regexes and arguments is much more convenient, neither in C language. The fact C++ is object oriented also made me to pick this language
<br>
I did it with help of [OpenSSL](https://www.openssl.org/) and [libxml2](http://xmlsoft.org/html/book1.html), more info in project documentation.

    --------------------------------------- EXECUTION SYNTAX ------------------------------------
    ```
    **Usage : ./feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]**
    Where:
        • URL is a required source or a feedfile(path to it after a -f flag) which is a text file with contains
            a list of source adresses (one on each row) YOU CAN USE ONLY URL OR -f FLAG IN ONE EXECUTION!
        • Optional flag -c with a following file with certificates which are used to validate server's SSL/TLS certificate
        • Optional flag -C with a following path to a folder, where the certificates for server's SSL/TLS certificate validation will be searched
        • With an optional flag -T feedreader will show you time of last change and creation(if downloaded file includes) of each record
        • With an optional flag -a feedreader will show you the author's name and e-mail address(if downloaded file includes) of each record
        • With optional flag -u feedreader will show you corresponding URL (if downloaded file includes) of each record\n";

    <br>

    <br>
    
    > Note : if -c nor -C flag wasn't set, feedreader will use certificates from path which is returned by SSL_CTX_set_default_verify_paths()function
    ```