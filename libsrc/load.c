#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "userinfo.h"
#include "boardinfo.h"
#include "input.h"
#include "array.h"
#include "load.h"

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
        Temp->request=0;
        if(str[0] !=0){
            key = strtok(str,"}");
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
                perror("origin open error!\n");
                exit(1);
            }
            Temp->originNumber = atoi(key);
            key = strtok(NULL,"}");
            strcpy(Temp->reply,key);

        }
        arrayAdd(*boardFullList,(const LPDATA) Temp);
	}
	fclose(fp);
	return 0;
}

int loadDataBoardList(LPARRAY* dataFullList)
{
	FILE* fp;
	int n;
	int nErr = arrayCreate(dataFullList);
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
        LPDATABOARD Temp = (LPDATABOARD)malloc(sizeof(dataBoard));
        if(str[0] !=0){
            key = strtok(str,"}");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->fileName,key);
            key = strtok(NULL,"}");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->Data,key);
            key = strtok(NULL,"}");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->id,key);
            key = strtok(NULL,"}");
        }
        arrayAdd(*dataFullList,(const LPDATA) Temp);
	}
	fclose(fp);
	return 0;
}


int saveUserList(LPARRAY userFullList){
    pthread_mutex_t mutexid;
    pthread_mutex_init(&mutexid,NULL);
    pthread_mutex_lock(&mutexid);
    LPUSER newTemp = (LPUSER)malloc(sizeof(user));
    FILE* fp;
    char buff[100];
    fp = fopen("userinfo.txt","w");
    for(int i=0;i<arraySize(userFullList);i++){
        arrayGetAt(userFullList,i,(LPDATA*)&newTemp);
        sprintf(buff,"%s/%s/%s/%s\n",newTemp->id,newTemp->pass,\
        newTemp->name,newTemp->tellNo);
        fwrite(buff,1,strlen(buff),fp); //sizeof에서 바꿔봄
    }
    fclose(fp);
    free(newTemp);
    pthread_mutex_unlock(&mutexid);

    return 0;
}

int saveBoardList(LPBOARD temp,LPARRAY boardFullList){
    pthread_mutex_t mutexid;
    pthread_mutex_init(&mutexid,NULL);
    pthread_mutex_lock(&mutexid);
    LPBOARD newTemp = (LPBOARD)malloc(sizeof(boardNormal));
    loadBoardList(&boardFullList);
    arrayAdd(boardFullList,(LPDATA)temp);
    FILE* fp;
    char buff[2800];
    fp = fopen("board_contents.txt","w");
    for(int i=0;i<arraySize(boardFullList);i++){
        arrayGetAt(boardFullList,i,(LPDATA*)&newTemp);
        sprintf(buff,"%s}%s}%s}%d}%s\n",newTemp->title,newTemp->context,\
        newTemp->id,newTemp->originNumber,newTemp->reply);
        fwrite(buff,1,strlen(buff),fp);
    }
    fclose(fp);
    free(newTemp);
    arrayDestroy(boardFullList);
    pthread_mutex_unlock(&mutexid);
    return 0;
}

int saveDataBoardList(LPDATABOARD temp, LPARRAY dataBoardFullList){
    pthread_mutex_t mutexid;
    pthread_mutex_init(&mutexid,NULL);
    pthread_mutex_lock(&mutexid);
    LPDATABOARD newTemp = (LPDATABOARD)malloc(sizeof(dataBoard));
    loadBoardList(&dataBoardFullList);
    arrayAdd(dataBoardFullList,temp);
    FILE* fp;
    char buff[1100];
    fp = fopen("DataBoard.txt","w");
    for(int i=0;i<arraySize(dataBoardFullList);i++){
        arrayGetAt(dataBoardFullList,i,(LPDATA*)&newTemp);
        sprintf(buff,"%s}%s}%s",newTemp->fileName,newTemp->Data,\
        newTemp->id);
        fwrite(buff,1,strlen(buff),fp);
    }
    fclose(fp);
    free(newTemp);
    arrayDestroy(dataBoardFullList);
    pthread_mutex_unlock(&mutexid);
    
    return 0;
}