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
int download(int sd,LPARRAY userFullList,int* chatUser){
    LPARRAY dataFullList;
    char buf[1024];
    char idUser[16];
    char idNickName[16];
    char fileNameSave[256];
    LPUSER user1;
    LPDATABOARD TempBoard;
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
        A:
        usleep(5000);
        send(sd, "clear!!", strlen("clear!!"), 0);
        usleep(5000);
        sprintf(buf,"\nā chocie ||  fileNO ||                          title                      \n" );
        send(sd,buf,strlen(buf),0);
        usleep(5000);
        sprintf(buf,"%s","ā======================================================================================================================================\n");
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
            sprintf(buf, "ā  [%2d ] ||%6d   || %30s \n",(cnt%10+1),TempBoard->number,TempBoard->fileName);
            checkBox[cnt%10+1] = TempBoard->number;
            cnt++;
            send(sd,buf,strlen(buf),0);
            if(cnt%10==0 || i==Max-1){
                while(1){
                    int k=9-((cnt-1)%10);
                    strcpy(buf,""); //ė³“ė¼ ė¬øģģ“źø°ķ
                    while(k){       //ė¹ģ¹øģ ėė ¤ ģ“ėź²ģźøģ ģģøķ ė³“ź³ ģ¶ģ¼ģ­ėź¹ ķøģ¶ ģ¼ģ ķ
                        strcat(buf,"\n");
                        k--;
                    }
                    strcat(buf, "\n\n\n\n  ģ“ė ķģ¼ģ ė¤ģ“ė°ģ¼ģź² ģµėź¹?(ė¤ģģė£ė³“źø°: 0  ģ¢ė£ģ: /e) : ");
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
                        sprintf(buf, "\n ģėŖ»ź³ ė„“ģØģµėė¤.");
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
                        sprintf(buf, "\n ģėŖ»ź³ ė„“ģØģµėė¤.");
                        send(sd,buf,strlen(buf),0);
                        sleep(1);
                        goto A;
                        }
                }
                send(sd, "clear!!", strlen("clear!!"), 0);
                if(i!=arraySize((LPC_ARRAY)dataFullList)){
                    usleep(5000);
                    sprintf(buf,"\nā chocie ||  fileNO ||                          title                      \n" );
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    sprintf(buf,"%s","ā======================================================================================================================================\n");
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                }
                
            }
        }
        
    }
    arrayDestroy(dataFullList);
    
    return 0;
}
