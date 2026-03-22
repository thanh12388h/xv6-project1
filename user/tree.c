#include "kernel/types.h"
#include "kernel/stat.h" // file status 
#include "user/user.h" 
#include "kernel/fs.h" // file structure

void tree(char* path, int depth){
	// 0. Declaration 
	int fd; // file dicription 
	struct dirent de; // name of directory, identify number
	struct stat st; // type of file  
	char buff[512], *p;

	// 1. Open the link (fd: file dicription of current file or specific file by the link)
	if((fd = open(path, 0)) < 0){
		printf("Tree cannot open %s\n", path);
		return;
	}
	
	// 2. Getting status information 
	if(fstat(fd, &st) < 0){
		printf("Tree cannot stat %s\n", path);
		close(fd);
		return;
	}

	// 3. reading current directory content 
	// using read() to read all subfile (name, inum) in the current directory. 
	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		// if the current file is empty or deleted 
		if(de.inum == 0) continue;
		// skip . and .. in the directory 
		if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
		// print space
		for(int i = 0; i < depth; i ++){
			printf("  ");
		}
		// create fully path to check type and call recursion
		strcpy(buff, path); 
		p = buff + strlen(buff);
		if(p > buff && *(p-1) != '/') *p++ = '/';
		strcpy(p, de.name);

		// check p stutus (file, dir, or not)
		if(stat(buff, &st) < 0){
			printf("Tree cannot stat %s\n", buff);
			continue;
		}
		
		// classify and handle
		// if the current file is a directory -> call recursion. 
		if(st.type == T_DIR){
			printf("%s/\n",de.name);
			// call recursion for 'buff'
			tree(buff, depth + 1);
		}
		else{
			printf("%s\n", de.name);
		}
	}
	close(fd);
}

int main(int argc, char* argv[]){
	// note: argv[0] always save the  name of currently executing file (tree). 
	// if there are no parameter. 
	if(argc < 2){
		printf("./\n");
		tree(".", 1);
	}
	// must be one parameter (tree just receive one parameter)
	else{	
		printf("%s/\n", argv[1]);
		tree(argv[1], 1);
	}
	exit(0);
}
