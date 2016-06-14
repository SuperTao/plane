#MAIN    : plane
#CC = gcc
#CFLAGS = 
#INCLUDE : -I/usr/include/SDL
#LIBS    : -lSDL -lSDL_image
#OBJS = game.o init.o joy.o plane.o timer.o 
#
#all     : $(MAIN)
#.c.o    : 
#	$(CC) -c $(CFLAGS) $(INCLUDE) $<
#$(MAIN) : $(OBJS)
#	$(CC) -o $(MAIN) $(OBJS) $(LIBS)
#clean:
#	rm -f *.o a.out $(MAIN) core *~
plane : game.o init.o joy.o plane.o timer.o 
	cc -o plane game.o init.o joy.o plane.o timer.o -lSDL -lSDL_image 
game.o: game.c plane.h
	cc -c game.c 
init.o: init.c plane.h 
	cc -c init.c
joy.o : joy.c plane.h 
	cc -c joy.c
plane.o:plane.c plane.h 
	cc -c plane.c 
timer.o:timer.c plane.h 
	cc -c timer.c 
clean :
	rm -f plane game.o init.o joy.o plane.o timer.o
#plane : game.o init.o joy.o plane.o timer.o 
#	cc -o plane game.o init.o joy.o plane.o timer.o -lSDL -lSDL_image -I/usr/include/SDL
#game.o: game.c plane.h
#	cc -c game.c -lSDL -lSDL_image -I/usr/include/SDL
#init.o: init.c plane.h 
#	cc -c init.c -lSDL -lSDL_image -I/usr/include/SDL
#joy.o : joy.c plane.h 
#	cc -c joy.c -lSDL -lSDL_image -I/usr/include/SDL
#plane.o:plane.c plane.h 
#	cc -c plane.c -lSDL -lSDL_image -I/usr/include/SDL
#timer.o:timer.c plane.h 
#	cc -c timer.c -lSDL -lSDL_image -I/usr/include/SDL
#clean :
#	rm -f plane game.o init.o joy.o plane.o timer.o
#	cc -o plane game.o init.o joy.o plane.o timer.o -lSDL -lSDL_image -I/usr/include/SDL
