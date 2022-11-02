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
        for(i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	        arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&TempCheck);
            usleep(50000);
            if(strcmp(Temp->id,TempCheck->id)==0){
                usleep(50000);
                if(strcmp(Temp->pass,TempCheck->pass)==0){
                    
                    
                    flagA=1;

                }
            }

		}
        if(flagA==0)
            send(sd,"틀리셨습니다",strlen("틀리셨습니다"),0);
        continue;
        
    }
    free(Temp);
    free(TempCheck);
    
    return 0;
}
