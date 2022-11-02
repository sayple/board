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
#include "total.h"
//#include "member.h"

#define BACKLOG 1024


void* start_main1(void* arg)
{
	int n;
	char buf[1024];
	int* sd = (int*) arg;
	while(1){
	sprintf(buf,"%d 님이 입장하셨습니다\n",sd[0]);
	send(sd[1],buf,sizeof(buf),0);

	n = recv(sd[0],buf,1024,0);
	printf("\n");
	send(sd[1],buf,n,0);
	}
	return NULL;
}
void* start_main2(void* arg){

	int n;
	char buf[1024];
	int* sd = (int*) arg;
	while(1){
	sprintf(buf,"%d 님이 입장하셨습니다\n",sd[1]);
	send(sd[0],buf,sizeof(buf),0);
	n= recv(sd[1],buf,1024,0);
	printf("\n");
	send(sd[0],buf,n,0);
	}
	return NULL;


}


char line[]="\n─────────────────────────────────────────────────────────────────────\n";
char thick_line[]="\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";

int main(){

	int sockfd, new_fd;
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
	int* sdMember=(int*)malloc(sizeof(int)*2);
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		if((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		else sdMember[count++]=new_fd;
		if(count%2==0){
			count=0;
			pthread_create(&tid, NULL, start_main1, sdMember)!=0;
			pthread_create(&tid2,NULL,start_main2,sdMember)!=0 ;
		} else {
			pthread_detach(tid);
			pthread_detach(tid2);
		}
		}
		return 0;
	}
