#include "funkcje_s.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void init() {
	koniec_kol = msgget(12345, 0644 | IPC_CREAT);
	rej_kol = msgget(123456, 0644 | IPC_CREAT);
	gram_kol = msgget(1234567, 0644 | IPC_CREAT);
	rozm_gry_kol = msgget(12345678, 0644 | IPC_CREAT);
	obserwuje_kol = msgget(123456789, 0664 | IPC_CREAT);
	ruch_kol = msgget(1234567890, 0644 | IPC_CREAT);
	first_gracz = NULL;
	last_gracz = NULL;
	first_wait = NULL;
	last_wait = NULL;
	first_poj = NULL;
	last_poj = NULL;
	printf("\033[2J");
	printf("\033[0;0f");
}

void rejestracja(struct gracz gracz) {
	struct gracz_extend gracz_e;
	struct rej_odp potwierdzenie;
	gracz_e.gracz = gracz;
	gracz_e.status = 0;
	add_lista_graczy(gracz_e, 1, NULL, NULL);
	potwierdzenie.type = gracz.pid;
	msgsnd(rej_kol, &potwierdzenie, 0, 0);
	printf("Dodano gracza:\nlogin %s, kolor %d, stopien zaawansowania %d, PID %d.\n", 
				 last_gracz->gracz_info.gracz.login, last_gracz->gracz_info.gracz.kolor, last_gracz->gracz_info.gracz.stopien_zaawansowania, last_gracz->gracz_info.gracz.pid); 
}

void gram(struct gracz gracz) {
	lista_graczy *tmp = first_wait;
	int znaleziono = 0;
	while(tmp != NULL) {
		if(tmp->gracz_info.gracz.kolor != gracz.kolor && tmp->gracz_info.gracz.stopien_zaawansowania == gracz.stopien_zaawansowania) {
			znaleziono = 1;
			struct pojedynek_odp odp1, odp2;
			struct pojedynek_extend poj;
			if(tmp->gracz_info.gracz.kolor == 1) {
				poj.pojedynek.gracz1 = tmp->gracz_info.gracz;
				poj.pojedynek.gracz2 = gracz;
			} else {
				poj.pojedynek.gracz1 = gracz;
				poj.pojedynek.gracz2 = tmp->gracz_info.gracz;
			}
			if(first_poj != NULL)
				poj.pojedynek.id = first_poj->pojedynek_info.pojedynek.id + 1;
			else
				poj.pojedynek.id = 1;
			poj.first_obserwator = NULL;
			poj.last_obserwator = NULL;
			poj.stan.pionki1 = 12;
			poj.stan.pionki2 = 12;
			int i, j;
			for(i = 0; i < 8; i++)
				for(j = 0; j < 8; j++)
					if((i+j)%2 == 0 && i <3)
						poj.stan.plansza[i][j] = 'O';
					else if((i+j)%2 == 0 && i > 4)
						poj.stan.plansza[i][j] = 'X';
					else
						poj.stan.plansza[i][j] = ' ';
			add_lista_pojedynkow(poj);
			del_lista_graczy(tmp->gracz_info.gracz.pid, 2, NULL, NULL);
			odp1.type = poj.pojedynek.gracz1.pid;
			odp1.status = 0;
			odp1.id_gry = odp2.id_gry = poj.pojedynek.id;
			odp2.type = poj.pojedynek.gracz2.pid;
			odp2.status = 0;
			msgsnd(gram_kol, &odp1, 2*sizeof(int), 0);
			msgsnd(gram_kol, &odp2, 2*sizeof(int), 0);
			poj.stan.type = poj.pojedynek.gracz1.pid;
			msgsnd(ruch_kol, &(poj.stan), 64*sizeof(char)+2*sizeof(int), 0);
			poj.stan.type = poj.pojedynek.gracz2.pid;
			msgsnd(ruch_kol, &(poj.stan), 64*sizeof(char)+2*sizeof(int), 0);
			poj.stan.type = 0;
			tmp = find_gracz(1, poj.pojedynek.gracz1.pid, NULL);
			tmp->gracz_info.status = 2;
			tmp->gracz_info.id_gry = poj.pojedynek.id;
			tmp = find_gracz(1, poj.pojedynek.gracz2.pid, NULL);
			tmp->gracz_info.status = 2;
			tmp->gracz_info.id_gry = poj.pojedynek.id;
			printf("Stworzono pojedynek:\ngracz_1: %s, gracz_2: %s, id_gry: %d.\n", 
						 last_poj->pojedynek_info.pojedynek.gracz1.login, last_poj->pojedynek_info.pojedynek.gracz2.login, last_poj->pojedynek_info.pojedynek.id);
			break;
		}
		tmp = tmp->next;
	}
	if(znaleziono == 0) {
		tmp = find_gracz(1, gracz.pid, NULL);
		tmp->gracz_info.status = 1;
		struct gracz_extend gracz_e = tmp->gracz_info;
		add_lista_graczy(gracz_e, 2, NULL, NULL);
	}
}

void obserwuje(struct gracz gracz) {
	struct lista_gier_rozmiar rozm;
	rozm.type = gracz.pid;
	rozm.rozmiar = count_lista_pojedynkow();
	msgsnd(rozm_gry_kol, &rozm, sizeof(int), 0);
	struct lista_gier {
		long type;
		struct pojedynek lista_g[rozm.rozmiar];
	} gry;
	gry.type = gracz.pid;
	conv_list_to_tab(gry.lista_g);
	msgsnd(rozm_gry_kol, &gry, rozm.rozmiar*sizeof(struct pojedynek), 0);
}

void wybor(struct obserwacja_wybor wybor_obs_gry) {
	lista_pojedynkow *wybrana_gra = find_pojedynek(wybor_obs_gry.id);
	lista_graczy *tmp = find_gracz(1, wybor_obs_gry.obserwator.pid, NULL);
	tmp->gracz_info.status = 3;
	tmp->gracz_info.id_gry = wybor_obs_gry.id;
	struct gracz_extend obserwator = tmp->gracz_info;
	add_lista_graczy(obserwator, 3, &(wybrana_gra->pojedynek_info.first_obserwator), &(wybrana_gra->pojedynek_info.last_obserwator));
	printf("Dodano obserwatora do gry nr. %d:\nlogin %s, PID %d.\n", 
				 wybor_obs_gry.id, wybrana_gra->pojedynek_info.last_obserwator->gracz_info.gracz.login, wybrana_gra->pojedynek_info.last_obserwator->gracz_info.gracz.pid);
	struct status_gry stat;
	stat = wybrana_gra->pojedynek_info.stan;
	stat.type = wybor_obs_gry.obserwator.pid;
	msgsnd(obserwuje_kol, &stat, 64*sizeof(char)+2*sizeof(int), 0);
}

void warcaby(struct ruch ruch) {
	lista_pojedynkow *tmp = find_pojedynek(ruch.id_gry);
	int i, kod_ruchu[4];
	for(i = 0; i < 4; i++) {
		if(i%2 == 0)
			kod_ruchu[i] = toupper(ruch.ruch[i]) - 'A';
		else
			kod_ruchu[i] = toupper(ruch.ruch[i]) - '1';
	}
	char tmp_move;
	tmp_move = tmp->pojedynek_info.stan.plansza[kod_ruchu[1]][kod_ruchu[0]];
	tmp->pojedynek_info.stan.plansza[kod_ruchu[1]][kod_ruchu[0]] = ' ';
	if(kod_ruchu[0] < kod_ruchu[2])
		kod_ruchu[0]++;
	else
		kod_ruchu[0]--;
	if(kod_ruchu[1] < kod_ruchu[3])
		kod_ruchu[1]++;
	else
		kod_ruchu[1]--;
	while(kod_ruchu[0] != kod_ruchu[2] || kod_ruchu[1] != kod_ruchu[3]) {
		if(tmp->pojedynek_info.stan.plansza[kod_ruchu[1]][kod_ruchu[0]] == 'O') {
			if(tmp->pojedynek_info.pojedynek.gracz1.kolor == 1)
				tmp->pojedynek_info.stan.pionki1--;
			else
				tmp->pojedynek_info.stan.pionki2--;
		}
		if(tmp->pojedynek_info.stan.plansza[kod_ruchu[1]][kod_ruchu[0]] == 'X') {
			if(tmp->pojedynek_info.pojedynek.gracz1.kolor == 2)
				tmp->pojedynek_info.stan.pionki1--;
			else
				tmp->pojedynek_info.stan.pionki2--;
		}
		tmp->pojedynek_info.stan.plansza[kod_ruchu[1]][kod_ruchu[0]] = ' ';
		if(kod_ruchu[0] < kod_ruchu[2])
			kod_ruchu[0]++;
		else
			kod_ruchu[0]--;
		if(kod_ruchu[1] < kod_ruchu[3])
			kod_ruchu[1]++;
		else
			kod_ruchu[1]--;
	}
	tmp->pojedynek_info.stan.plansza[kod_ruchu[3]][kod_ruchu[2]] = tmp_move;
	tmp->pojedynek_info.stan.type = tmp->pojedynek_info.pojedynek.gracz1.pid;
	msgsnd(ruch_kol, &(tmp->pojedynek_info.stan), 64*sizeof(char)+2*sizeof(int), 0);
	tmp->pojedynek_info.stan.type = tmp->pojedynek_info.pojedynek.gracz2.pid;
	msgsnd(ruch_kol, &(tmp->pojedynek_info.stan), 64*sizeof(char)+2*sizeof(int), 0);
	lista_graczy *obserwator = tmp->pojedynek_info.first_obserwator;
	while(obserwator != NULL) {
		tmp->pojedynek_info.stan.type = obserwator->gracz_info.gracz.pid;
		msgsnd(obserwuje_kol, &(tmp->pojedynek_info.stan), 64*sizeof(char)+2*sizeof(int), 0);
		obserwator = tmp->pojedynek_info.first_obserwator->next;
	}
	tmp->pojedynek_info.stan.type = 0;
	if(tmp->pojedynek_info.stan.pionki1 == 0 || tmp->pojedynek_info.stan.pionki2 == 0) {
		clean_lista_graczy(3, &(tmp->pojedynek_info.first_obserwator));
		del_lista_pojedynkow(ruch.id_gry);
	}
}

void wyjscie_gracza(struct gracz gracz) {
	lista_graczy *tmp = find_gracz(1, gracz.pid, NULL);
	switch(tmp->gracz_info.status) {
		case 0: break;
		case 1:
			del_lista_graczy(gracz.pid, 2, NULL, NULL);
		break;
		case 2: {
			lista_pojedynkow *poj = find_pojedynek(tmp->gracz_info.id_gry);
			lista_graczy *tmp_2;
			if(gracz.kolor == 1) {
				poj->pojedynek_info.stan.pionki1 = 0;
				poj->pojedynek_info.stan.type = poj->pojedynek_info.pojedynek.gracz2.pid;
				tmp_2 = find_gracz(1, poj->pojedynek_info.pojedynek.gracz2.pid, NULL);
			} else {
				poj->pojedynek_info.stan.pionki2 = 0;
				poj->pojedynek_info.stan.type = poj->pojedynek_info.pojedynek.gracz1.pid;
				tmp_2 = find_gracz(1, poj->pojedynek_info.pojedynek.gracz1.pid, NULL);
			}
			tmp_2->gracz_info.id_gry = 0;
			tmp_2->gracz_info.status = 0;
			msgsnd(ruch_kol, &(poj->pojedynek_info.stan), 64*sizeof(char)+2*sizeof(int), 0);
			lista_graczy *obserwator = poj->pojedynek_info.first_obserwator;
			while(obserwator != NULL) {
				poj->pojedynek_info.stan.type = obserwator->gracz_info.gracz.pid;
				msgsnd(obserwuje_kol, &(poj->pojedynek_info.stan), 64*sizeof(char)+2*sizeof(int), 0);
				obserwator = poj->pojedynek_info.first_obserwator->next;
			}
			del_lista_pojedynkow(tmp->gracz_info.id_gry);
		} break;
		case 3: {
			lista_pojedynkow *poj = find_pojedynek(tmp->gracz_info.id_gry);
			del_lista_graczy(gracz.pid, 3, &(poj->pojedynek_info.first_obserwator), &(poj->pojedynek_info.last_obserwator));
		} break;
	}
	del_lista_graczy(gracz.pid, 1, NULL, NULL);
}

void zamykanie(int signo) {
	clean_lista_graczy(1, NULL);
	clean_lista_graczy(2, NULL);
	lista_pojedynkow *tmp = first_poj;
	while(tmp != NULL) {
		clean_lista_graczy(3, &(tmp->pojedynek_info.first_obserwator));
		tmp = tmp->next;
	}
	clean_lista_pojedynkow();
	msgctl(koniec_kol, IPC_RMID, NULL);
	msgctl(rej_kol, IPC_RMID, NULL);
	msgctl(gram_kol, IPC_RMID, NULL);
	msgctl(rozm_gry_kol, IPC_RMID, NULL);
	msgctl(obserwuje_kol, IPC_RMID, NULL);
	msgctl(ruch_kol, IPC_RMID, NULL);
	exit(0);
}