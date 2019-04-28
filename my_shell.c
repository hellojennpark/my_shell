#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_LEN 100 /* The maximum length command */
#define	MAX_PATH 1024 /* The maximum path length command */

int fetch_input(char *a) {
	char p;
	int num  = 0;

	//getchar를 이용해서 사용자가 엔터를 하기 전까지 입력을 받는다.
	//만약 사용자의 입력이 입력 최대길이를 넘어간다면 더이상 입력을 받지 않는다.
	while (((p = getchar()) != '\n') && (num < MAX_LEN)) {
		a[num++] = p;
	}
	//입력최대길이를 넘어설 경우 경고문을 발생시킨다.
	if (num == MAX_LEN && p != '\n') {
		perror("length error");
		num = -1;
	} else {
		a[num] = 0;
	}
	//엔터를 하기 전까지 계속 입력을 받는다.
	while (p != '\n') p = getchar();
	return num;
}


int parse(char *buffer, int length, char* args[]) {
	int args_num = 0, last = -1, i;
	args[0] = NULL;
	//입력받은 문자열을 파싱한다.
	for (i = 0; i <= length; ++i) {
		//뛰어쓰기나 탭 처리
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
	printf("구현한 기능을 확인하고 싶으면 myfunction을 입력해주세요.\n");
	printf("자세한 사용법은 위키를 참고해주세요.\n");
	printf("https://github.com/vivi9814/my_shell/wiki/My-Own-Linux-Shell-in-C\n");
	printf("주의사항1 : 입력할 수 있는 최대 문자열 길이는 100자입니다.\n");
	printf("주의사항2 : 현재 위치를 출력할 때 위치의 문자열 길이가 1024를 초과하면 에러가 발생합니다.\n");
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

		//기능
		//(strcmp를 이용해서 쪼개진 문자열을 비교함)
		//0. 구현한 기능들에 대해 알 수 있는 명령어
		if (strcmp(args[0], "myfunction") == 0){
			printf("#######MY FUNCTION LIST#######\n");
			printf("0. myfunction\n");
			printf("1. myexit\n");
			printf("2. mycd\n");
			printf("3. background check(&)\n");
			printf("4. mypwd\n");
			printf("##############################\n");
		}

		//1. 쪼개진 문자열이 exit과 같다면 프로그램을 종료시킨다.
		if (strcmp(args[0], "myexit") == 0) {
			printf("프로그램 my_shell을 종료합니다.\n");
			should_run = 0;
		}

		if (strcmp(args[0], "exit") == 0) {
			printf("exit이 존재하지 않기 때문에 myexit으로 대체합니다.\n");
			printf("프로그램 my_shell을 종료합니다.\n");
			should_run = 0;
		}

		//2.쪼개진 문자열이 &으로 마무리가 된다면 background 프로세스로 실행하기 위해 flag를 활성화한다.		
		int background = 0;
		if (strcmp(args[args_num-1], "&") == 0) {
			args[args_num - 1] = NULL;
			background = 1;
		}

		//3. 쪼개진 문자열의 첫번째가 cd라면 두번째 문자열에 있는 디렉터리로 이동한다.
		if (strcmp(args[0], "mycd") == 0){
			chdir(args[1]);
		} 
		if (strcmp(args[0], "cd") == 0){
			printf("cd가 존재하지 않기 때문에 mycd로 대체합니다.\n");
			printf("mycd를 실행했습니다.\n");
			chdir(args[1]);
		} 

		// 4. 쪼개진 문자열이 mypwd와 같다면 현재 위치를 출력한다.

		// if (strcmp(args[0], "mypwd") == 0){
		// 	char path[MAX_PATH];
		// 	if (getcwd(path, sizeof(path)) != NULL) {
		// 		printf("My pwd : %s\n", path);
		// 	}
		// 	else{
		// 		perror("getpwd() error\n");
		// 	}
		// }

		// mypwd 동적 할당 ver

		char* mypwd(void){
			char *buf, *pwd;
			size_t size = MAX_PATH;

			buf = malloc(size);
			if (!buf) return 0;
			for (;;) {
				errno = 0;
				if (getcwd(buf,size)) return buf;
				if (errno != ERANGE) break;
				size = size*2;
				pwd = realloc(buf, size);
				if (!pwd) break;
				buf = pwd;
			}			
			free(buf);
			return 0;
		}

		if (strcmp(args[0], "mypwd") == 0){
			char *path;

			path = mypwd();
			if (!path) {
				perror("getcwd");
				exit(1);
			}
			puts(path);
			free(path);
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
					// printf("\n(waiting for child, not a background process)\n");
					//waitpid는 첫번째 인자로 pid를 받고, 그 pid를 갖는 자식 프로세스의 종료를 기다린다. 
					waitpid(pid, &status, 0);
					// printf("\n(child process complte)\n");
				}
				else if(background) {
					//부모 프로세스를 바로 종료시킨다.
					printf("(background process)\n\n");
				}
			}
		}
	no_input:;
	}

	return 0;
}
