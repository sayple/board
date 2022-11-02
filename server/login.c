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
#include "userinfo.h"
#include "input.h"
#include "array.h"
#include "total.h"
#include "load.h"

void* startMenu(void* arg)
{   fflush(stdin);
    LPSENDFILE checkPre = (LPSENDFILE) arg;
	LPMENU lpMenu;
	LPARRAY userFullList;
    int sd;
    sd = checkPre->sendSd;
    userFullList = checkPre->sendList;
	loadUserList(&userFullList);
	menuCreate(&lpMenu,"menu_two.txt");
	menuRun(lpMenu,sd,userFullList);
	menuDestroy(lpMenu);
    free(checkPre);
	return NULL;
	
}
int login(int sd,LPARRAY userFullList)
{
    int a,b,n;
	char buf[1024];
    LPUSER Temp = (LPUSER)malloc(sizeof(user));
    LPUSER TempCheck = (LPUSER)malloc(sizeof(user));
    while(1){
        while(1){
            sprintf(buf,"%s", "ID : ");
	        send(sd, buf, strlen(buf), 0);
            usleep(50000);
            n = recv(sd, buf, sizeof(buf), 0);
            if(n<16) break;
            else{
                sprintf(buf,"%s","ID 기준을 초과하셨습니다. 다시 입력하십시요 \n");
                send(sd,buf,sizeof(buf),0);
            }
        }
	    buf[n] = '\0';
	    strcpy(Temp->id,buf);
        while(1){
        sprintf(buf,"%s", "PW : ");
        send(sd, buf, strlen(buf), 0);
        usleep(50000);
        n = recv(sd, buf, sizeof(buf), 0);
	    if(n<16) break;
        else{
            sprintf(buf,"%s","PW 기준을 초과하셨습니다. 다시 입력하십시요 \n");
            send(sd,buf,sizeof(buf),0);
            }
        }
        buf[n] = '\0';
	    strcpy(Temp->pass,buf);
        int i;
        int flagA=0;
        pthread_t newOne;
        for(i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	        arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&TempCheck);
            usleep(50000);
            if(strcmp(Temp->id,TempCheck->id)==0){
                usleep(50000);
                if(strcmp(Temp->pass,TempCheck->pass)==0){
                    flagA=1;
                    TempCheck->request = 1;
                    arraySetAt(userFullList,i,TempCheck); ////어카지
                    LPSENDFILE fileGive = (LPSENDFILE)malloc(sizeof(sendFile));
                    fileGive->sendSd=sd;
                    fileGive->sendList = userFullList;
                    if(pthread_create(&newOne, NULL, startMenu,fileGive)!=0) {
			            perror("pthread_create");}
                    //pthread_detach(newOne); //아마 쓰레드 디테치 되면서 그냥 꺼지는듯...

                    pthread_join(newOne,NULL);
                }
            }

		}
        if(flagA==0)
            send(sd,"틀리셨습니다",strlen("틀리셨습니다"),0);
        if(flagA==1)
            return 0;
        continue;
        
    }
    free(Temp);
    free(TempCheck);
    
    return 0;
}
