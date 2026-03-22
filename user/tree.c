#include "kernel/types.h"
#include "kernel/stat.h" // file status 
#include "user/user.h" 
#include "kernel/fs.h" // file structure

void tree(char* path, int depth){ 
	int fd;  
	struct dirent de; // name of directory, identify number
	struct stat st; // type of file  
	char buff[512], *p;

	if((fd = open(path, 0)) < 0){
		printf("Tree cannot open %s\n", path);
		return;
	}
	
	if(fstat(fd, &st) < 0){
		printf("Tree cannot stat %s\n", path);
		close(fd);
		return;
	}

	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		if(de.inum == 0) continue;
		if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
		
		for(int i = 0; i < depth; i ++){
			printf("  ");
		}

		strcpy(buff, path); 
		p = buff + strlen(buff);
		if(p > buff && *(p-1) != '/') *p++ = '/';
		strcpy(p, de.name);

		if(stat(buff, &st) < 0){
			printf("Tree cannot stat %s\n", buff);
			continue;
		}
		
		if(st.type == T_DIR){
			printf("%s/\n",de.name);
			tree(buff, depth + 1);
		}
		else{
			printf("%s\n", de.name);
		}
	}
	close(fd);
}

int main(int argc, char* argv[]){
	if(argc == 1){
		printf("./\n");
		tree(".", 1);
	}

	else{	
		if(strcmp(argv[1], "/") == 0){
			printf("%s\n", argv[1]);
		}
		else{
			printf("%s/\n", argv[1]);
		}
		tree(argv[1], 1);
	}
	exit(0);
}
