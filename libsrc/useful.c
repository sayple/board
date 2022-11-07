#include <stdio.h>
#include "array.h"
#include "userinfo.h"
#include <string.h>
#include <stdlib.h>

char* getName(LPARRAY userFullList){
    LPARRAY boardFulllist;
    char* userName = (char*)malloc(sizeof(char)*16);
    LPUSER user1;
    int i;
    for(i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	    arrayGetAt((LPC_ARRAY)userFullList,i, (LPDATA*)&user1);
        if(user1->request ==1) break;
    }
    strcpy(userName,user1->id);
    return userName;
    if(i==arraySize((LPC_ARRAY)userFullList)){
        return NULL;
    }
}