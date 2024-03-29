**Meno:** Adrián Ponechal <br>
**Login:** xponec01 <br>
**Dátum:** 18.11.2023

# Popis programu
Program dns je klient slúžiaci na získanie dotazovaných DNS záznamov. Program rezolúciu nevykonáva, ale pošle dotaz na zadaný server a príjma odpoveď. 
Po prijatí úspešnej odpovede ju zobrazí na štandartnom výstup, inak zobrazí chybové hlásenie.

Formát výstupu:

<pre>
Authoritative: No, Recursive: Yes, Truncated: No
Question section (1) 
  www.fit.vut.cz., A, IN 
Answer section (1) 
  www.fit.vut.cz., A, IN, 14400, 147.229.9.26 
Authority section (0)
Additional section (0)
</pre>

# Rozšírenia a obmedzenia
Klient podporuje dotazovanie sa na záznamy typu A, AAAA a PTR. V odpovedi zobrazuje záznamy typu A, AAAAA, PTR, SOA, NS. V prípade skrátenej odpovedi nastaví hodnotu truncated na "Yes", vypíše otázku a sekcie 'Answer', 'Authority', 'Additional' vypíše ako prázdne. 

V prípade dotazovania sa na server, ktorý neexistuje alebo nepošle odpoveď, je v programe implementovaný timeout. Po skončení timeoutu program na štandartný výstup vypíše chybové hlásenie.

## Návratová hodnota
V prípade úspechu program vráti kód s hodnotou 0, inak vráti kód chyby.

# Spustenie
<code>./dns [-r] [-x] [-6] -s server [-p port] address</code>

Popis: 
- -r nastaví príznak pre rekurzívny dotaz
- -x nastaví príznak pre reverzný dotaz (program očakáva dotazovanú adresu vo formáte IPv4)
- -6 nastaví príznak pre dotazovanie sa na adresy IPv6 (v kombinácii s prepínačom -x program očakáva dotazovanú adresu vo formáte IPv6)
- -s povinný prepínač; za ním nasleduje adresa serveru
- -p nepovinný prepínač; za ním nasleduje port serveru, na ktorý sa bude posielať dotaz (v prípade nezadaného portu alebo zadania portu v zlom formáte bude štandartne použitý port 53)
- address je adresa požadovaného servera

*Poznámka 1:* <br>
V prípade nezadania ani jedného z prepínačov sa vypíše použitie programu.

*Poznámka 2:* <br>
V prípade zadania viackrát prepínača -s, program použie poslednú zadanú neprázdnu hodnotu za prepínačom. V prípade zadania viacerých adries program použije poslednú zadanú v programe.

## Príklad spustenia
<code>./dns -s kazi.fit.vutbr.cz 147.229.8.12 -r -x</code>

<pre>
Authoritative: Yes, Recursive: Yes, Truncated: No
Question section (1)
  12.8.229.147.in-addr.arpa., PTR, IN
Answer section (1)
  12.8.229.147.in-addr.arpa., PTR, IN, 14400, kazi.fit.vutbr.cz.
Authority section (0)
Additional section (0)
</pre>


# Zoznam súborov

<pre>
include
|-- Conection.h
|-- constants.h
|-- Error.h
|-- InputParser.h
|-- Message.h
`-- Query.h
src
|-- components
|   |-- Connection.cpp
|   |-- Error.cpp
|   |-- InputParser.cpp
|   |-- Message.cpp
|   `-- Query.h
`-- dns.cpp
tests
|-- digParser.py
|-- dnsParser.py
`-- tests.py
LICENSE
Makefile
README.md
manual.pdf
</pre>
