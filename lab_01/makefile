CC := gcc
FLAGS := -std=c99 -Wall -Werror

app.exe: main.o io.o
	$(CC) -o $@ $^

main.o: main.c defines.h io.h
	$(CC) $(FLAGS) -c $<

io.o: io.c io.h defines.h
	$(CC) $(FLAGS) -c $<
	
clean:
	rm *.exe *.o