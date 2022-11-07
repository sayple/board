#ifndef _CURSSORCON_H_
#define _CURSSORCON_H_
#define _MACRO_H
// x,y 좌표로 커서 이동
#define gotoxy(x,y)	printf("[%d;%dH", x, y)
// 화면에 입력 문자 echo
#define setecho()   system("stty echo")
// 화면에 입력 문자 noecho
#define setnoecho() system("stty -echo")
// 한글자씩 입력
#define setraw()    system("stty raw");
// 한 문장씩 입력(개행문자 올때까지)
#define resetraw()  system("stty -raw -istrip sane")
// 화면 지우기
#define nclear()     system("clear")
#define COLOR_RED	"\033[38;2;255;0;0m"
#define COLOR_RESET	"\033[0m"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

void nnclear(){
    nclear();
    char buff[256];
    printf("┌───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    for(int i=1;i<16;i++){
        printf("│                                                                                                                                       │\n");
    }
    printf("└───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘\n");
    gotoxy(0,0);
}
void textAlign(int columns, char* buff, char* text)
{
    int indent = (columns - strlen(text)) / 2; // 왼쪽 여백 구하기 
    int i;
    strcpy(buff,"│");
    for (i = 0; i < indent-2; i++) 
    {
        strcat(buff," "); // 왼쪽 여백 채우기 
    }
    strcat(buff,text);
    while(1){
        strcat(buff," ");
        if(strlen(buff)==columns-2){
            strcat(buff,"│\n");
            break;
        }
    }
}
//되려나;
#endif