simple_shell : simple_shell.o
	gcc -o simple_shell simple_shell.o

simple_shell.o : simple_shell.c
	gcc -c -o simple_shell.o simple_shell.c
