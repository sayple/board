#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/unistd.h>
#include <sys/wait.h>
#include <termio.h>
#include <pthread.h>
#include "macro.h"
#include "cursorCon.h"
#include <sys/stat.h>
#include <fcntl.h>
#define CR '\012'
#define BUFFER_SIZE 1024
#define MAXSIZE 128
void clean_thread(void* arg);
void* reader_thread(void *arg);
void* writer_thread(void *arg);
pthread_t tid1, tid2;
struct termio tbuf, oldtbuf;

int main(int argc, char *argv[])
{
	if(argc!=2){
		fprintf(stderr, "Usage : ./client IP_ADDRESS\n");
		return 1;
	}

    if(ioctl(0, TCGETA, &tbuf) == -1) { // 현재터미널모드
        perror("ioctl");    exit(1);
    }
    oldtbuf=tbuf;
	oldtbuf.c_lflag|=ECHO;
	oldtbuf.c_lflag|=ICANON;
    tbuf.c_lflag &= ~ECHO;
    tbuf.c_lflag&=~ICANON;
   	tbuf.c_cc[VMIN] =1;
    tbuf.c_cc[VTIME]=0;
	int sockfd, new_fd;
	struct sockaddr_in user_addr;
	struct sockaddr_in board_addr;
	int sin_size;
	

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	user_addr.sin_family = AF_INET;
	user_addr.sin_port = htons(60000);
	user_addr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(&(user_addr.sin_zero), '\0', 8);

	if(connect(sockfd, (struct sockaddr*)&user_addr, sizeof(user_addr)) == -1){
		perror("CONNECT");
		return 1;
	}

	if(pthread_create(&tid2, NULL, reader_thread, &sockfd) != 0){
		perror("pthread_create");
	}
	if(pthread_create(&tid1, NULL, writer_thread, &sockfd) != 0){
		perror("pthread_create");
	}

	pthread_join(tid1, NULL);
	
	close(sockfd);

	return 0;
}

void* reader_thread(void *arg){
	int sock = *((int*)arg);

	int n;
	char buffer[1024];

	while(1){
		n = recv(sock, buffer, 1024, 0);
		if(n <= 0){
			printf("\n서버 연결 끊김\n");
			break;
		}
		buffer[n] = '\0';
		pthread_cleanup_push(clean_thread,NULL);
		if(strncmp(buffer,"│                                                               PW : ",strlen("│                                                               PW : "))==0){
			if(ioctl(0, TCSETAF, &tbuf)==-1) {perror("ioctl"); exit(1);}
			printf("%s", buffer);
		}
		else if(strncmp(buffer,"clear!!",7)==0){
			if(ioctl(0, TCSETAF, &oldtbuf)==-1) {perror("ioctl"); exit(1);}
			nnclear();
		}
		//아래는 다운로드 함수 클라우드 투 클라이언트
		else if(strncmp(buffer,"__FiLeOpEn__",strlen("__FiLeOpEn__"))==0){
			
			int fs;
			usleep(5000);
			char fileName[128];
			char bufRcv[MAXSIZE];
			struct stat fileInfo;
			memset(fileName, 0, 128);
			n = recv(sock, fileName, 1024, 0);
			usleep(5000);
			sprintf(buffer,"downloads/%s",fileName);
			if((fs =open(buffer,O_WRONLY | O_CREAT,0666))==0){
				perror("file open error!\n");
				close(fs);
				return NULL;
			}
			printf("%s Download start!\n",fileName);
			while(1){
				memset(bufRcv, 0, MAXSIZE);
				n=recv(sock,bufRcv,MAXSIZE,0);
				if(n<=0) break;
				if(strcmp(bufRcv,"end")==0) break;
				write(fs, bufRcv,n);
			}
			close(fs);
			usleep(5000);
			stat(buffer,&fileInfo);
			printf("File 사이즈 : %d\t",(int)fileInfo.st_size);
			sleep(1);
			printf("\n%s Download Complete!\n",fileName);
		}
		//아래는 업로드 함수 클라이언트 투 클라우드
		else if(strncmp(buffer,"__FiLeUploaD__",strlen("__FiLeUploaD__"))==0){
			
			int fs;
			usleep(5000);
			char fileName[128];
			char bufRcv[MAXSIZE];
			struct stat fileInfo;
			memset(fileName, 0, 128);
			n = recv(sock, fileName, 1024, 0);
			usleep(5000);
			memset(buffer,0,1024);
			sprintf(buffer,"../server/cloud/%s",fileName);
			if((fs =open(buffer,O_WRONLY | O_CREAT,0666))==0){
				perror("file open error!\n");
				close(fs);
				return NULL;
			}
			printf("%s upload start!\n",fileName);
			while(1){
				memset(bufRcv, 0, MAXSIZE);
				n=recv(sock,bufRcv,MAXSIZE,0);
				if(n<=0) break;
				if(strcmp(bufRcv,"end")==0) break;
				write(fs, bufRcv,n);
			}
			close(fs);
			usleep(5000);
			stat(buffer,&fileInfo);
			printf("File 사이즈 : %d\t",(int)fileInfo.st_size);
			sleep(1);
			printf("\n%s upload Complete!\n",fileName);
		}
		else if(strncmp(buffer,"startChaTtiNg!!",strlen("startChaTtiNg!!"))==0){
			usleep(5000);
			char** chatList=(char**)malloc(sizeof(char*)*15);
			for(int i=0;i<15;i++){
				chatList[i] = (char*)malloc(sizeof(char)*1024);
				memset(chatList[i],0,sizeof(char)*1024);
			}
			
			int n,chatCount=0,printCount;
			while(1){
				usleep(5000);
				memset(buffer, 0, sizeof(char)*1024);
				n = recv(sock, buffer, 1024, 0);
				if(strncmp(buffer,"goutMyyarddddd!!",strlen("goutMyyarddddd!!"))==0){
					for(int i=0;i<15;i++){
						free(chatList[i]);
					}
					free(chatList);
					break;
				}
				buffer[n-1]='\0';
				nnclear();
				if(chatCount<15){
					strcpy(chatList[chatCount++],buffer);
				}
				else{
					char* delelePoint = chatList[0];
					char* tempWord=(char*)malloc(sizeof(char)*1024);
					//memcpy(&chatList[0],&chatList[1],sizeof(char*)*15);
					for(int i=0;i<14;i++){
						chatList[i]=chatList[i+1];
					}
					free(delelePoint);
					chatList[14]=tempWord; 
					strcpy(tempWord,buffer);
				}
				usleep(5000);
				printCount=0;
				gotoxy(0,0);
				printf("┌\n");
				while(*(chatList[printCount])!='\0'){
					printf("│ %s\n",chatList[printCount]);
					printCount++;
					if(printCount>=15)break;
				}
				while((15-printCount)>0){
					printf("│\n");
					printCount++;
				}
				
				printf("└\n");
			}
		}
		else{
			
			if(ioctl(0, TCSETAF, &oldtbuf)==-1) {perror("ioctl"); exit(1);}
			printf("%s", buffer);
		}
		fflush(stdout);
		pthread_cleanup_pop(0);
	}
	pthread_cancel(tid1);
	pthread_exit(NULL);
}

void* writer_thread(void *arg){
	int sock = *((int*)arg);

	int n;
	char buffer[1024];
	
	while(1){
		fgets(buffer, 1024, stdin);
		n = strlen(buffer);
		buffer[n-1] = '\0';
		send(sock, buffer, n, 0);
		if(!strcmp(buffer, "/q"))
			break;
		
	}
	pthread_cancel(tid2);
	pthread_exit(NULL);	
}
void clean_thread(void* arg){
	struct termio abs;
	if(ioctl(0, TCGETA, &abs)==-1) {perror("ioctl"); exit(1);}
	abs.c_lflag|=ECHO;
	abs.c_lflag|=ICANON;
	if(ioctl(0, TCSETAF, &abs)==-1) {perror("ioctl"); exit(1);}
}