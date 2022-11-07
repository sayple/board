#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "menu.h"
#include "total.h"
#include "userinfo.h"
#define CHATMEM 30
typedef struct{
    char nick[16];
    int sd;
    int* chatMem;  //전역변수로 선언해둔 챗맴버
}chat,*LPCHAT;

void* chatProgramWrite(void* arg){
	int n;
	char buf[256];
    char toBuf[300];
	LPCHAT chatData = (LPCHAT) arg;
    char* nickName = chatData->nick;
    int sd = chatData->sd;
    int * userArray = chatData->chatMem;
    int check=0;
    usleep(5000);
    send(sd, "startChaTtiNg!!", strlen("startChaTtiNg!!"), 0);
    usleep(5000);
    sprintf(buf, "%s 님이 입장하셨습니다.",nickName);
    usleep(5000);
    while(userArray[check]!=-1){
        send(userArray[check],buf,strlen(buf),0);
        check++;
    }
    usleep(5000);
    sprintf(buf, "/e를 누르면 종료됩니다.");
    send(sd,buf,strlen(buf),0);
    usleep(5000);
	while(1){
        usleep(5000);

        n = recv(sd,buf,256,0);
        if(n <= 0){
			printf("\n서버 연결 끊김\n");
			break;
		}
        buf[n-1] = '\0';
        check=0;
        if(!strcmp(buf,"/e")){
            sprintf(buf,"%s 님이 나가셨습니다.",nickName);
            send(sd, "goutMyyarddddd!!", strlen("goutMyyarddddd!!"), 0);
            while(userArray[check]!=-1){
                if(userArray[check]!=sd)
                    send(userArray[check],buf,strlen(buf),0);
                    check++;
            }
            check=0;
            while(userArray[check]!=-1){
                if(userArray[check]==sd){
                    memcpy(&userArray[check],&userArray[check+1],(buf,CHATMEM-check-1)*sizeof(int));
                    break;
                }
                check++;
            }
            send(sd, "clear!!", strlen("clear!!"), 0);
            usleep(5000);
            break;
        }
        sprintf(toBuf,"%s : %s\n",nickName,buf);
        check=0;
        while(userArray[check]!=-1){
            send(userArray[check],toBuf,strlen(toBuf),0);
            check++;
        }
    }
    return NULL;
}


int chatting(int sd,LPARRAY userFullList,int* chatUser){
    int sockFd;
    LPUSER user1;
    LPCHAT userChatting = (LPCHAT)malloc(sizeof(chat));
    for(int i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1){
            strcpy(userChatting->nick,user1->name);
            break;
        }
    }
    userChatting->sd = sd;
    int n=0;
    while(chatUser[n]!=-1){
        n++;
    }
    chatUser[n] = sd;
    userChatting->chatMem=chatUser;
	chatProgramWrite(userChatting);
    free(userChatting);
    send(sd, "clear!!", strlen("clear!!"), 0);
    usleep(5000);
	return 0;
	}
