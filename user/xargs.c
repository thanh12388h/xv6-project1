#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]){
	char buf[512]; // command 
	char* params[MAXARG]; // list of pointer 
	char c;	
	int n, i = 0;

	if(argc < 2){
		printf("Usage: xargs command [args...]\n");
		exit(1);
	}
	// copy arguments from argv[] into params[]/ 
	for(int j = 1; j < argc; j ++){
		params[j - 1] = argv[j];
	}
	// handle data flow 
	// read each character 
	while((n = read(0, &c, 1)) > 0){
		// read until encounter a newline (\n)
		if(c == '\n'){
			buf[i] = 0; // end the current string
			// execute command in the child process 
			if(fork() == 0){ // child process
				params[argc - 1] = buf;
				params[argc] = 0;
				exec(params[0], params);
				exit(0);
			}
			wait(0); // waiting child process 
			i = 0; // reset to read a next line. 
		}
		else{
			if(i < sizeof(buf) - 1){
				buf[i++] = c;
			}
		}
	}
	
	// if there are still data in buf but have encoutered \n
	if(i > 0){
		buf[i] = 0; // close the buff 
		if(fork() == 0){
			params[argc - 1] = buf;
			params[argc] = 0;
			exec(params[0], params);
			exit(0);
		}
		wait(0);
	}
	exit(0);
}
	
