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
        if(user1->request ==1) break;
    }
    strcpy(TempBoard->id,user1->id);
    strcpy(TempBoard->reply,"");
    srand(time(NULL));
    n=rand()%50000000;
    TempBoard->originNumber=n;
    loadBoardList(&boardFulllist);
    while(1){
        sprintf(buf,"%s", "clear!!");
	    send(sd, buf, strlen(buf), 0);
        usleep(50000);
        sprintf(buf,"%s", "제목  : ");
	    send(sd, buf, strlen(buf), 0);
        usleep(50000);
        n = recv(sd, buf, sizeof(buf), 0);
        usleep(50000);
        if(n>80){
            sprintf(buf,"%s", "80자를 초과하였습니다");
	        send(sd, buf, strlen(buf), 0);
        }
        else break;
    }
    buf[n] = '\0';
    strcpy(TempBoard->title,buf);
    while(1){
        sprintf(buf,"%s", "내용\n");
	    send(sd, buf, strlen(buf), 0);
        usleep(50000);
        sprintf(buf,"%s","========================================================\n\n");
        send(sd, buf, strlen(buf), 0);
        usleep(50000);
        n = recv(sd, buf, sizeof(buf), 0);
        usleep(50000);
        if(n>500){
	        buf[501] ='\0';
            strcpy(TempBoard->context,buf);
            usleep(50000);
            sprintf(buf,"%s", "500자를 초과하여 일부 내용만 저장합니다.\n");
            send(sd, buf, strlen(buf), 0);
        }
        else{
            buf[n] = '\0';
            strcpy(TempBoard->context,buf);
        }       
    }
    arrayAdd(boardFulllist,TempBoard);
    free(user1);
    return 0;
}