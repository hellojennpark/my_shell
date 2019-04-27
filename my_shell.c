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
				//입력받은 인자의 메모리 공간을 동적으로 할당 받는다.
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

	char buffer[MAX_LEN + 1];
	memset(buffer, 0, sizeof(buffer));

	int length, args_num;

	//쉘 시작 하기 전 안내 문구
	printf("\n\n\n");
	printf("[CSE4009 SYSTEM PROGRAMMING] TERM PROJECT\n");
	printf("나만의 쉘 만들기\n");
	printf("소프트웨어학부 2017011985 박예진\n");
	printf("\n\n\n");
	while (should_run) {
		printf("my_shell>");
		//flush는 버퍼가 차지 않았어도 강제로 출력을 해준다.
		fflush(stdout);

		length = fetch_input(buffer);
		if (length == -1) continue;
	
		args_num = parse(buffer, length, args);

		//입력된 인자의 갯수가 0개이면 입력이 없다.
		//빈입력을 했을 경우, no_input전까지의 코드를 건너뛴다.
		if (args_num == 0)
			goto no_input;

		//쪼개진 문자열을 비교해서 쪼개진 문자열이 exit과 같다면 프로그램을 종료시킨다.
		if (strcmp(args[0], "exit") == 0) {
			should_run = 0;
			continue;
		}


		int background = 0;
		//쪼개진 문자열을 비교해서 쪼개진 문자열이 &으로 마무리가 된다면 background 프로세스로 실행하기 위해 flag를 활성화한다.
		if (strcmp(args[args_num-1], "&") == 0) {
			args[args_num - 1] = NULL;
			background = 1;
		}

		//pid(process id)는 포크를 하면 생기는 반환값이다.
		pid_t pid = fork();

		if (pid < 0) {
			//fork함수는 실패를 하면 -1을 반환한다(따라서 pid가 0보다 작으면 fork 실패)
			perror("Fork error");
			exit(0);
		}

		//status는 자식 프로세스의 상태를 나타낸다.
		int status;
		
		//fork 함수는 성공했을 때 자식 프로세스에게는 0을, 부모 프로세스에게는 자식 프로세스의 pid를 반환한다.
		if (pid == 0) {
			//자식 프로세스를 실행한다.
			status = execvp(args[0], args);
			return 0;
		}
		else{
			//부모 프로세스를 실행한다.
			//부모 프로세스가 자식프로세스의 아이디를 아는 방법은 fork의 리턴값으로 받아오는 수 밖에 없기 때문에 부모프로세스에게 자식 프로세스의 아이디를 반환한다.
			//프로세스의 아이디는 양수이다.
			if(pid >0) {
				if (!background) {
					//background flag가 활성화되어 있다면, 자식 프로세스의 종료를 기다린다.
					//background = 0이므로 !background = 1이고 flag 활성화 
					printf("(waiting for child, not a background process)\n");
					//waitpid는 첫번째 인자로 pid를 받고, 그 pid를 갖는 자식 프로세스의 종료를 기다린다. 
					waitpid(pid, &status, 0);
					printf("(child process complte)\n");
				}
				else if(background) {
					//부모 프로세스를 바로 종료시킨다.
					printf("(background process)\n");
				}
			}
		}
	no_input:;
	}

	return 0;
}
