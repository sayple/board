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

int board(int sd,LPARRAY userFullList){
    LPMENU lpMenu;
    menuCreate(&lpMenu,"menu_board.txt");
	menuRun(lpMenu,sd,userFullList);
	menuDestroy(lpMenu);
    return 0;
}