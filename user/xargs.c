#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXLINE 512

int main(int argc, char* argv[]){
	char* cmd[MAXARG];
	char buf[MAXLINE];
	int n = 1;
	int cmd_argc = 0;

	int arg_start = 1;

	// xu ly option -n
	if(argc > 2 && strcmp(argv[1], "-n") == 0){
		n = atoi(argv[2]);
		arg_start = 3;
	}


	for(int i = arg_start; i < argc; i ++){
		cmd[cmd_argc++] = argv[i];
	}

	char c;
	int idx = 0; 
	int collected = 0;


	
	
	while(read(0, &c, 1) == 1){
		if(c == '\n'){
			if(idx == 0) continue;
		
		buf[idx] = '\0';
		
		char *arg = malloc(strlen(buf) + 1);
		strcpy(arg,buf);
		cmd[cmd_argc + collected] = arg;
		collected++;

		idx = 0;

		if(collected == n || c == '\n'){
			cmd[cmd_argc + collected] = 0;


			int pid = fork();
			if(pid == 0){
				exec(cmd[0], cmd);
				fprintf(2, "exec failed \n");
			} else {
				wait(0);
			}

			collected = 0;

		}

		}
	else {
		buf[idx++] = c;
	}
	}
	// xu ly input cuoi neu ko co newline
	if(idx > 0){
		buf[idx] = '\0';
		char *arg = malloc(strlen(buf) + 1);
		strcpy(arg,buf);
		cmd[cmd_argc + collected] = arg;
		collected ++;

		cmd[cmd_argc + collected] = 0;

		int pid = fork();
		if(pid == 0){
			exec(cmd[0], cmd);
			exit(1);
		}
		else {
			wait(0);
		}
	}
	exit(0);
}
