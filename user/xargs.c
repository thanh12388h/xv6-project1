


int main(int argc, char* argv[]){
	
	char* new_argv[100];
	
	// copy to new command 
	for(int i = 1; i < argc; i ++){
		new_argv[i - 1] = argv[i];	

	}

	
	char c;
	int id = 0;
	char* buff; // chua 1 word
	int c_word = 0;

	while(read(0, &c, 1) == 1){
		if(c == ' ' || c == '\n'){
			buff[id] = '\0'; // ket thuc chuoi
			new_argv[argc + c_word] = buff;
			id = 0; // reset 
			 
			

		}
		else{
				
		}
	)

	










}



