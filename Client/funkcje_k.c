#include "funkcje_k.h"
#include <stdlib.h>
#include <unistd.h>

struct gracz init(void) {
	struct gracz gracz;
	gracz.pid = getpid();
	printf("\033[2J");
  printf("\033[0;0f");
	printf("Podaj login (max. 19 znakow):\n");
	scanf("%s", gracz.login);
	printf("Podaj preferowany kolor pionkow\n(1 - biale, 2 - czarne):\n");
	scanf("%d", &gracz.kolor);
	printf("Podaj poziom zaawansowania\n(1 - poczatkujacy, 2 - srednio-zaawansowany, 3 - Chuck Noris warcabow):\n");
	scanf("%d", &gracz.stopien_zaawansowania);
	printf("\033[2J");
	printf("\033[0;0f");
	return gracz;
}

void rejestracja(void) {
	struct zapytanie rej;
	struct rej_odp odp;
	rej.type = 1;
	rej.gracz_info = gracz;
	msgsnd(rej_kol, &rej, sizeof(struct gracz), 0);
	msgrcv(rej_kol, &odp, 0, getpid(), 0);
	printf("\033[2J");
  printf("\033[0;0f");
	printf("Rejstracja zakonczona pomyslnie!\n");
	sleep(1);
}

int gram(void) {
	struct zapytanie gram;
	struct pojedynek_odp odp;
	gram.type = 1;
	gram.gracz_info = gracz;
	msgsnd(gram_kol, &gram, sizeof(struct gracz), 0);
	msgrcv(gram_kol, &odp, 2*sizeof(int), getpid(), 0);
	printf("\033[2J");
	printf("\033[0;0f");
	if(odp.status == 0)
		printf("Znaleziono przeciwnika!\n");
	else
		printf("Wystapil blad\n");
	return odp.id_gry;
}

int obserwuje(void) {
	struct lista_gier_rozmiar rozm;
	struct zapytanie obserwuje;
	struct obserwacja_wybor obs;
	obserwuje.type = 1;
	obserwuje.gracz_info = gracz;
	msgsnd(rozm_gry_kol, &obserwuje, sizeof(struct gracz), 0);
	msgrcv(rozm_gry_kol, &rozm, sizeof(int), getpid(), 0);
	struct lista_gier {
		long type;
		struct pojedynek lista_gier[rozm.rozmiar];
	} gry;
	msgrcv(rozm_gry_kol, &gry, rozm.rozmiar*sizeof(struct pojedynek), getpid(), 0);
	if(rozm.rozmiar != 0) {
		printf("Wybierz numer obserwowanej gry:\n");
		int i;
		for(i = 0; i < rozm.rozmiar; i++)
			printf("%d. %s VS %s\n", gry.lista_gier[i].id, gry.lista_gier[i].gracz1.login, gry.lista_gier[i].gracz2.login);
		scanf("%d", &obs.id);
		obs.type = 1;
		obs.obserwator = gracz;
		msgsnd(obs_kol, &obs, sizeof(struct gracz)+sizeof(int), 0);
		printf("Gotowe!");
		return(0);
	} else
		printf("Aktualnie nie toczy sie zaden pojedynek, sprobuj ponownie za chwile.\nWcisnij ENTER, aby kontynuowac...");
	sleep(1);
	return(-1);
}

int ruch(int id_gry) {
	struct ruch ruch;
	struct status_gry stan_tmp;
	char tmp_ruch[6];
	ruch.type = 1;
	ruch.id_gry = id_gry;
	printf("\nTWOJ RUCH\n\n");
	scanf("%s", tmp_ruch);
	int i;
	for(i = 0; i < 4; i++)
		ruch.ruch[i] = tmp_ruch[i];
	if(msgrcv(ruch_kol, &stan_tmp, 64*sizeof(char)+2*sizeof(int), getpid(), IPC_NOWAIT) != -1) {
		printf("Przeciwnik poddal sie!\n");
		return -1;
	}
	msgsnd(ruch_kol, &ruch, 4*sizeof(char)+sizeof(int), 0);
	return 0;
}

struct status_gry update(int kol) {
	struct status_gry stan;
	msgrcv(kol, &stan, 64*sizeof(char)+2*sizeof(int), getpid(), 0);
	printf("\033[2J");
	printf("\033[0;0f");
	int i, k;
	printf("Legenda:\nO-pionki biale\nX-pionki czarne\n\n   A B C D E F G H\n   _______________\n");
	for(i = 0; i < 8; i++) {
		printf("%d |", i+1);
		for(k = 0; k < 8; k++)
			printf("%c|", stan.plansza[i][k]);
		printf("\n");
	}
		printf("   ⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺⎺\n");
	printf("Gracz_1 pionki: %d\nGracz_2 pionki: %d\n", stan.pionki1, stan.pionki2);
	return stan;
}

void warcaby(int id_gry) {
	struct status_gry stan;
	int err = 0;
	stan = update(ruch_kol);
	if(gracz.kolor == 1) {
		err = ruch(id_gry);
		if(err != -1)
			stan = update(ruch_kol);
	}
	while(stan.pionki1 != 0 && stan.pionki2 != 0 && err != -1) {
		stan = update(ruch_kol);
		if(stan.pionki1 != 0 && stan.pionki2 != 0)
			err = ruch(id_gry);
		else
			break;
		if(err != -1)
			stan = update(ruch_kol);
		else
			break;
	}
	if(gracz.kolor == 1)
		if(stan.pionki1 == 0)
			printf("\nPrzegrana!\n");
		else
			printf("\nZWYCIESTWO!!!\n");
	else
		if(stan.pionki2 == 0)
			printf("\nPrzegrana!\n");
		else
			printf("\nZWYCIESTWO!!!\n");
	sleep(2);
}

void obserwacja(void) {
	struct status_gry stan;
	stan = update(obs_kol);
	while(stan.pionki1 != 0 && stan.pionki2 != 0) {
		stan = update(obs_kol);
	}
	printf("\nGra zostala zakonczona!\n");
	sleep(2);
}

void wyjscie(int signo) {
	koniec();
	exit(0);
}

void koniec(void) {
	struct zapytanie zapytanie;
	zapytanie.type = 1;
	zapytanie.gracz_info = gracz;
	msgsnd(koniec_kol, &zapytanie, sizeof(struct gracz), 0);
	printf("\033[2J");
	printf("\033[0;0f");
}