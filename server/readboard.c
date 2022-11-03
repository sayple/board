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
int readBoard(int sd,LPARRAY userFullList){
    LPARRAY boardFulllist;
    char buf[1024];
    char idUser[16];
    LPUSER user1 = (LPUSER)malloc(sizeof(user));
    LPBOARD TempBoard = (LPBOARD)malloc(sizeof(boardNormal));
    int n;
    for(int i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1){
            strcpy(idUser,user1->id);
            break;
        }
    }
    free(user1);
    while(1){
        send(sd, "clear!!", strlen("clear!!"), 0);
        usleep(50000);
        sprintf(buf," chocie ||  textNO ||   writer  ||                                  subject                      \n" );
        send(sd,buf,strlen(buf),0);
        usleep(50000);
        sprintf(buf,"============================================================================================\n" );
        send(sd,buf,strlen(buf),0);
        loadBoardList(&boardFulllist);
        usleep(50000);
        int cnt=0;
        int flag=0;
        for(int i=arraySize((LPC_ARRAY)boardFulllist)-1;i>=0;i--){
            cnt++;
	        arrayGetAt((LPC_ARRAY)boardFulllist,i, (LPDATA*)&TempBoard);
            sprintf(buf, "  [%2d]  ||%6d   || %10s || %s\n",cnt%10,i+1,TempBoard->id,TempBoard->title);
            send(sd,buf,strlen(buf),0);
            if(cnt%10==0 || i==0){
                while(1){
                    sprintf(buf, "\n 어느 게시글을 자세히 보고 싶으십니까?(다음글보기: 0 종료시 :/e) : ");
                    send(sd,buf,strlen(buf),0);
                    usleep(50000);
                    recv(sd,buf,1024,0);
                    if(strncmp(buf,"/e",2)==0){
                        flag =1;
                        break;
                    }
                    if(strcmp(buf,"0")==0){
                        break;
                    }
                    usleep(50000);
                    int choice = atoi(buf);
                    if(choice<=10 &&choice>0){
                        //boardFulllist.LPDATA[i+10-choice]->context readdetail함수 만들기
                        send(sd,"아직 안만들었습니다\n",strlen("아직 안만들었습니다\n"),0);
                    }
                    else{
                        sprintf(buf, "\n잘못고르셨습니다.\n ");
                        continue;
                        }
                }
                send(sd, "clear!!", strlen("clear!!"), 0);
                if(i!=0){
                    usleep(50000);
                    sprintf(buf," chocie ||  textNO ||   writer  ||                                  subject                      \n" );
                    send(sd,buf,strlen(buf),0);
                    usleep(50000);
                    sprintf(buf,"============================================================================================\n" );
                    send(sd,buf,strlen(buf),0);
                }
                if(flag==1) break;
            }
        }
        if(flag==1) break;
        
    }
    free(TempBoard);
    return 0;
}