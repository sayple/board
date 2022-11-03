#ifndef _BOARDINFO_H_
#define _BOARDINFO_H_
#include "reply.h"
typedef struct {
    int request; //0 :none 1 : choice
    char title[81];
    char context[501];
    char id[16];
    int originNumber;
    char reply[2048];
}boardNormal,*LPBOARD;

typedef struct{
    char fileName[20];
    char Data[1024];
    char id[16];
}dataBoard,*LPDATABOARD;
#endif
