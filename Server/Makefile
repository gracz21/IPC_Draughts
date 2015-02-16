all: serwer 

serwer: lista.o funkcje_s.o serwer.o 
	gcc -Wall -g funkcje_s.o lista.o serwer.o -o serwer 
funkcje_s.o: funkcje_s.c funkcje_s.h lista.h struktury.h
	gcc -c -Wall -g funkcje_s.c -o funkcje_s.o
lista.o: lista.c lista.h struktury.h
	gcc -c -Wall -g lista.c -o lista.o
serwer.o: serwer.c funkcje_s.h lista.h struktury.h
	gcc -c -Wall -g serwer.c -o serwer.o

clean:
	rm *.o
	rm serwer
