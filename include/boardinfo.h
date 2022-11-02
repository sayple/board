#ifndef _BOARDINFO_H_
#define _BOARDINFO_H_
#include "reply.h"
typedef struct {
    int request; //0 :none 1:add 2:change 3:delete
    char frontText[81];
    char context[501];
    char id[16];
    int originNumber;
    char reply[2048];
}board,*LPBOARD;

typedef struct{
    char fileName[20];
    char Data[1024];
    char id[16];
}dataBoard,*LPDATABOARD;
#endif
