#ifndef _BOARDINFO_H_
#define _BOARDINFO_H_
#include <sys/types.h>
#include <dirent.h>
typedef struct {
    int originNum; 
    char title[81];
    char context[756];
    char id[16];
    char nick[16];
    char reply[4096];
}boardNormal,*LPBOARD;

typedef struct{
    char fileName[128];
    int number;
}dataBoard,*LPDATABOARD;

#endif
