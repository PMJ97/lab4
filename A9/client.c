#include "client.h"

int main()
{
    int port;

    printf("포트를 입력하세요 :");
    scanf("%d", &port);
    getchar();
    signal(SIGINT, SignalExit);
    
    pthread_mutex_init(&usermutex, NULL);
    
    if((sd = SockSetting(SERVER_IP, port)) == -1)
    {
        perror("socket");
        return 0;
    }

    JoinChat(sd);

    return 0;
}

void JoinChat(int SocketSd)
{
    User user;
    pthread_t ptr[2];
    char nickname[MAX_NICKNAME] = "";
    char rbuf[MAX_MESSAGE] = "";

    recv(SocketSd, rbuf, 100, 0);
    fputs(rbuf,stdout);
    fgets(nickname, sizeof(nickname), stdin);
    send(SocketSd, nickname, strlen(nickname), 0);
    user.userSd = SocketSd;
    strcmp(user.userNick,nickname);
    
    pthread_create(&ptr[0], NULL, RecvMsg, &user);
    pthread_detach(ptr[0]);
    pthread_create(&ptr[1], NULL, SendMsg, &user);
    pthread_detach(ptr[1]);

    while(1)
        pause();
}

void *RecvMsg(void *user)
{
    User ur = *(User *)user;
    char rbuf[MAX_MESSAGE];
    
    while(1)
    {
        if(flag == 0)
        {
            recv(ur.userSd, rbuf, sizeof(rbuf), 0);
            
        }    
        while(recv(ur.userSd, rbuf, sizeof(rbuf), 0) > 0)
        {
            fputs(rbuf, stdout);
            memset(rbuf, 0, sizeof(rbuf));
        }
    }
}

void *SendMsg(void *user)
{
    User ur = *(User *)user;
    char sbuf[MAX_MESSAGE];

    while(1)
    {
        fgets(sbuf, sizeof(sbuf), stdin);
        send(ur.userSd, sbuf, sizeof(sbuf), 0);
        if(!strncmp(sbuf, "/f", 2))
            flag = 1;

        memset(sbuf, 0, sizeof(sbuf));
    }
}

int SockSetting(char *ip, int port)
{
    int SocketSd;
    
    if((SocketSd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("sock error");
        return -1;
    }
    
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    if(connect(SocketSd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("connect");
        return -1;
    }

    return SocketSd;
}

void SignalExit(int no)
{
    printf("클라이언트를 종료합니다.\n");
    close(sd);
    exit(0);
}
