#ifndef _USERINFO_H_
#define _USERINFO_H_
typedef struct {
    int request; //0 :none 1:add 2:change 3:delete
    char id[16];
    char pass[16];
    char name[32];
    char tellNo[16];
}user,*LPUSER;

#endif
