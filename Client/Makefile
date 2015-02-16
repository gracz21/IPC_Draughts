all: klient 

klient: funkcje_k.o klient.o 
	gcc -Wall -g funkcje_k.o klient.o  -o klient
funkcje_k.o: funkcje_k.c funkcje_k.h struktury.h
	gcc -c -Wall -g funkcje_k.c -o funkcje_k.o
klient.o: klient.c funkcje_k.h struktury.h
	gcc -c -Wall -g klient.c -o klient.o

clean:
	rm *.o
	rm klient
