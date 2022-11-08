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
int findBoard(int sd,LPARRAY userFullList,int* chatUser){
    LPARRAY boardFulllist;
    char buf[1024];
    char idUser[16];
    char idNickName[16];
    char findWord[1024];
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
    V:
    send(sd, "clear!!", strlen("clear!!"), 0);
    usleep(5000);
    sprintf(buf, "\n│ 찾고 싶은 제목을 검색해주세요 : (20자 이내) : ");
    send(sd,buf,strlen(buf),0);
    usleep(5000);
    n=recv(sd,findWord,1024,0);
    if(n>20){
        sprintf(buf, "\n│ 초과하셨습니다. 다시 검색부탁드립니다.");
        send(sd,buf,strlen(buf),0);
        sleep(1);
        goto V;
    }
    usleep(5000);
    while(1){
        M: 
        send(sd, "clear!!", strlen("clear!!"), 0);
        usleep(5000);
        sprintf(buf,"\n│ chocie ||  textNO ||   writer  ||            title\n" );
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
            strcpy(buf,TempBoard->reply);
            char* key = strtok(buf,"|");
            int repleNum=0;
            if(key==NULL){perror("reple read error!");}
            while(key=strtok(NULL,"|")){
                repleNum++;
            }
            if(strstr(TempBoard->title,findWord)!=NULL){
                sprintf(buf, "│  [%2d ] ||%6d   || %6s    || %s[%d]\n",(cnt%10+1),i+1,TempBoard->id,TempBoard->title,repleNum);
                send(sd,buf,strlen(buf),0);
                checkBox[cnt%10+1] = i;
                cnt++;
            }
            if(cnt==0 && i==0){
                send(sd, "clear!!", strlen("clear!!"), 0);
                usleep(5000);
                sprintf(buf,"\n│ 찾는 내용이 존재하지 않습니다.");
                send(sd,buf,strlen(buf),0);
                sleep(1);
                send(sd, "clear!!", strlen("clear!!"), 0);
                usleep(5000);
                return 0;
            }
            if(cnt!=0 && (cnt%10==0 || i==0)){
                while(1){
                    int k=9-((cnt-1)%10);
                    strcpy(buf,""); //보낼 문자초기화
                    while(k){       //빈칸을 늘려 어느게시글을 자세히 보고싶으십니까 호출 일정화
                        strcat(buf,"\n");
                        k--;
                    }
                    strcat(buf, "\n\n\n\n  어느 게시글을 자세히 보고 싶으십니까?(다음글보기: 0  종료시: /e) : ");
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    recv(sd,buf,1024,0);
                    if(strncmp(buf,"/e",2)==0){
                        usleep(5000);
                        send(sd, "clear!!", strlen("clear!!"), 0);
                        usleep(5000);
                        saveBoard(boardFulllist); //여기 다른점
                        return 0;
                    }
                    if(strcmp(buf,"0")==0){
                        break;
                    }
                    int choice = atoi(buf);
                    if(i==0 && choice>(cnt-1)%10+1){
                        sprintf(buf, "\n잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto M;
                    }
                    usleep(5000);
                    if(choice<=10 &&choice>0){
                        ((LPBOARD)boardFulllist->lpData[checkBox[choice]])->request =1;
                        loadText(sd,idUser,idNickName,boardFulllist);
                        usleep(5000);
                        ((LPBOARD)boardFulllist->lpData[checkBox[choice]])->request =0;
                        usleep(5000);
                        saveBoard(boardFulllist);
                        usleep(5000);
                        goto M;
                    }
                    else{
                        sprintf(buf, "\n잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto M;
                        }
                }
                send(sd, "clear!!", strlen("clear!!"), 0);
                if(i!=0){
                    usleep(5000);
                    sprintf(buf,"\n│ chocie ||  textNO ||   writer  ||            title\n" );
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    sprintf(buf,"%s","│======================================================================================================================================\n");
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                }
            }
        }
        
    }
    arrayDestroy(boardFulllist);
    return 0;
}