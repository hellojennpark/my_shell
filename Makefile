my_shell : my_shell.o
	gcc -o my_shell my_shell.o
	gcc -o my_ls my_ls.o
	gcc -o my_cat my_cat.o

my_shell.o : my_shell.c
	gcc -c -o my_shell.o my_shell.c
	gcc -c -o my_ls.o my_ls.c
	gcc -c -o my_cat.o my_cat.c

clean :
	rm -rf my_shell.o my_shell my_ls.o my_ls my_cat.o my_cat
