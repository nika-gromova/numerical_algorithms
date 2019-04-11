CC := gcc
FLAGS := -std=c99 -Wall -Werror

app.exe: main.o math_module.o
	$(CC) -o $@ $^
	
main.o: main.c math_module.h
	$(CC) $(FLAGS) -c $<
	
math_module.o: math_module.c math_module.h
	$(CC) $(FLAGS) -c $<
	
clean:
	rm *.exe *.o