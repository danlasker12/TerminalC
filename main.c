#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#define MAX_DIR_LEN 50
#define MAX_USERNAME_LEN 30
#define MAX_PROC_PATH 20
#define PROC_NAME 30
#define USER_INPUT 100

//Colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

typedef struct dirent DIRENT;
typedef struct _PACKET {
	
}PACKET;

void echo(char*);
void pwd();
void ls();
void cd(char*);
void whoami();
void makedir(char*);
void touch(char*);
void ps();
void myFgets(char*,int);
void clear();
void myPrintf(char*,char*);


int main(int argc, char* argv[]){
	//makedir(argv[1]);		
	char input[USER_INPUT];
	char* token;

	clear();

	while(1){
		memset(input,0,USER_INPUT);
		pwd();
		myPrintf(KWHT,"> ");
		myFgets(input,USER_INPUT);
		token = strtok(input," ");
		if(!strcmp(token,"echo")){
			token = strtok(NULL,"\0");
			echo(token);
		}
		else if(!strcmp(token,"clear")){
			clear();
		}
		else if(!strcmp(token,"pwd")){
			pwd();
			printf("\n");
		}
		else if(!strcmp(token,"ls")){
			ls();
		}
		else if(!strcmp(token,"ps")){
			ps();
		}
		else if(!strcmp(token,"whoami")){
			whoami();
		}
		else if(!strcmp(token,"cd")){
			token = strtok(NULL,"\0");
			cd(token);
		}
		else if(!strcmp(token,"mkdir")){
			token = strtok(NULL,"\0");
			makedir(token);
		}
		else if(!strcmp(token,"touch")){
			token = strtok(NULL,"\0");
			touch(token);
		}
		else{
			myPrintf(KRED,"Command not found! Suck your nan!\n");
		}		
	}

	return 0;


}

void echo(char* str){
	printf("%s\n",str);	
}

void pwd(){
	
	char directory[MAX_DIR_LEN];
	memset(directory, 0, MAX_DIR_LEN);//initialize directory 
	if(getcwd(directory,MAX_DIR_LEN)!=NULL){
		myPrintf(KCYN,directory);
	}else{
		myPrintf(KRED, "PWD ERROR");
	}
}

void ls(){
	DIR* d;
	DIRENT* dir;
	d = opendir(".");
	if(d){
		while((dir = readdir(d))!=NULL){
			if(strcmp(dir->d_name,".")&&strcmp(dir->d_name,"..")){
				printf("%s\n",dir->d_name);
			}		
		}			
		closedir(d);
	}
}

void cd(char* dir){
	if(chdir(dir) != 0){
		myPrintf(KRED, "Cannot change directory");
	}
}

void whoami(){
	
	char* buffer;
	buffer = getlogin();
	if(buffer!=NULL){
		printf("%s\n",buffer);
	}
	else{
		myPrintf(KRED, "You suck!");
	}
}


void makedir(char* str){
	struct stat st = {0};

	if (stat(str, &st) == -1) {
		mkdir(str, 0700);
		printf("Directory made!\n");
		return;
	}
	myPrintf(KRED, "Sasik!");
}


void touch(char* str){
	FILE* f;
	f = fopen(str,"w");
	
	if(f!=NULL){
		printf("File Created\n");
		fclose(f);	
	}else{
		myPrintf(KRED, "You suck");
	}
}

int fsize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int sz=ftell(fp);
    fseek(fp,prev,SEEK_SET);
    return sz;
}

void ps(){
	char path[MAX_PROC_PATH];
	char procName[PROC_NAME];
	int pid;
	FILE* proc;
	DIR* d;
	DIRENT* dir;

	d = opendir("/proc");

	while((dir = readdir(d))!=NULL){
		memset(path, 0, MAX_PROC_PATH);
		memset(procName, 0, PROC_NAME);
		strcpy(path, "/proc/");
		pid = atoi(dir->d_name);
		if(pid <= 0){
			continue;		
		}
		strcat(path, dir->d_name);
		strcat(path, "/comm");
		proc = fopen(path, "r+");
		if(proc != NULL){
			fread(procName, sizeof(char), PROC_NAME, proc);
			printf("%d : %s\n",pid, procName);
			fclose(proc);
		}
	}
}

void myFgets(char* input,int len){
	fflush(stdout);
	fgets(input,len,stdin);
	input[strlen(input)-1] = '\0';
}

void clear(){
	//const char* CLEAR_SCREEN = "\e[1;1H\e[2J";
	//write(STDOUT_FILENO,CLEAR_SCREEN,12);
	
	system("clear");
	
}

void myPrintf(char* color,char* input){
	printf("%s%s",color,input);
}







