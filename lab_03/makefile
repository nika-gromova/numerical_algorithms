CC := gcc
FLAGS := -std=c99 -Wall -Werror

app.exe: main.o allocate.o process.o
	$(CC) -o $@ $^

main.o: main.c defines.h allocate.h process.h
	$(CC) $(FLAGS) -c $<
	
allocate.o: allocate.c allocate.h defines.h
	$(CC) $(FLAGS) -c $<
	
process.o: process.c process.h defines.h
	$(CC) $(FLAGS) -c $<

clean:
	rm *.exe *.o