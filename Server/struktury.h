#ifndef STRUKTURY_H
#define STRUKTURY_H

//struktury ujete w protokole i wykorzystywane zarowno w kliencie jak i w serwerze

struct gracz {
	char login[20];
	int kolor;
	int stopien_zaawansowania;
	int pid;
};

struct zapytanie {
	long type;
	struct gracz gracz_info;
};

struct rej_odp {
	long type;
};

struct pojedynek_odp {
	long type;
	int status;
	int id_gry;
};

struct pojedynek {
	struct gracz gracz1;
	struct gracz gracz2;
	int id;
};

struct lista_gier_rozmiar {
	long type;
	int rozmiar;
};

struct obserwacja_wybor {
	long type;
	struct gracz obserwator;
	int id;
};

struct status_gry {
	long type;
	char plansza[8][8];
	int pionki1;
	int pionki2;
};

struct ruch {
	long type;
	int id_gry;
	char ruch[4];
};

#endif
