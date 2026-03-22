#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLINE 512

int main(int argc, char* argv[]){

	char* new_argv[MAXARG];
	int new_argc = 0;
	
	for(int i = 1; i < argc; i ++){
		new_argv[new_argc++] = argv[i];	
	}

	char c;
	int id = 0;
	char buff[MAXLINE]; 
	int c_word = 0;
	

	while(read(0, &c, 1) == 1){
		if(c == '\n' || c == ' '){

			if(id == 0) continue;
			
			
			buff[id] = '\0';
			
			char* word = malloc(strlen(buff) + 1);
			strcpy(word, buff);
	
			new_argv[new_argc + c_word++] = word; 
			
			
			id = 0; 
			
			
			if(c == '\n'){
				new_argv[new_argc + c_word] = 0; 
				
				int p_id = fork();
				if(p_id == 0){ 
					exec(new_argv[0], new_argv);
					printf("exec fail \n");
					exit(1);
				}
				else{ 
					wait(0);
					for(int i = 0; i < c_word; i ++){
						free(new_argv[new_argc + i]); 
						new_argv[new_argc + i] = 0; 
					}
					c_word = 0; 
				}
			}
		}
		else{
			buff[id++] = c;
		}
	}
	
	if(id > 0){
		buff[id] = '\0'; 
		
		char* word = malloc(strlen(buff) + 1);
		strcpy(word, buff);

		new_argv[new_argc + c_word++] = word; 
		
		
		new_argv[new_argc + c_word] = 0; 
		
		int p_id = fork();
		if(p_id == 0){ 
			exec(new_argv[0], new_argv);
			printf("exec fail \n");
			exit(1);
		}
		else{ 
			wait(0);
			for(int i = 0; i < c_word; i ++){
				free(new_argv[new_argc + i]);  
				new_argv[new_argc + i] = 0; 
			}
		}
	
	}


	exit(0);
}



