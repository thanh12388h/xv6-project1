#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLINE 512

int main(int argc, char* argv[]){

	char* new_argv[MAXARG];
	int new_argc = 0;
	
	// copy to new command 
	for(int i = 1; i < argc; i ++){
		new_argv[new_argc++] = argv[i];	
	}

	char c;
	int id = 0;
	char buff[MAXLINE]; // chua 1 word
	int c_word = 0;
	
	// gia su chac chan co du lieu tu stdin
	// logic doc tung dong du lieu 
	while(read(0, &c, 1) == 1){
		// neu doc duoc 1 word hoac xuong dong thi dung lai xu ly
		if(c == '\n' || c == ' '){
			// xoa khoang trang thua, chi nap word ("hello" not "   hello" or "hello  ")
			if(id == 0) continue;
			
			
			buff[id] = '\0'; // ket thuc chuoi
			
			char* word = malloc(strlen(buff) + 1);
			strcpy(word, buff);
	
			new_argv[new_argc + c_word++] = word; //*
			
			
			id = 0; // reset 
			
			
			// neu da het 1 dong, exec
			if(c == '\n'){
				new_argv[new_argc + c_word] = 0; // tu cuoi luon la NULL
				
				int p_id = fork();
				if(p_id == 0){ // child process
					exec(new_argv[0], new_argv);// echo + new cmd
					printf("exec fail \n");
					exit(1);
				}
				else{ // parent process 
					wait(0);
					for(int i = 0; i < c_word; i ++){
						free(new_argv[new_argc + i]); // giai phong cac word da dung xong
						new_argv[new_argc + i] = 0; // dat lai cac word trong la NULL de bat dau doc dong tiep theo
					}
					c_word = 0; // reset doc dong moi 
				}
			}
		}
		else{
			// day ky tu vao buff (word)
			buff[id++] = c;
		}
	}
	
	// handle EOF: neu dong cuoi cua stdin khong co newline \n
	if(id > 0){
		buff[id] = '\0'; // ket thuc chuoi
		
		char* word = malloc(strlen(buff) + 1);
		strcpy(word, buff);

		new_argv[new_argc + c_word++] = word; //*
		
		
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



