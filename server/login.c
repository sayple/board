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
    int* chatUser;
    sd = checkPre->sendSd;
    send(sd, "clear!!", strlen("clear!!"), 0);
    userFullList = checkPre->sendList;
    chatUser = checkPre->chatArray;
    usleep(5000);
	menuCreate(&lpMenu,"menu_two.txt");
	menuRun(lpMenu,sd,userFullList,chatUser);
	menuDestroy(lpMenu);
    free(checkPre);
    send(sd, "clear!!", strlen("clear!!"), 0);
    usleep(5000);
	return NULL;
	
}
int login(int sd,LPARRAY userFullList,int* chatUser)
{
    loadUserList(&userFullList);
    send(sd, "clear!!", strlen("clear!!"), 0);
    usleep(5000);
    int a,b,n;
	char buf[1024];
    LPUSER Temp = (LPUSER)malloc(sizeof(user));
    LPUSER TempCheck;
    while(1){
        while(1){
            usleep(5000);
            send(sd, "clear!!", strlen("clear!!"), 0);
            usleep(5000);
            sprintf(buf,"\n\n\n\n\n\n\n│%68s", "ID : ");
	        send(sd, buf, strlen(buf), 0);
            usleep(5000);
            n = recv(sd, buf, 1024, 0);
            if(n<16) break;
            else{
                sprintf(buf,"\n│  %104s","ID 기준을 초과하셨습니다. 다시 입력하십시요 \n");
                send(sd,buf,strlen(buf),0);
            }
        }
	    buf[n] = '\0';
	    strcpy(Temp->id,buf);
        while(1){
        sprintf(buf,"│%68s", "PW : ");
        send(sd, buf, strlen(buf), 0);
        usleep(5000);
        n = recv(sd, buf, 1024, 0);
	    if(n<16) break;
        else{
            sprintf(buf,"\n│  %104s","PW 기준을 초과하셨습니다. 다시 입력하십시요 \n");
            send(sd,buf,strlen(buf),0);
            }
        }
        buf[n] = '\0';
	    strcpy(Temp->pass,buf);
        int i;
        int flagA=0;
        pthread_t newOne;
        for(i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	        arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&TempCheck);
            usleep(5000);
            if(strcmp(Temp->id,TempCheck->id)==0){
                usleep(5000);
                if(strcmp(Temp->pass,TempCheck->pass)==0){
                    flagA=1;
                    TempCheck->request =1;
                    break;
                }
            }
        }
		
        if(flagA==0){
            sprintf(buf,"\n│%80s","틀리셨습니다.");
            send(sd,buf,strlen(buf),0);
            sleep(1);
        }
        if(flagA==1){

            LPSENDFILE fileGive = (LPSENDFILE)malloc(sizeof(sendFile));
            fileGive->sendSd=sd;
            fileGive->sendList = userFullList;
            fileGive->chatArray = chatUser;
            // if(pthread_create(&newOne, NULL, startMenu,fileGive)!=0) {
			//     perror("pthread_create");}
            // pthread_join(newOne,NULL);
            startMenu(fileGive); ///c추가한것 
            break;
        } 
        continue;
        
    }
    free(Temp);
    
    return 0;
}
