#ifndef _TOTAL_H_
#define _TOTAL_H_
#include "array.h"
int login(int sd,LPARRAY userFullList);
int join(int sd,LPARRAY userFullList);
typedef struct{
    int sendSd;
    LPARRAY sendList;
} sendFile, *LPSENDFILE;
#endif
