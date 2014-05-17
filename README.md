BeGadu ?.? (??.??.2007)
=======================

Maciej Witkowiak, <ytm@elysium.pl>, <mwitkowiak@gmail.com>
http://members.elysium.pl/ytm/html/beos.html
http://ytm.bossstation.dnsalias.org/html/beos.html

BeGadu to klient GG oparty na tym, co udało się odnaleźć i odtworzyć z projektu BeGadu dostępnego na
Sourceforge: http://begadu.sourceforge.net

Obecna wersja jest skompilowana dla BeOS R5 z netserver.
Użyta libgadu pochodzi z listopada 2006r (~rc1.7) i została skonfigurowana jak dla systemów *BSD.



#LICENCJA
Zarówno libgadu, jak i źródła z których korzystałem są na licencji GPLv2, więc i ta wersja musi być
rozwijana na tych warunkach.



#START
Po prostu uruchom program, podaj dane dla utworzenia profilu. BeGadu połączy się z serwerem i ściągnie listę kontaktów z konta.



#NIE DZIAŁA / NIE MA
- przełączanie profili (po prostu tego nie rób)
- pełne preferencje (brak kilku plików źródłowych z wersji, która to potrafiła) 
- dodawanie/usuwanie kontaktów
- zapis listy kontaktów na serwer / do pliku
- import listy kontaktów z pliku
- nie ma logowania rozmów



#KOMPILACJA
W katalogu lib/ wykonaj make.
Resztę projektu zbuduj za pomocą BeIDE.



#LIBGADU & NETSERVER, ZETA, BONE
Prawdopodobnie bez większych zmian można zbudować program z BONE lub stosem TCP/IP z Haiku.

Zmiany w libgadu polegały na dodaniu/usunięciu nagłówków systemowych i zamianie kodu w styl BSD:
open -> opensocket
read -> recv
write -> send
close -> closesocket

libgadu nie korzysta z SSL (ale można to wkompilować), nie używa pthreads.

Program działa tylko w trybie synchronicznego połączenia, to prawdopodobnie wina netservera. Pod BeOSem nie ma to większego znaczenia, ponieważ system wymusza wielowątkowość.



#LOKALIZACJA
Usunąłem kod lokalizacji programu w stylu Zety - to ślepa uliczka i złe rozwiązanie. SpLocale również
jest złym rozwiązaniem. Prawdopodobnie użyję czegoś w rodzaju GNU gettext.
