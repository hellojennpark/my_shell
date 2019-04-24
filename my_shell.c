#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LEN 100 /* The maximum length command */


int fetch_input(char *a) {
	char p;
	int num  = 0;

	while (((p = getchar()) != '\n') && (num < MAX_LEN)) {
		a[num++] = p;
	}

	if (num == MAX_LEN && p != '\n') {
		perror("length error");
		num = -1;
	} else {
		a[num] = 0;
	}
	while (p != '\n') p = getchar();
	return num;
}


int parse(char *buffer, int length, char* args[]) {
	int args_num = 0, last = -1, i;
	args[0] = NULL;
	for (i = 0; i <= length; ++i) {
		if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i]) {
			continue;
		} else {
			if (last != i-1) {
				args[args_num] = (char*)malloc(i-last);
				if (args[args_num] == NULL) {
					perror("error");
					return 1;
				}
				memcpy(args[args_num], &buffer[last+1], i-last-1);
				args[args_num][i-last] = 0;
				args[++args_num] = NULL;
			}
			last = i;
		}
	}
	return args_num;
}


int main(void) {

	char *args[MAX_LEN/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
//	int background = 0;

/*	char *input;
	int status;
*/
	char buffer[MAX_LEN + 1];
	memset(buffer, 0, sizeof(buffer));

	int length, args_num;

	while (should_run) {
		printf("my_shell>");
		fflush(stdout);

/*
		input = (char*)malloc(MAX_LEN *sizeof(char));
		fgets(input, MAX_LEN, stdin);

		int i = 0;
		args[i] = strtok(input, " ");
		while (args[i] != NULL) {
			i++;
			args[i] = strtok(NULL, " ");
		}

*/

		length = fetch_input(buffer);
		if (length == -1) continue;
	
		args_num = parse(buffer, length, args);

		if (args_num == 0)
			goto no_input;

		if (strcmp(args[0], "exit") == 0) {
			should_run = 0;
			continue;
		}


		int background = 0;
		if (strcmp(args[args_num-1], "&") == 0) {
			args[args_num - 1] = NULL;
			background = 1;
			//printf("child process complte");
		}

		pid_t pid = fork();

		if (pid < 0) {
			perror("Fork error");
			exit(0);
		}

		int status;

		if (pid == 0) {
			//printf("waiting for child, not a background process\n");
			status = execvp(args[0], args);
			return 0;
			//printf("child process complte\n");
		}
		else{
			if(pid >0) {
				if (!background) {
					printf("waiting for child, not a background process\n");
					waitpid(pid, &status, 0);
					printf("child process complte\n");
				}
				else if(background) {
					//exit(0);
					printf("background process\n");
				}
			}
		}
	no_input:;
	}

	return 0;
}
