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

int join(int sd,LPARRAY userFullList,int* chatUser)
{
    pthread_mutex_t mutexid;
    pthread_mutex_init(&mutexid,NULL);
    loadUserList(&userFullList); ////////////change 
    int n;
	char buf[1024];
    LPUSER Temp = (LPUSER)malloc(sizeof(user));
    Temp->request=0;
    fflush(stdin);
    fflush(stdout);
    LPUSER TempCheck;
        while(1){
            send(sd, "clear!!", strlen("clear!!"), 0);
            usleep(5000);
            int flag =1;
            sprintf(buf,"\n\n\n\n│  %s", "ID  : ");
	        send(sd, buf, strlen(buf), 0);
            usleep(5000); //10배늘려봄 500000
            n = recv(sd, buf, sizeof(buf), 0);
            if(n<16){
                int i=0;
                buf[n] = '\0';
                strcpy(Temp->id,buf);
                for(i=0;i<arraySize(userFullList);i++){
	                arrayGetAt(userFullList,i, (LPDATA*)&TempCheck);
                    if(strcmp(Temp->id,TempCheck->id)==0){
                        sprintf(buf,"│  %s","중복된 ID입니다. 다시 입력하십시요.\n");
                        send(sd,buf,sizeof(buf),0);
                        sleep(1);
                        break;
		            }
                }
                if(i==arraySize(userFullList)) flag=0;
            }
            else{
                sprintf(buf,"│  %s","ID 기준을 초과하셨습니다. 다시 입력하십시요.\n");
                send(sd,buf,sizeof(buf),0);
                sleep(1);
            }
            if(flag==0) break;
        }
        while(1){
        sprintf(buf,"%s", "│  PW  : ");
        send(sd, buf, strlen(buf), 0);
        usleep(50000);
        memset(buf,0,sizeof(char)*1024);
        n = recv(sd, buf, sizeof(buf), 0);
	    if(n<16) break;
        else{
            sprintf(buf,"│  %s","PW 기준을 초과하셨습니다. 다시 입력하십시요.\n");
            send(sd,buf,sizeof(buf),0);
            sleep(1);
            }
        }
        buf[n] = '\0';
	    strcpy(Temp->pass,buf);
        while(1){
            memset(buf,0,sizeof(char)*1024);
            sprintf(buf,"%s", "│  NICK  : ");
	        send(sd, buf, strlen(buf), 0);
            usleep(50000);
            n = recv(sd, buf, sizeof(buf), 0);
            if(n<16) break;
            else{
                sprintf(buf,"│  %s","닉네임 기준을 초과하셨습니다. 다시 입력하십시요.\n");
                send(sd,buf,sizeof(buf),0);
                sleep(1);
            }
        }
	    buf[n] = '\0';
	    strcpy(Temp->name,buf);
        while(1){
            sprintf(buf,"│  %s", "PHONE Number(xxx-xxxx-xxxx)  : ");
	        send(sd, buf, strlen(buf), 0);
            usleep(50000);
            n = recv(sd, buf, sizeof(buf), 0);
            if(n<16) break;
            else{
                sprintf(buf,"│  %s","전화번호 기준을 초과하셨습니다. 다시 입력하십시요.\n");
                send(sd,buf,sizeof(buf),0);
                sleep(1);
            }
        }
	    buf[n] = '\0';
	    strcpy(Temp->tellNo,buf);
    pthread_mutex_lock(&mutexid);
    arrayAdd(userFullList,(const LPDATA) Temp);
    saveUserList(userFullList);
    sprintf(buf,"%s", "clear!!");
	send(sd, buf, strlen(buf), 0);
    usleep(5000);
    sprintf(buf,"\n│  %s", "회원가입이 완료되셨습니다.\n\n");
    send(sd,buf,sizeof(buf),0);
    sleep(1);
    sprintf(buf,"%s", "clear!!");
	send(sd, buf, strlen(buf), 0);
    pthread_mutex_unlock(&mutexid);
    usleep(5000);
    
    return 0;
}
