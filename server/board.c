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
#include "total.h"
#include "load.h"

int board(int sd,LPARRAY userFullList,int* chatUser){
    send(sd, "clear!!", strlen("clear!!"), 0);
    usleep(50000);
    LPMENU lpMenu;
    menuCreate(&lpMenu,"menu_board.txt");
	menuRun(lpMenu,sd,userFullList,chatUser);
	menuDestroy(lpMenu);
    send(sd, "clear!!", strlen("clear!!"), 0);
    usleep(5000);
    return 0;
}