#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include "menu.h"
#include "userinfo.h"
#include "input.h"
#include "array.h"
#include "load.h"
#include "total.h"
//#include "member.h"

#define BACKLOG 1024

int main(){
    LPARRAY userFullList;
	loadUserList(&userFullList);
    char array1[10];
    scanf("%s",array1);
    LPUSER TempCheck = (LPUSER)malloc(sizeof(user));
    int i;
        for(i=0;i<arraySize((LPC_ARRAY)userFullList);i++){
	        arrayGetAt(userFullList,i, (LPDATA*)&TempCheck);
            printf("%s/%s/%s/%s\n",TempCheck->id,TempCheck->pass,TempCheck->pass,TempCheck->tellNo);
            if(strcmp(TempCheck->id,array1)==0) printf("이거!\n");
            }

}
	
