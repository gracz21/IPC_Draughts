#ifndef LISTA_H
#define LISTA_H

#include "struktury.h"

//dodatkowo uzyte struktury niezawarte w protokole

struct gracz_extend { //rozszerzona struktura gracza ulatwiajaca identyfikacje w przypadku wyjscia
	struct gracz gracz;
	int status; //0 - zarejestrowany, 1 - oczekuje na przeciwnika, 2 - gra, 3 - obserwuje
	int id_gry; //id gry, do ktorej przydzielony jest gracz
};

/* element listy graczy (w programie wystepuja 3 rodzaje list graczy: 
 * lista graczy zarejestrowanych, lista graczy oczekujacych na przeciwnika oraz lista graczy obserwatorow fanej rozgrywki
 * list obserwatorow jest kilka (kazda dotyczy danego pojedynku) i sa przechowywane w rozszerzonej strukturze pojedynku)
*/

typedef struct el_gracz { 
	struct gracz_extend gracz_info; 
	struct el_gracz *next;
} lista_graczy;

struct pojedynek_extend { //rozszerzona structura pojedynku ulatwiajaca przechowywanie obserwatorow danej rozgrywki oraz statusu gry
	struct pojedynek pojedynek;
	struct status_gry stan; //stan pojeynku
	lista_graczy *first_obserwator; //glowa listy obserwatorow danego pojedynku
	lista_graczy *last_obserwator; //ogon listy obserwatorow
};

typedef struct el_pojedynek { // element listy pojedynkow (w odroznieniu od listy graczy lista pojedynkow jest tylko jedna)
	struct pojedynek_extend pojedynek_info;
	struct el_pojedynek *next;
} lista_pojedynkow;

//procedury i funkcje operacji na listach

void add_lista_graczy(struct gracz_extend gracz, int arg, lista_graczy **first, lista_graczy **last); //dodaj eleent do listy graczy (arg informuje o rodzaju listy: 1 - lista graczy zarejstrowanych, 2 - lista graczy obserwyjacych, 3- lista obserwatorow (wraz z podaniem wskaznika na poczatek i ogon danej listy))
void add_lista_pojedynkow(struct pojedynek_extend poj); //dodaj element do listy pojedynkow
void del_lista_graczy(int pid, int arg, lista_graczy **first, lista_graczy **last); //usun element z listy graczy (arg pelni ta sama role co w add_lista_graczy)
void del_lista_pojedynkow(int id); //usun element z listy pojedynkow
int count_lista_pojedynkow(void); //policz, jak dluga jest lista pojedynkow (przydatne przy przesylaniu tablicy gier obserwatorowi)
void conv_list_to_tab(struct pojedynek tab[]); //przepisz pojedynki z listy do tablicy (znowy przydatne przy przesylaniu tablicy pojdeynkow obserwaotorwi)
lista_graczy* find_gracz(int arg, int pid, lista_graczy *first); //znajdz gracza (arg pelni juz wczesniej opisana role)
lista_pojedynkow* find_pojedynek(int id); //znajdz pojedynek
void clean_lista_graczy(int arg, lista_graczy **first); //wyczysc calkowicie liste graczy (arg jak wyzej)
void clean_lista_pojedynkow(void); //wyczysc calkowicie liste pojedynkow

//deklaracja zmiennych globalnych glow i ogonow wykorzystywanych list

lista_graczy *first_gracz;
lista_graczy *last_gracz;
lista_graczy *first_wait;
lista_graczy *last_wait;
lista_pojedynkow *first_poj;
lista_pojedynkow *last_poj;

#endif
