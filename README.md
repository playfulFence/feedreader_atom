# ISA - Čtečka novinek ve formatu Atom a RSS

## Autor : Kirill Mikhailov (xmikha00)
## Datum: 14.11.2022

### Seznam odevzdaných souborů :
    - manual.pdf
    - test_feed.txt
    - test_expects/expected_help_output.txt
    - test_expects/expected_test_both_sources.stderr
    - test_expects/expected_test_more_than_one_a.stderr
    - test_expects/expected_test_more_than_one_c.stderr
    - test_expects/expected_test_more_than_one_certaddr.stderr
    - test_expects/expected_test_more_than_one_T.stderr
    - test_expects/expected_test_more_than_one_u.stderr
    - test_expects/expected_test_more_than_one_unknown_input.stderr
    - test.sh
    - feedreader.cpp
    - makefile
    - proc_args.cpp
    - proc_argc.hpp
    - proc_conn.cpp
    - proc_conn.hpp
    - proc_url.cpp
    - proc_url.hpp
    - proc_xml.cpp
    - proc_xml.hpp
    - README.md

## Popis

Tento program zobrazuje požadovana data ze stážených feedů (titul, jmeno autora, čas aktualizace) podle zadaných argumentů
<br>
Pro tento projekt jsem vybral  jazyk C++, protože on má spoustu užitečných knihoven a je objektově orientovaný, což mi přišlo jako dost dobrý důvod, proč si vybrat tento jazyk. Také práce s řetězci je mnohem jednodušší a flexibilnější než například v Cčku.
<br>
Projekt je realizován pomocí knihoven [OpenSSL](https://www.openssl.org/) and [libxml2](http://xmlsoft.org/html/book1.html), pro dálší informace víz soubor `manual.pdf`

    --------------------------------------- SYNTAXE SPUŠTĚNÍ ------------------------------------
    
    Usage : ./feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]
    Kde:
        • Povinně je uveden buď URL požadovaného zdroje (podporovaná schémata jsou http a https) nebo parametr -f, ktery za kterým pak nasleduje umístění souboru feedfile.

        • Volitelný parametr -c definuje umístění souboru certfile s certifikáty pro ověření platnosti certifikátu SSL/TLS předloženého serverem.
        • Volitelný parametr -C definuje umístění složky certaddr, kde se budou vyhledávat certifikáty pro ověření platnosti certifikátu SSL/TLS předloženého serverem.
        • Pozn.: Pokud není uveden -c ani -C, použije se úložiště certifikátů získané funkcí SSL_CTX_set_default_verify_paths().

        • Při spuštění s parametrem -T se pro každý záznam zobrazí informace o čase změny či vytvoření záznamu

        • Při spuštění s parametrem -a se pro každý záznam zobrazí jméno autora či jeho e-mailova adresa

        • Při spuštění s parametrem -u se pro každý záznam zobrazí asociované URL

    
    Note : if -c nor -C flag wasn't set, feedreader will use certificates from path which is returned by SSL_CTX_set_default_verify_paths()function
