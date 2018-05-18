CFLAGS = -Wall
 sdl = `sdl-config --cflags --libs` -lSDL_image


main: main.c  mesfonctions.o mesfonctions.h
	gcc $(CFLAGS) main.c mesfonctions.o  -o main $(sdl)  -lm -lpthread -O3

mes_fonctions.o:	mesfonctions.c mesfonctions.h 
	gcc $(CFLAGS) -c main.c mesfonctions.c mesfonctions.h $(sdl)
 


	




clean:
	rm -f main 
	rm -f *.o