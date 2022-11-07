#ifndef _BOARDINFO_H_
#define _BOARDINFO_H_
#include "reply.h"
#include <sys/types.h>
#include <dirent.h>
typedef struct {
    int request; //0 :none 1 : choice
    char title[81];
    char context[524];
    char id[16];
    char nick[16];
    char reply[2048];
}boardNormal,*LPBOARD;

typedef struct{
    char fileName[128];
    int number;
}dataBoard,*LPDATABOARD;

#endif
