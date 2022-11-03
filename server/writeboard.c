#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "menu.h"
#include "input.h"
#include "array.h"
#include "total.h"
#include "load.h"
#include "boardinfo.h"
#include "userinfo.h"
#include <time.h>

int writeBoard(int sd,LPARRAY userFullList){
    LPARRAY boardFulllist;
    char buf[1024];
    LPUSER user1 = (LPUSER)malloc(sizeof(user));
    LPBOARD TempBoard = (LPBOARD)malloc(sizeof(boardNormal));
    int n;
    for(int i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1){
            strcpy(TempBoard->id,user1->id);
            break;
        }
    }
    free(user1);
    strcpy(TempBoard->reply,"리플목록");
    srand(time(NULL));
    n=rand()%50000000;
    TempBoard->originNumber=n;
    while(1){
        usleep(50000);
        sprintf(buf,"%s", "clear!!");
	    send(sd, buf, strlen(buf), 0);
        usleep(50000);
        sprintf(buf,"%s", "제목  : ");
	    send(sd, buf, strlen(buf), 0);
        usleep(50000);
        n = recv(sd, buf, 1024, 0);
        usleep(50000);
        if(n>80){
            sprintf(buf,"%s", "80자를 초과하였습니다");
	        send(sd, buf, strlen(buf), 0);
        }
        else break;
    }
    buf[n-1] = '\0';
    if(strcmp(buf,"")==0){
        strcpy(TempBoard->title,"  ");
    }
    else
        strcpy(TempBoard->title,buf);
    strcpy(TempBoard->context,""); //초기화
    sprintf(buf,"%s","========================================================\n");
	send(sd, buf, strlen(buf), 0);
    usleep(5000);
    sprintf(buf,"%s", "내용(마지막에 '/w'를 넣을 시 전송됩니다)\n");
    send(sd, buf, strlen(buf), 0);
    int totalN=0;
     usleep(5000);
    sprintf(buf,"%s","========================================================\n\n");
    send(sd, buf, strlen(buf), 0);
    while(1){
        usleep(50000);
        n = recv(sd, buf, 1024, 0);
        usleep(50000);
        totalN+=n;
        if(totalN>500){
            strcpy(TempBoard->context,buf);
            TempBoard->context[501]='\0';
            usleep(50000);
            sprintf(buf,"%s", "500자를 초과하여 일부 내용만 저장합니다.\n");
            send(sd, buf, strlen(buf), 0);
            break;
        }
        else if(buf[n-2]=='w'&&buf[n-3]=='/'){
            buf[n-3] = '\0';
            strcat(TempBoard->context,buf);
            break;
        }
        else{
            strcat(TempBoard->context,"|");
            strcat(TempBoard->context,buf);
        }          
    }
    if(strcmp(TempBoard->context,"")==0){
        strcpy(TempBoard->context,"  ");
    }
    loadBoardList(&boardFulllist);
    sprintf(buf,"%s %s %s\n",TempBoard->title,TempBoard->context,TempBoard->id);
    send(sd,buf,strlen(buf),0);
    usleep(50000);
    saveBoardList(TempBoard,boardFulllist);
    usleep(50000);
    arrayDestroy(boardFulllist);
    send(sd, "clear!!", strlen("clear!!"), 0);
    return 0;
}