CC=clang
#CFLAGS=-Wall -Werror -pedantic -std=gnu11
CFLAGS=-Wall -Werror -pedantic -std=gnu11 -DDEBUG -g3 -ggdb
EXEC=uBash
OBJS=main.o utils.o shell.o parse.o commands.o

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

main.o: utils.o shell.o parse.o commands.o main.c library.h
	$(CC) $(CFLAGS) -c main.c

shell.o: utils.o parse.o commands.o shell.c library.h
	$(CC) $(CFLAGS) -c shell.c

parse.o: utils.o commands.o parse.c library.h
	$(CC) $(CFLAGS) -c parse.c

commands.o: utils.o commands.c library.h
	$(CC) $(CFLAGS) -c commands.c

utils.o: utils.c library.h
	$(CC) $(CFLAGS) -c utils.c

.PHONY: clear

clear:
	rm -rf *.o vgcore.* $(EXEC)
