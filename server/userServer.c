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
#define CHATMEMBER 30
typedef struct{
	int sd;
	int* arrayInt;
}use1,*USE1;

void* start_login(void* arg)
{
	usleep(50000);
	LPMENU lpMenu;
	LPARRAY userFullList;
	loadUserList(&userFullList);
	int sd = ((USE1)arg)->sd;
	int* chatUser = ((USE1)arg)->arrayInt;
	menuCreate(&lpMenu,"menu_login.txt");
	send(sd, "clear!!", strlen("clear!!"), 0);
	usleep(50000);
	menuRun(lpMenu,sd,userFullList,chatUser);
	menuDestroy(lpMenu);
	arrayDestroy(userFullList);
	close(sd);
	return NULL;
	
}



char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

int main(){
	int new_fd,sockfd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	int yes=1;
	pthread_t tid,tid2;
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(60000);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(my_addr.sin_zero), '\0', 8);

	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	if(listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}
	int count =0;
	USE1 test=(USE1)malloc(sizeof(use1));
	int* chatUser = (int*)malloc(sizeof(int)*CHATMEMBER);
	for(int i=0;i<CHATMEMBER;i++){
		chatUser[i] = -1;
	}
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		if((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		test->sd = new_fd;
		test->arrayInt=chatUser;
		printf("server : got connection from %s \n", inet_ntoa(their_addr.sin_addr));
		if(pthread_create(&tid, NULL, start_login, test)!=0) {
			perror("pthread_create");
			break;
		} else {
			pthread_detach(tid);
		}
	}
		free(chatUser);
		free(test);
		return 0;
	}
