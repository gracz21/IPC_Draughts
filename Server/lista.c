#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void add_lista_graczy(struct gracz_extend gracz, int arg, lista_graczy **first, lista_graczy **last) {
	lista_graczy **tmp_f; //zmienne zastosowane w celu uniwersalizacji procedury dla wszystkich rodzajow list graczy
	lista_graczy **tmp_l;
	switch(arg) {
		case 1: {
			tmp_f = &first_gracz;
			tmp_l = &last_gracz;
		} break;
		case 2: {
			tmp_f = &first_wait;
			tmp_l = &last_wait;
		} break;
		case 3: {
			tmp_f = first;
			tmp_l = last;
		} break;
	}
	lista_graczy *nowy = (lista_graczy*) malloc(sizeof(lista_graczy));
	nowy->gracz_info = gracz;
	nowy->next = NULL;
	if(*tmp_f == NULL)
		*tmp_f = *tmp_l = nowy;
	else {
		(*tmp_l)->next = nowy;
		*tmp_l = nowy;
	}
}

void add_lista_pojedynkow(struct pojedynek_extend poj) {
	lista_pojedynkow *nowy = (lista_pojedynkow*) malloc(sizeof(lista_pojedynkow));
	nowy->pojedynek_info = poj;
	nowy->next = NULL;
	if(first_poj == NULL)
		first_poj = last_poj = nowy;
	else {
		last_poj->next = nowy;
		last_poj = nowy;
	}
}

void del_lista_graczy(int pid, int arg, lista_graczy **first, lista_graczy **last) {
	lista_graczy **tmp_f;
	lista_graczy **tmp_l;
	switch(arg) {
		case 1: {
			tmp_f = &first_gracz;
			tmp_l = &last_gracz;
		} break;
		case 2: {
			tmp_f = &first_wait;
			tmp_l = &last_wait;
		} break;
		case 3: {
			tmp_f = first;
			tmp_l = last;
		} break;
	}
	lista_graczy *tmp = *tmp_f;
	lista_graczy *to_del = *tmp_f;
	while(to_del->gracz_info.gracz.pid != pid) {
		tmp = to_del;
		to_del = to_del->next;
	};
	if(to_del != *tmp_f)
		tmp->next = to_del->next;
	else
		*tmp_f = (*tmp_f)->next;
	if(to_del == *tmp_l)
		*tmp_l = tmp;
	free(to_del);
}

void del_lista_pojedynkow(int id) {
	lista_pojedynkow *tmp = first_poj;
	lista_pojedynkow *to_del = first_poj;
	while(to_del->pojedynek_info.pojedynek.id != id) {
		tmp = to_del;
		to_del = to_del->next;
	};
	if(to_del != first_poj)
		tmp->next = to_del->next;
	else
		first_poj = first_poj->next;
	if(to_del == last_poj)
		last_poj = tmp;
	free(to_del);
}

int count_lista_pojedynkow(void) {
	lista_pojedynkow *tmp = first_poj;
	int rozm = 0;
	while(tmp != NULL) {
		rozm++;
		tmp = tmp->next;
	}
	return rozm;
}

void conv_list_to_tab(struct pojedynek tab[]) {
	lista_pojedynkow *tmp = first_poj;
	int i = 0;
	while(tmp != NULL) {
		tab[i] = tmp->pojedynek_info.pojedynek;
		i++;
		tmp = tmp->next;
	}
}

lista_graczy* find_gracz(int arg, int pid, lista_graczy *first) {
		lista_graczy *tmp;
	switch(arg) {
		case 1:
			tmp = first_gracz;
		break;
		case 2:
			tmp = first_wait;
		break;
		case 3:
			tmp = first;
		break;
	}
	while(tmp->gracz_info.gracz.pid != pid)
		tmp = tmp ->next;
	return tmp;
}

lista_pojedynkow* find_pojedynek(int id) {
	lista_pojedynkow *tmp = first_poj;
	while(tmp->pojedynek_info.pojedynek.id != id)
		tmp = tmp ->next;
	return tmp;
}

void clean_lista_graczy(int arg, lista_graczy **first) {
	lista_graczy **tmp_f;
	switch(arg) {
		case 1:
			tmp_f = &first_gracz;
		break;
		case 2:
			tmp_f = &first_wait;
		break;
		case 3:
			tmp_f = first;
		break;
	}
	lista_graczy *to_del;
	while(*tmp_f != NULL) {
		to_del = *tmp_f;
		*tmp_f = (*tmp_f)->next;
		free(to_del);
	}
}

void clean_lista_pojedynkow(void) {
	lista_pojedynkow *to_del;
	while(first_poj != NULL) {
		to_del = first_poj;
		first_poj = first_poj->next;
		free(to_del);
	}
}