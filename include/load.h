#ifndef _LOAD_H
#define _LOAD_H
#include "array.h"
#include "boardinfo.h"

int loadUserList(LPARRAY* userFullList);
int loadBoardList(LPARRAY* boardFullList);
int loadDataBoardList(LPARRAY* dataFullList);
int saveUserList(LPARRAY boardFullList);
int saveBoardList(LPBOARD temp, LPARRAY boardFullList);
int saveDataBoardList(LPDATABOARD temp, LPARRAY dataFullList);
char* loadName(LPARRAY userFullList);
char* loadText(LPARRAY* boardFullList);
#endif