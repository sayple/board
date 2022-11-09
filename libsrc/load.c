#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include "userinfo.h"
#include "boardinfo.h"
#include "input.h"
#include "array.h"
#include "load.h"
#include "cursorCon.h"

int loadUserList(LPARRAY* userFullList)
{
	FILE* fp;
	int n;
	int nErr = arrayCreate(userFullList);
    if (ERR_ARRAY_OK != nErr) {
        exit(nErr);
    }
	char* str,*key;
	fp = fopen("userinfo.txt", "r");
	if(NULL== fp){
        perror("file open error!\n");
        exit(1);
    }
	while(1){
        str = inputLine(fp);
        if(NULL==str) break;
        LPUSER Temp = (LPUSER)malloc(sizeof(user));
        if(str[0] !=0){
            Temp->request=0;
            key = strtok(str,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->id,key);
            key = strtok(NULL,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->pass,key);
            key = strtok(NULL,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->name,key);
            key = strtok(NULL,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->tellNo,key);
        }
        arrayAdd(*userFullList,(const LPDATA) Temp);
	}
	fclose(fp);
	return 0;
}

int loadBoardList(LPARRAY* boardFullList)
{
	FILE* fp;
	int n;
	int nErr = arrayCreate(boardFullList);
    if (ERR_ARRAY_OK != nErr) {
        exit(nErr);
    }
	char* str,*key;
	fp = fopen("board_contents.txt", "r");
	if(NULL== fp){
        perror("file open error!\n");
        exit(1);
    }
	while(1){
        str = inputLine(fp);
        if(NULL==str) break;
        LPBOARD Temp = (LPBOARD)malloc(sizeof(boardNormal));
        srand(time(NULL));
        if(str[0] !=0){
            key = strtok(str,"}");
            if(key==NULL) {
                perror("title open error!\n");
                exit(1);
            }
            Temp->originNum=atoi(key);
            key = strtok(NULL,"}");
            if(key==NULL) {
                perror("title open error!\n");
                exit(1);
            }
            strcpy(Temp->title,key);
            key = strtok(NULL,"}");
            if(key==NULL) {
                perror("context open error!\n");
                exit(1);
            }
            strcpy(Temp->context,key);
            key = strtok(NULL,"}");
            if(key==NULL) {
                perror("key open error!\n");
                exit(1);
            }
            strcpy(Temp->id,key);
            key = strtok(NULL,"}");
            if(key==NULL) {
                perror("id open error!\n");
                exit(1);
            }
            strcpy(Temp->nick,key);
            key = strtok(NULL,"}");
            strcpy(Temp->reply,key);

        }
        arrayAdd(*boardFullList,(const LPDATA) Temp);
	}
	fclose(fp);
	return 0;
}

int loadDataBoardList(LPARRAY* dataFullList,const char* wayToFile)
{
	int n;
    DIR* dirPoint;
    struct dirent* dirFile;
    
	int nErr = arrayCreate(dataFullList);
    if (ERR_ARRAY_OK != nErr) {
        return 1;
    }
	dirPoint = opendir(wayToFile);
    n=0;
	if(dirPoint!=NULL){
        while(dirFile=readdir(dirPoint)){
            LPDATABOARD Temp = (LPDATABOARD)malloc(sizeof(dataBoard));
            if(strcmp(dirFile->d_name,".")==0||strcmp(dirFile->d_name,"..")==0) continue;
            strcpy(Temp->fileName,dirFile->d_name);
            Temp->number = n++;
            arrayAdd(*dataFullList,(const LPDATA) Temp);
        }
    }
    else{
        fprintf(stderr, "%s directory 정보를 읽을 수 없습니다.\n", wayToFile);
        return 1;
    }
	closedir(dirPoint);
	return 0;
}


int saveUserList(LPARRAY userFullList){
    pthread_mutex_t mutexid;
    pthread_mutex_init(&mutexid,NULL);
    pthread_mutex_lock(&mutexid);
    LPUSER newTemp;
    FILE* fp;
    char buff[100];
    fp = fopen("userinfo.txt","w");
    for(int i=0;i<arraySize(userFullList);i++){
        arrayGetAt(userFullList,i,(LPDATA*)&newTemp);
        sprintf(buff,"%s/%s/%s/%s\n",newTemp->id,newTemp->pass,\
        newTemp->name,newTemp->tellNo);
        fwrite(buff,1,strlen(buff),fp);
    }
    fclose(fp);
    pthread_mutex_unlock(&mutexid);

    return 0;
}

int saveBoardList(LPBOARD temp,LPARRAY boardFullList){
    pthread_mutex_t mutexid;
    pthread_mutex_init(&mutexid,NULL);
    pthread_mutex_lock(&mutexid);
    LPBOARD newTemp;
    arrayAdd(boardFullList,(LPDATA)temp);
    FILE* fp;
    char buff[4980];
    fp = fopen("board_contents.txt","w");
    for(int i=0;i<arraySize(boardFullList);i++){
        arrayGetAt(boardFullList,i,(LPDATA*)&newTemp);
        sprintf(buff,"%d}%s}%s}%s}%s}%s\n",newTemp->originNum,newTemp->title,newTemp->context,\
        newTemp->id,newTemp->nick,newTemp->reply);
        fwrite(buff,1,strlen(buff),fp);
    }
    fclose(fp);
    arrayDestroy(boardFullList);
    pthread_mutex_unlock(&mutexid);
    return 0;
}

int saveBoard(LPARRAY boardFullList){
    pthread_mutex_t mutexid;
    pthread_mutex_init(&mutexid,NULL);
    pthread_mutex_lock(&mutexid);
    LPBOARD newTemp;
    FILE* fp;
    char buff[4980];
    fp = fopen("board_contents.txt","w");
    
    for(int i=0;i<arraySize(boardFullList);i++){
        arrayGetAt(boardFullList,i,(LPDATA*)&newTemp);
        sprintf(buff,"%d}%s}%s}%s}%s}%s\n",newTemp->originNum,newTemp->title,newTemp->context,\
        newTemp->id,newTemp->nick,newTemp->reply);
        fwrite(buff,1,strlen(buff),fp);
    }
    fclose(fp);
    //arrayDestroy(dataBoardFullList);
    pthread_mutex_unlock(&mutexid);
    return 0;
}

char* loadID(LPARRAY userFullList){
    LPARRAY boardFulllist;
    char* userName = (char*)malloc(sizeof(char)*16);
    LPUSER user1;
    int i;
    for(i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1) break;
    }
    if(user1->request==1){
        strcpy(userName,user1->id);
        return userName;
    }
    if(i==arraySize((LPC_ARRAY)userFullList)){
        return NULL;
    }
}

char* loadName(LPARRAY userFullList){
    LPARRAY boardFulllist;
    char* userName = (char*)malloc(sizeof(char)*16);
    LPUSER user1;
    int i;
    for(i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1) break;
    }
    if(user1->request==1){
        strcpy(userName,user1->name);
        return userName;
    }
    if(i==arraySize((LPC_ARRAY)userFullList)){
        return NULL;
    }
}
void loadText(int sd,int originNumber,char* id,char* nickName, LPARRAY boardFullList){
    time_t now;
    struct tm* timeInfo;
    char timeWord[128];
    int i,n;
    char* key;
    char buf[4096];
    char buff[4231];
    int countRow;
    int deleteFlag = 0;
    int replyFlag =0;
    int enterFlag=0;
    LPBOARD newTemp,newTempReply;
    while(1){
        loadBoardList(&boardFullList);
        for(i=0;i<arraySize((LPC_ARRAY)boardFullList);i++){
	        arrayGetAt((LPC_ARRAY)boardFullList,i, (LPDATA*)&newTemp);
            if(newTemp->originNum ==originNumber){
                enterFlag=1;
                countRow=0;
                send(sd, "clear!!", strlen("clear!!"), 0);
                if(strcmp(id,newTemp->id)==0 ||strcmp(id,"admin")==0) deleteFlag=1;
                usleep(5000);
                if(deleteFlag==1){
                    sprintf(buf, "\n│ 작성자 : %s ",newTemp->nick);
                    strcat(buf,"      [삭제가능 : 단축키 /dd ]");
                    send(sd, buf, strlen(buf), 0);
                }
                else{
                    sprintf(buf,"\n│ 작성자 : %s      ",newTemp->nick);
                    send(sd, buf, strlen(buf), 0);
                }
                usleep(5000);
                sprintf(buf,"%s","\n│======================================================================================================================================\n");
                send(sd, buf, strlen(buf), 0);
                strcpy(buf,newTemp->context);
                countRow++;
                key = strtok(buf,"|");
                if(key==NULL) break;
                sprintf(buff,"│ %s\n",key);
                send(sd,buff,strlen(buff),0);
                while(key=strtok(NULL,"|")){
                    countRow++;
                    usleep(5000);
                    sprintf(buff,"│ %s\n",key);
                    send(sd,buff,strlen(buff),0);
                }
                usleep(5000);
                sprintf(buf,"%s","│======================================================================================================================================\n");
                send(sd, buf, strlen(buf), 0);
                usleep(5000);
                strcpy(buf,newTemp->reply);
                key = strtok(buf,"|");
                countRow++;
                if(key==NULL) break;
                sprintf(buff,"│ ──── %s ────\n",key);
                send(sd,buff,strlen(buff),0);
                while(key=strtok(NULL,"|")){
                    countRow++;
                    usleep(5000);
                    sprintf(buff,"│ %s\n",key);
                    send(sd,buff,strlen(buff),0);
                }
                
                usleep(5000);
                V:
                strcpy(buf,"");
                while(countRow<=12){
                    strcat(buf,"\n");
                    countRow++;
                }
                strcat(buf,"  리플을 다시겠습니까(최대 80자) : (/r) (나가기 /e) : ");
                send(sd,buf,strlen(buf),0);
                usleep(5000);
                n = recv(sd, buf, 512, 0);
                if(strcmp(buf,"/r")==0){
                    usleep(5000);
                    sprintf(buf,"%s : ", nickName);
                    send(sd,buf,strlen(buf),0);
                    usleep(5000);
                    n = recv(sd, buf, 256, 0);
                    if(n>80){
                        send(sd,"리플을 초과하였습니다. 다시 작성해주세요.\n",strlen("리플을 초과하였습니다. 다시 작성해주세요.\n"),0);
                        goto V;
                    }
                    loadBoardList(&boardFullList);
                    int flagToRepl=0;
                    for(i=0;i<arraySize((LPC_ARRAY)boardFullList);i++){
	                    arrayGetAt((LPC_ARRAY)boardFullList,i, (LPDATA*)&newTempReply);
                        if(newTempReply->originNum ==originNumber){
                            if(strlen(newTempReply->reply)+strlen(buf)>=3900){
                                send(sd,"게시글에 최대리플을 초과하였습니다.\n",strlen("게시글에 최대리플을 초과하였습니다.\n"),0);
                                sleep(1);
                                return;
                            }
                            flagToRepl=1;
                            time(&now);
                            timeInfo = localtime(&now);
                            sprintf(timeWord,"%s",asctime(timeInfo));
                            timeWord[strlen(timeWord)-1]='\0';
                            sprintf(buff, " %s : %s - %s|",nickName,buf,timeWord);
                            strcat(newTempReply->reply,buff);
                            break;
                        }
                    }
                    if(flagToRepl==0){
                        sprintf(buff, " 삭제된 글입니다.\n");
                        send(sd,buff,strlen(buff),0);
                        sleep(1);
                        return;
                    }
                    saveBoard(boardFullList);
                }
                else if((strcmp(buf,"/dd")==0)&&(deleteFlag==1)){
                    arrayRemoveAt(boardFullList, i);
                    saveBoard(boardFullList);//////
                    return;
                }
                else if(strcmp(buf,"/e")==0){
                    return;
                    }
                else{
                    sprintf(buff, " 잘못된 입력입니다.\n");
                    send(sd,buff,strlen(buff),0);
                    sleep(1);
                }
            
            usleep(5000);
        }
        }
        if(enterFlag==0){   //글이 삭제된 경우 읽지 않는 처리
            sprintf(buff, " 삭제된 글입니다.\n");
            send(sd,buff,strlen(buff),0);
            sleep(1);
            return;
        }
	}
}
