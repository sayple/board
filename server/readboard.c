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
int readBoard(int sd,LPARRAY userFullList,int* chatUser){
    LPARRAY boardFulllist;
    char buf[1024];
    char idUser[16];
    char idNickName[16];
    LPUSER user1;
    LPBOARD TempBoard;
    int n;
    for(int i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1){
            strcpy(idUser,user1->id);
            strcpy(idNickName,user1->name);
            break;
        }
    }
    while(1){
        M:
        send(sd, "clear!!", strlen("clear!!"), 0);
        usleep(5000);
        sprintf(buf,"\n│ chocie ||  textNO ||   writer  ||            subject\n" );
        send(sd,buf,strlen(buf),0);
        usleep(5000);
        sprintf(buf,"%s","│======================================================================================================================================\n");
        send(sd,buf,strlen(buf),0);
        loadBoardList(&boardFulllist);
        usleep(5000);
        int cnt=0;
        int checkBox[11] ={0,};
        if(arraySize((LPC_ARRAY)boardFulllist)==0){
            sprintf(buf,"│ 게시글이 없습니다.\n" );
            send(sd,buf,strlen(buf),0);
            sleep(1);
            send(sd, "clear!!", strlen("clear!!"), 0);
            usleep(5000);
            return 1;
        }
        int i=arraySize((LPC_ARRAY)boardFulllist)-1;
        for(;i>=0;i--){

	        arrayGetAt((LPC_ARRAY)boardFulllist,i, (LPDATA*)&TempBoard);
            sprintf(buf, "│  [%2d ] ||%6d   || %6s    || %s\n",(cnt%10+1),i+1,TempBoard->id,TempBoard->title);
            checkBox[cnt%10+1] = i;
            cnt++;
            send(sd,buf,strlen(buf),0);
            if(cnt%10==0 || i==0){
                while(1){
                    int k=9-((cnt-1)%10);
                    strcpy(buf,""); //보낼 문자초기화
                    while(k){       //빈칸을 늘려 어느게시글을 자세히 보고싶으십니까 호출 일정화
                        strcat(buf,"\n");
                        k--;
                    }
                    strcat(buf, "\n\n\n\n  어느 게시글을 자세히 보고 싶으십니까?(다음글보기: 0 종료시 :/e) : ");
                    usleep(5000);
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    recv(sd,buf,1024,0);
                    if(strncmp(buf,"/e",2)==0){
                        usleep(5000);
                        send(sd, "clear!!", strlen("clear!!"), 0);
                        usleep(5000);
                        return 0;
                    }
                    if(strcmp(buf,"0")==0){
                        break;
                    }
                    int choice = atoi(buf);
                    if(i==0&&choice>(cnt-1)%10+1){
                        printf("%d\n",cnt);
                        sprintf(buf, "잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        continue;
                    }
                    else if(choice<=10 &&choice>0){
                        ((LPBOARD)boardFulllist->lpData[checkBox[choice]])->request =1;
                        loadText(sd,idUser,idNickName,boardFulllist);
                        usleep(5000);
                        saveBoard(boardFulllist);
                        goto M;
                    }
                    else{
                        sprintf(buf, "잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        continue;
                        }
                }
                send(sd, "clear!!", strlen("clear!!"), 0);
                if(i!=0){
                    usleep(5000);
                    sprintf(buf,"\n│ chocie ||  textNO ||   writer  ||            subject\n" );
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    sprintf(buf,"%s","\n│======================================================================================================================================\n");
                    send(sd,buf,strlen(buf),0);
                }
                
            }
        }
        
    }
    arrayDestroy(boardFulllist);
    return 0;
}
