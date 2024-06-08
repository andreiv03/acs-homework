In cadrul implementarii clientului HTTP, am utilizat clase din C++ pentru a crea o structura modulara si clara. Clasa
principala, definita in client.h, gestioneaza logica principala a aplicatiei, inclusiv initializarea si ciclul de viata
al acesteia. Aceasta se ocupa de primirea comenzilor de la utilizator si de apelarea acestora.

Fiecare comanda specifica, cum ar fi add_book, delete_book, enter_library, get_book, get_books, login, logout sau
register, este implementata intr-un fisier separat. Fiecare modul de comanda mosteneste clasa abstracta Command si
implementeaza metoda virtuala execute(). Acest design permite adaugarea de noi comenzi fara a modifica structura
existenta, asigurand astfel extensibilitatea codului.

Clasa definita in request.h este responsabila de crearea si configurarea cererilor HTTP. Utilitarele, definite in
utils.h, contin functii esentiale pentru operarea cu socket-uri, cum ar fi open_connection, care deschide o conexiune
socket cu serverul, send_to_server, care trimite date catre server prin intermediul socket-ului deschis, si
receive_from_server, care primeste date de la server. Aceste functii au fost inspirate din laboratorul 9. In plus, acest modul include alte functii ajutatoare pentru
manipularea string-urilor, asigurand astfel o functionare fluida a clientului HTTP.

Libraria utilizata pentru a parsa JSON este nlohmann, care este inclusa in aplicatie prin #include <nlohmann/json.hpp>. Aceasta trebuie instalata in prealabil pentru a putea compila si rula aplicatia. Se poate folosi
comanda "sudo apt install nlohmann-json3-dev" pentru a instala libraria.
