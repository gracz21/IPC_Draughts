#ifndef FUNKCJE_K_H
#define FUNKCJE_K_H

#include "struktury.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

struct gracz init(void); //odczyt danych na temat nowego gracza i stworzenie na ich podstawie nowej jednostki struktury gracza
void rejestracja(void); //wyslanie danych o nowym graczu do serwera
int gram(void); //poinformowanie serwera o wyrazeniu checi gry z innym uzytkownikiem i pobieranie informacji o przydzieleniu do gry
int obserwuje(void); //poinformowanie serwera o wyrazeniu checi obserwowania juz rozpoczetej gry, wybor gry do obserwacji i pobranie aktualnego stanu gry
void warcaby(int id_gry); //wyslanie ruchu do serwera
void obserwacja(void); //odebranie od serwera aktualnego stanu gry i wypisanie go na ekran
void wyjscie(int signo); //procedura glowna przebiegu gry
void koniec(void); //wyslanie sygnalu o zakonczeniu pracy klienta do serwera

//deklaracja globalnych deskryptorow kolejek

int koniec_kol;
int rej_kol;
int gram_kol;
int rozm_gry_kol;
int obs_kol;
int ruch_kol;

//deklaracja gracza

struct gracz gracz;

#endif
