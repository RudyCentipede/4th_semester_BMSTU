.PHONY : clean run

clean :
	rm -rf *.o *.exe *.out

run :
	gcc -std=c99 -Wall -Werror -O0 -masm=intel -msse main.c -o app.exe
	./app.exe