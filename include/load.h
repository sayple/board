#ifndef _LOAD_H
#define _LOAD_H
#include "array.h"
#include "boardinfo.h"

int loadUserList(LPARRAY* userFullList);
int loadBoardList(LPARRAY* boardFullList);
int loadDataBoardList(LPARRAY* dataFullList,const char* wayToFile);
int saveUserList(LPARRAY boardFullList);
int saveBoardList(LPBOARD temp, LPARRAY boardFullList);
char* loadID(LPARRAY userFullList);
char* loadName(LPARRAY userFullList);
void loadText(int sd, char* id,char* nickName, LPARRAY boardFullList);
int saveBoard(LPARRAY dataBoardFullList);
#endif