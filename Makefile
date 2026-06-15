CC=gcc
CFLAGS=-Wall

OBJ=main.o parser.o builtin.o executor.o history.o redirection.o pipe.o process.o input.o

shell: $(OBJ)
	$(CC) $(OBJ) -o shell

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o shell