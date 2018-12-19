#include "info.h"

User userList[5];

int sd;
int roomSd;
int flag = 0;
int roomCount = 0;
int rUsernum = 0;
pthread_mutex_t usermutex;

void SignalExit(int no);
int SockSetting(char *ip, int port);
void JoinChat(int SocketSd);
void *RecvMsg(void *user);
void *SendMsg(void *user);
void *ThreadMain(void *ur);
