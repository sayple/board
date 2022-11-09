#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include "input.h"
#include "array.h"
#include "total.h"
#include "load.h"
#include "boardinfo.h"
#include "userinfo.h"
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAXSIZE 128
int findData(int sd,LPARRAY userFullList,int* chatUser){
    LPARRAY dataFullList;
    char buf[1024];
    char findWord[32];
    char fileNameSave[256];
    LPDATABOARD TempBoard;
    int n;
    while(1){
        A:
        usleep(5000);
        send(sd, "clear!!", strlen("clear!!"), 0);
        usleep(5000);
        sprintf(buf, "\n│ 찾고 싶은 파일을 검색해주세요 : (20자 이내) : ");
        send(sd,buf,strlen(buf),0);
        usleep(5000);
        n=recv(sd,findWord,21,0);
        usleep(5000);
        if(n>20){
            sprintf(buf, "\n│ 초과하셨습니다. 다시 검색부탁드립니다.");
            send(sd,buf,strlen(buf),0);
            sleep(1);
            goto A;
        }
        if(strcmp(findWord,"/e")==0){
            send(sd, "clear!!", strlen("clear!!"), 0);
            usleep(5000);
            return 0;
        }
        sprintf(buf,"│ chocie ||  fileNO ||                          title                      \n" );
        send(sd,buf,strlen(buf),0);
        usleep(5000);
        sprintf(buf,"%s","│======================================================================================================================================\n");
        send(sd,buf,strlen(buf),0);
        sprintf(fileNameSave,"cloud");
        loadDataBoardList(&dataFullList,fileNameSave);
        usleep(5000);
        int cnt=0;
        int flag=0;
        int checkBox[11] ={0,};
        int Max=arraySize((LPC_ARRAY)dataFullList);
        int i;
        int checkTen=10;
        for(i=0;i<Max;i++){
	        arrayGetAt((LPC_ARRAY)dataFullList,i, (LPDATA*)&TempBoard);
            if(strstr(TempBoard->fileName,findWord)!=NULL){
                sprintf(buf, "│  [%2d ] ||%6d   || %30s \n",(cnt%10+1),TempBoard->number,TempBoard->fileName);
                checkBox[cnt%10+1] = TempBoard->number;
                cnt++;
                send(sd,buf,strlen(buf),0);
            }
            if(cnt==0 && i==Max-1){
                send(sd, "clear!!", strlen("clear!!"), 0);
                usleep(5000);
                sprintf(buf,"\n│ 찾는 파일명이 존재하지 않습니다.");
                send(sd,buf,strlen(buf),0);
                sleep(1);
                send(sd, "clear!!", strlen("clear!!"), 0);
                usleep(50000);
                return 0;
            }
            if(cnt!=0 && (cnt%10==0 || i==Max-1)){
                if(i!=Max-1){
                        if(cnt==checkTen) checkTen+=10;
                        else continue;
                    }
                else if(i==Max-1&&checkTen-10==cnt) goto A;
                while(1){
                    int k=9-((cnt-1)%10);
                    strcpy(buf,""); //보낼 문자초기화
                    while(k){       //빈칸을 늘려 어느게시글을 자세히 보고싶으십니까 호출 일정화
                        strcat(buf,"\n");
                        k--;
                    }
                    strcat(buf, "\n\n\n\n  어느 파일을 다운받으시겠습니까?(다음자료보기: 0  종료시: /e) : ");
                    usleep(5000);
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    recv(sd,buf,1024,0);
                    if(strncmp(buf,"/e",2)==0){
                        usleep(5000);
                        send(sd, "clear!!", strlen("clear!!"), 0);
                        usleep(5000);
                        flag =1;
                        return 0;
                    }
                    if(strcmp(buf,"0")==0){
                        break;
                    }
                    int choice = atoi(buf);
                    if(choice>(cnt-1)%10+1){
                        sprintf(buf, "\n│ 잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto A;
                    }
                    if(choice<=10 &&choice>0){
                        int fs,sread;
                        char* fName = ((LPDATABOARD)(dataFullList->lpData[checkBox[choice]]))->fileName;
                        strcat(fileNameSave,"/");
                        strcat(fileNameSave,fName);
                        send(sd,"__FiLeOpEn__",strlen("__FiLeOpEn__"),0);
                        usleep(5000);
                        send(sd,fName,strlen(fName),0);
                        usleep(5000);
                        if((fs=open(fileNameSave,O_RDONLY)) ==0){
                            perror("file open error!");
                            close(fs);
                            return 1;
                        }
                        char buffDown[MAXSIZE];
                        while(1){
                            memset(buffDown,0,MAXSIZE);
                            sread= read(fs,buffDown,MAXSIZE);
                            send(sd,buffDown,sread,0);
                            if(sread<=0) break; 
                        }
                        close(fs);
                        usleep(50000);
                        send(sd,"end",strlen("end"),0);
                        sleep(2);
                        goto A;
                    }
                    else{
                        sprintf(buf, "\n│ 잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto A;
                        }
                }
                send(sd, "clear!!", strlen("clear!!"), 0);
                if(i!=arraySize((LPC_ARRAY)dataFullList)){
                    usleep(5000);
                    sprintf(buf,"\n│ chocie ||  fileNO ||                          title                      \n" );
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    sprintf(buf,"%s","│======================================================================================================================================\n");
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                }
                
            }
        }
        
    }
    arrayDestroy(dataFullList);
    
    return 0;
}
