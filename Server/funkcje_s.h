#ifndef FUNKCJE_S_H
#define FUNKCJE_S_H

#include "lista.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//funkcje serwera

void init(); //inicjalizacja kolejek i zmiennych listowych
void rejestracja(struct gracz gracz); //rejestracja gracza
void gram(struct gracz gracz); //dobranie przeciwnika dla gracza, lub dodanie do listy oczekujacych na gre
void obserwuje(struct gracz gracz); //przyjecie zgloszenia checi obserwacji gry i wyslanie tablicy dostepnych gier
void wybor(struct obserwacja_wybor wybor_obs_gry); //obsluga wygoru obserwowanej gry
void warcaby(struct ruch ruch); //obsluga wlasciwej gry
void wyjscie_gracza(struct gracz gracz); //reakcja na wyjscie gracza (zakonczenie pracy klienta)
void zamykanie(int signo); //czyszczenie kolejek i zasobow serwera

//deklaracja globalnych deskryptorow kolejek

int koniec_kol;
int rej_kol;
int gram_kol;
int rozm_gry_kol;
int obserwuje_kol;
int ruch_kol;

#endif
