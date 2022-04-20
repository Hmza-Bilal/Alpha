/*
*  Video Lecture: 14
*  Programmer: Arif Butt
*  Course: System Programming with Linux
*  lsv3.c Long Listing of Files
*  usage: ./a.out 
         ./a.out dirpath
         ./a.out dir1 dir2 dir3
*/


#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>

char perm[11],usr[32],Grp[32],Time[32];
char Filenames [100][100];

extern int errno;
void do_ls(char*);
void printLongList(char*);
void File_Permission(mode_t);
void getUsr(uid_t);
void getGrp(gid_t);
void setTime(long);

int main(int argc, char* argv[]){
   if (argc == 1){
         printf("Directory listing of pwd:\n");
      printLongList(".");
   }
   else{
      int i = 0;
      while(++i < argc){
         printf("Directory listing of %s:\n", argv[i] );
	 printLongList(argv[i]);
      }
   }
   
   return 0;
}


void do_ls(char * dir)
{
   struct dirent * entry;
   DIR * dp = opendir(dir);
   if (dp == NULL){
      fprintf(stderr, "Cannot open directory:%s\n",dir);
      return;
   }
   errno = 0;
   while((entry = readdir(dp)) != NULL){
         if(entry == NULL && errno != 0){
  		perror("readdir failed");
		exit(1);
  	}else{
                if(entry->d_name[0] == '.')
                    continue;
        }     	printf("%s\n",entry->d_name);
   }
   closedir(dp);
}
void printLongList(char *dirName){
	struct dirent *entry;
	struct stat stats;
	DIR *dir=opendir(dirName);
	if (dir == NULL){
      		fprintf(stderr, "Cannot open directory:%s\n",dirName);
      		return;
	}
	errno = 0;
	int i=0;
	while((entry=readdir(dir))!=NULL){
		if(entry == NULL && errno != 0){
			 perror("readdir failed");
			 exit(errno);
	      	}	
	      	else{
	      		strcpy(Filenames[i++],entry->d_name);
		}
		
	}
		for(int j=0;j<i;j++){
		for(int k=0;k<i-1;k++){
			if(strcmp(Filenames[j],Filenames[k])<0){
				char temp[30];
				strcpy(temp,Filenames[j]);
				strcpy(Filenames[j],Filenames[k]);
				strcpy(Filenames[k],temp);
				}
			}
	}
	
	for(int j=0;j<i;j++){
		if(Filenames[j][0]!='.'){
			char name[PATH_MAX];	
			sprintf(name,"%s/%s",dirName,Filenames[j]);
			int ans=lstat(name,&stats);
			if(ans!=-1){
				File_Permission(stats.st_mode);
				getUsr(stats.st_uid);
				getGrp(stats.st_gid);
				setTime(stats.st_mtime);
				
				printf("%s %ld %s %s %6ld %s %s\n",perm,stats.st_nlink,usr,Grp,stats.st_size,Time,Filenames[j]); 			}
				
			}
		}
	
	      	
}
void File_Permission(mode_t mode){
	strcpy(perm,"----------");
				//permissions for owner
	if((mode & 0000400) == 0000400) perm[1] = 'r';	
   	if((mode & 0000200) == 0000200) perm[2] = 'w';
   	if((mode & 0000100) == 0000100) perm[3] = 'x';
				//permissions for group
   	if((mode & 0000040) == 0000040) perm[4] = 'r';
   	if((mode & 0000020) == 0000020) perm[5] = 'w';
   	if((mode & 0000010) == 0000010) perm[6] = 'x';
				//Other permissions
   	if((mode & 0000004) == 0000004) perm[7] = 'r';
   	if((mode & 0000002) == 0000002) perm[8] = 'w';
   	if((mode & 0000001) == 0000001) perm[9] = 'x';
				//special  permissions
   	if((mode & 0004000) == 0004000) perm[3] = 's';
   	if((mode & 0002000) == 0002000) perm[6] = 's';
   	if((mode & 0001000) == 0001000) perm[9] = 't';
				//7 types of file permissions
   	if ((mode &  0170000) == 0010000) perm[0]='p';
   	else if ((mode &  0170000) == 0020000) perm[0]='c';
   	else if ((mode &  0170000) == 0040000) perm[0]='d';
   	else if ((mode &  0170000) == 0060000) perm[0]='b';
   	else if ((mode &  0170000) == 0100000) perm[0]='-';
   	else if ((mode &  0170000) == 0120000) perm[0]='l';
   	else if ((mode &  0170000) == 0170000) perm[0]='s';
}

void getUsr(uid_t uid){
   struct passwd * pwd = getpwuid(uid);
   strcpy(usr,pwd->pw_name);
}

void getGrp(gid_t gid){
   struct group * grp = getgrgid(gid);
   strcpy(Grp,grp->gr_name);
}

void setTime(long seconds){
	strcpy(Time,ctime(&seconds));
	Time[17]='\0';
}
