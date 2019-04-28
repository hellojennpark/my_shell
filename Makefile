my_shell : my_shell.o
	gcc -o my_shell my_shell.o
	gcc -o my_ls my_ls.o
	gcc -o my_cat my_cat.o
	gcc -o my_pipe my_pipe.o
	gcc -o my_redirection my_redirection.o

my_shell.o : my_shell.c
	gcc -c -o my_shell.o my_shell.c
	gcc -c -o my_ls.o my_ls.c
	gcc -c -o my_cat.o my_cat.c
	gcc -c -o my_pipe.o my_pipe.c
	gcc -c -o my_redirection.o my_redirection.c

clean :
	rm -rf my_shell.o my_shell my_ls.o my_ls my_cat.o my_cat my_pipe.o my_pipe my_redirection.o my_redirection
