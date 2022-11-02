#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "userinfo.h"
#include "boardinfo.h"
#include "input.h"
#include "array.h"

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
        Temp->request=0;
        if(str[0] !=0){
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
        LPBOARD Temp = (LPBOARD)malloc(sizeof(dataBoard));
        Temp->request=0;
        if(str[0] !=0){
            key = strtok(str,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->frontText,key);
            key = strtok(NULL,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->context,key);
            key = strtok(NULL,"/");
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
            Temp->originNumber = atoi(key);
            key = strtok(NULL,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
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
	fp = fopen("DataBoard.txt", "r");
	if(NULL== fp){
        perror("file open error!\n");
        exit(1);
    }
	while(1){
        str = inputLine(fp);
        if(NULL==str) break;
        LPDATABOARD Temp = (LPDATABOARD)malloc(sizeof(dataBoard));
        if(str[0] !=0){
            key = strtok(str,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->fileName,key);
            key = strtok(NULL,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->Data,key);
            key = strtok(NULL,"/");
            if(key==NULL) {
                perror("file open error!\n");
                exit(1);
            }
            strcpy(Temp->id,key);
            key = strtok(NULL,"/");
        }
        arrayAdd(*dataFullList,(const LPDATA) Temp);
	}
	fclose(fp);
	return 0;
}
