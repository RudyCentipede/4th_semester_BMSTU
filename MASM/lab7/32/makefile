run: main.exe
	./main.exe

main.exe: main.o my_strncpy.o
	gcc -m32 main.o my_strncpy.o -o main.exe

my_strncpy.o: my_strncpy.asm
	nasm -f elf -o my_strncpy.o my_strncpy.asm

main.o: main.c
	gcc -m32 -c main.c

.PHONY : clean
clean :
	rm -rf *.o *.exe *.out