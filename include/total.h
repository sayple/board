#ifndef _TOTAL_H_
#define _TOTAL_H_
#include "array.h"
int login(int sd,LPARRAY userFullList,int* chatUser);
int join(int sd,LPARRAY userFullList,int* chatUser);
typedef struct{
    int sendSd;
    int* chatArray;
    LPARRAY sendList;
} sendFile, *LPSENDFILE;
#endif
