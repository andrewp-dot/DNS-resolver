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

# Spustenie

# Zoznam súborov