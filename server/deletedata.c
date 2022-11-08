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
int deleteData(int sd,LPARRAY userFullList,int* chatUser){
    LPARRAY dataFullList;
    char buf[1024];
    char idUser[16];
    char fileNameSave[256];
    LPDATABOARD TempBoard;
    LPUSER user1;
    int n;
    for(int i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1){
            strcpy(idUser,user1->id);
            break;
        }
    }
    if(strcmp(user1->id,"admin")!=0){
        sprintf(buf," 해당 사용자는 삭제할 권한이 없습니다.");
        send(sd,buf,strlen(buf),0);
        sleep(2);
        send(sd, "clear!!", strlen("clear!!"), 0);
        usleep(5000);
        return 0;
    }
    while(1){
        L:
        A:
        usleep(5000);
        send(sd, "clear!!", strlen("clear!!"), 0);
        usleep(5000);
        sprintf(buf,"\n│ chocie ||  fileNO ||                          title                      \n" );
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
        for(i=0;i<Max;i++){
	        arrayGetAt((LPC_ARRAY)dataFullList,i, (LPDATA*)&TempBoard);
            sprintf(buf, "│  [%2d ] ||%6d   || %30s \n",(cnt%10+1),TempBoard->number,TempBoard->fileName);
            checkBox[cnt%10+1] = TempBoard->number;
            cnt++;
            send(sd,buf,strlen(buf),0);
            if(cnt%10==0 || i==Max-1){
                while(1){
                    int k=9-((cnt-1)%10);
                    strcpy(buf,""); //보낼 문자초기화
                    while(k){       //빈칸을 늘려 어느게시글을 자세히 보고싶으십니까 호출 일정화
                        strcat(buf,"\n");
                        k--;
                    }
                    strcat(buf, "\n\n\n\n  어느 파일을 삭제하시겠습니까?(다음자료보기: 0 종료시 :/e) : ");
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
                        sprintf(buf, "\n 잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto A;
                    }
                    if(choice<=10 &&choice>0){
                        char* fName = ((LPDATABOARD)(dataFullList->lpData[checkBox[choice]]))->fileName;
                        strcat(fileNameSave,"/");
                        strcat(fileNameSave,fName);
                        unlink(fileNameSave);
                        usleep(5000);
                        sprintf(buf," %s 파일이 클라우드에서 삭제되었습니다.",fName);
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto A;
                    }
                    else{
                        sprintf(buf, "\n 잘못고르셨습니다.");
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto L;
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
