/* Autor: Kamil Walkowiak
 * Grupa dziekanska I3
 * Indeks 109714
 * 
 * SERWER
*/

#include "funkcje_s.h"
#include <unistd.h>
#include <signal.h>

int main(int argc, char* argv[]) {
	init(); //inicjalizacja kolejek i elementow list oraz czyszczenie ekranu
	
	struct zapytanie zapytanie; //deklaracja struktur uzywanych odbieranych od klienta
	struct obserwacja_wybor wybor_obs_gry;
	struct ruch ruch;
	
	signal(SIGINT, zamykanie); //zmiana obslugi sygnalu SIGINT w celu zwalniania zasobow serwera przy zamykaniu
	printf("Serwer gotowy!\n"); //informacja o gotowosci serwera
	
	while(1) { //wlasciwa czesc serwera
		if(msgrcv(rej_kol, &zapytanie, sizeof(struct gracz), 1, IPC_NOWAIT) != -1) //jezeli zostal odebrany komunikat z kolejki rejestracyjnej, to zarejestruj gracza
			rejestracja(zapytanie.gracz_info);
		if(msgrcv(gram_kol, &zapytanie, sizeof(struct gracz), 1, IPC_NOWAIT) != -1) //jezeli jakis gracz wyraza chec gry, to sprauj gracza i utwoz pojedynek lub dodaj go do listy oczekujacych na gre
			gram(zapytanie.gracz_info);
		if(msgrcv(koniec_kol, &zapytanie, sizeof(struct gracz), 1, IPC_NOWAIT) != -1) //jezeli jakis gracz wyszedl, usun go z odpowiednich list
			wyjscie_gracza(zapytanie.gracz_info);
		if(msgrcv(ruch_kol, &ruch, 4*sizeof(char)+sizeof(int), 1, IPC_NOWAIT) != -1) //jezeli zostal odebrany komunikat z kolejki ruhcow, to zrealizuj go
			warcaby(ruch);
		if(msgrcv(rozm_gry_kol, &zapytanie, sizeof(struct gracz), 1, IPC_NOWAIT) != -1) //jezeli jakis gracz
			obserwuje(zapytanie.gracz_info);
		if(msgrcv(obserwuje_kol, &wybor_obs_gry, sizeof(struct gracz)+sizeof(int), 1, IPC_NOWAIT) != -1)
			wybor(wybor_obs_gry);
		usleep(20000);
	};
	
	return 0;
}
