#include "chat.h"

#define MAX_USER    5
#define MAX_NICKNAME    30
#define MAX_MESSAGE    1024

#define SERVER_IP    "127.0.0.1"
#define SERVER_PORT    10200

typedef struct _user{
    int userSd;
    int userNum;
    int roomSd;
    char userNick[MAX_NICKNAME];
}User;

typedef struct _room{
    int roomSd;
    int userSd[5];
    int userCount;
}Room; 
