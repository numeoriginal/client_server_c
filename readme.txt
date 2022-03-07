Tema3 Portocale de comunicatie.

Am preluat din laboratorul 10 buffer.c,buffer.h,helpers,c,helpers.h,request.c si requests.h.

Referitor la implementarea temei. Am implementat tema cu mi s-a parut mie potrivit si cum am vazut eu
ca ar lucra un server real. Am incercat sa tratez erorile intalnite ca atare si cat mai sugestiv pentru
un user obisnuit care poate nu stie prea bine mediul de lucru.Cu hint-uri pe alocuri.
In acest scop. mi-am permis sa adaug o comanda de info. care le-ar explica utilizatorilor
ce face o comanda in parte..
De asemenea am pus la dispozitia lor constant lista de comenzi pe care le pot folosi, in fuctie de state-ul lor.


functiile implementate de mine :
void register_foo();
void login_foo();
void enter_library_foo();
void add_book_foo();
void delete_get_book_foo(char *token);
void get_books_foo();
void logout_foo();

functiile reprezinta tratarea fiecarei comenzi in parte cu crearea de string si trimiterea folosind
functiile GET si POST, get-ul modificat un pic ca sa mearga si pentru un request de Delete.

in principiu logica la toate functiile este verificari daca suntem logati sau in biblioteca,
citirea de la tastarura a imputului userului , crearea stringului trimiterea unul mesaj serverului
tratarea mesajului primit si tratarea erorilor.
pentru ca comanda de delete si get book au fost atat de asemanatoare le-am combinat intr-o singura functie
pentru a evita spaghetti code si cod repetitiv.
am folosit get_basic_json din lab si nu am mai folosit o biblioteca dedicata pentru JSON.
NU am un motiv anume pentru asta..utilizarea stringurilor a fost destul de usoara si familiara
desi poate utilizarea unor biblioteci de JSON ar fi fost mai estetic si mai la tema.



