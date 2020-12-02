CC=clang
CFLAGS=-Wall -Werror -pedantic -g3
EXEC=uBash
OBJS=main.o utils.o shell.o parse.o

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

parse.o: main.o utils.o shell.o parse.c library.h
	$(CC) $(CFLAGS) -c parse.c

shell.o: main.o utils.o shell.c library.h
	$(CC) $(CFLAGS) -c shell.c

utils.o: utils.c library.h
	$(CC) $(CFLAGS) -c utils.c

main.o: main.c library.h
	$(CC) $(CFLAGS) -c main.c

.PHONY: clear

clear:
	rm -rf *.o vgcore.* $(EXEC)
