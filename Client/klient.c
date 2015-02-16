/* Autor: Kamil Walkowiak
 * Grupa dziekanska I3
 * Indeks 109714
 * 
 * KLIENT
*/

#include "funkcje_k.h"

int main(int argc, char* argv[]) {
	
	koniec_kol = msgget(12345, 0); //otwarcie kolejek (stworzonych przez serwer)
	rej_kol = msgget(123456, 0);
	gram_kol = msgget(1234567, 0);
	rozm_gry_kol = msgget(12345678, 0);
	obs_kol = msgget(123456789, 0);
	ruch_kol = msgget(1234567890, 0);
	int wybor, id_gry, zakoncz = 0; //zmienne pomocnicze
	
	gracz = init(); //stworzenie profilu nowego gracza
	signal(SIGINT, wyjscie); //zmiana obslugi sygnalu SIGINT w celu wysylania komunitaku do serwera o wyjsciu z klienta
	rejestracja(); //rejestracja gracza na serwerze
	
	while(zakoncz == 0) { //menu klienta
		printf("\033[2J");
		printf("\033[0;0f");
		printf("Wybierz numer jednej z opcji:\n1 - chce grac\n2 - chce obserwowac juz trwajacy pojedynek\n3 - chce wyjsc\n");
		scanf("%d", &wybor);
		switch (wybor) {
			case 1: { //chce grac z innym uzytkownikiem
				id_gry = gram();
				warcaby(id_gry);
			} break;
			case 2: { //chce byc obserwatorem
				if(obserwuje() != -1)
					obserwacja();
			} break;
			case 3: //chce zakonczyc prace z klientem
				zakoncz = 1;
			break;
		}
	}
	koniec(); //wyslanie sygnalu do serwera o wyjsciu z gry
	
	return 0;
}
