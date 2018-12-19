#include "server.h"

int main()
{
    int room, i;

    signal(SIGINT, SignalExit);

    pthread_mutex_init(&usermutex, NULL);
    pthread_mutex_init(&roommutex, NULL);

    printf("방 갯수를 입력하세요\n");
    scanf("%d",&room);
    getchar();

    pthread_t ptr[256];

    for(i=0;i<room;i++)
    {
        pthread_create(&ptr[i], NULL, ThreadMain, &room);
        pthread_detach(ptr[i]);
    }

    while(1)
        pause();

    return 0;
}

void *ThreadMain(void *room)
{
    pthread_mutex_lock(&roommutex);
    User user;
    Room rm;

    int SocketSd;
    int port = SERVER_PORT + roomCount;
    rm.userCount = roomCount;

    SocketSd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(SocketSd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        perror("bind error");
        close(SocketSd);
    }
    
    if(listen(SocketSd, 5) == -1)
    {
        perror("listen error");
        close(SocketSd);
    }

    printf("port %d\n",port);

    roomCount++;

    pthread_mutex_unlock(&roommutex);

    struct sockaddr_in cliaddr = {0};
    int clen = sizeof(cliaddr);
    pthread_t ptr;
    
    while((user.userSd = accept(SocketSd, (struct sockaddr *)&cliaddr, &clen)))
    {
        if(user.userSd == -1)
        {
            perror("accept");
            return 0;
        }

        pthread_mutex_lock(&usermutex);
        
        user.userNum = rUsernum;

        user.roomSd = SocketSd;
        rm.roomSd = SocketSd;
        rm.userSd[rm.userCount] == user.userSd;
        
        roomList[rm.userCount] = rm;
        pthread_create(&ptr, NULL, JoinChat, &user);
        pthread_detach(ptr);

        rUsernum++;
        rm.userCount++;
        
        pthread_mutex_unlock(&usermutex);
    }
    
    return 0;
}

void *JoinChat(void *user)
{
    User ur = *(User *)user;
    char nickname[MAX_NICKNAME] = "";
    pthread_t ptr[2];

    send(ur.userSd, "채팅방에 오신 것을 환영합니다.\n닉네임을 입력하세요.\n", 100, 0);
    recv(ur.userSd, nickname, sizeof(nickname), 0);
    nickname[strlen(nickname)-1] = '\0';
    printf("%s 접속\n",nickname);
    
    strcpy(ur.userNick, nickname);

    userList[ur.userNum] = ur;

    pthread_create(&ptr[0], NULL, ConveyMessage, &ur);
    pthread_create(&ptr[1], NULL, Notice, &ur);
    pthread_join(ptr[0], NULL);
    pthread_join(ptr[1], NULL);
}

void *ConveyMessage(void *user)
{
    User ur = *(User *)user;
    int i;
    char rbuf[MAX_MESSAGE];
    char sbuf[MAX_MESSAGE];

    while(recv(ur.userSd, rbuf, sizeof(rbuf), 0) >0)
    {
        if(!strncmp(rbuf,"/f",2))
        {
            SendFile(ur, rbuf);
            continue;
        }
        else
        {
            sprintf(sbuf, "%s : %s", ur.userNick, rbuf);
            for(i=0;i<rUsernum;i++)
            {
                if(userList[i].userSd == ur.userSd)
                    continue;
                if(userList[i].roomSd == ur.roomSd)
                    send(userList[i].userSd, sbuf, sizeof(sbuf), 0);
            }
        }

        memset(rbuf, 0, sizeof(rbuf));
        memset(sbuf, 0, sizeof(sbuf));
    }    

    ClientExit(ur);
}

void SendFile(User user, char *rbuf)
{
    int fd;
    char *fp;
    char sbuf[MAX_MESSAGE];

    fp = strtok(rbuf, " ");
    fd = open(fp, O_RDONLY, 0644);
    
    while(read(fd, rbuf, sizeof(rbuf)) > 0)
    {
        if(send(user.userSd, rbuf, sizeof(rbuf), 0) == -1)
        {
            perror("send");
            exit(1);
        }
    }
}

void ClientExit(User user)
{
    int i, j;
    char sbuf[MAX_MESSAGE];
    
    sprintf(sbuf, "%s님이 퇴장하였습니다.\n", user.userNick);

    for(i=0;i<rUsernum;i++)
    {
        if(userList[i].userSd == user.userSd)
        {
            for(j=0;j<rUsernum;j++)
            {
                if(userList[j].userSd == user.userSd)
                    continue;
                if(userList[j].roomSd == user.roomSd)
                    send(userList[j].userSd, sbuf, sizeof(sbuf), 0);
            }
            break;
        }
    }

    pthread_mutex_lock(&usermutex);
    for(j=i;j<rUsernum-1;j++)
    {
        userList[j] = userList[j+1];
    }
    rUsernum --;
    pthread_mutex_unlock(&usermutex);
}

void *Notice(void *user)
{
    int i;
    User ur = *(User *)user;
    char sbuf[MAX_MESSAGE];
    char inbuf[MAX_MESSAGE];
    
    sprintf(sbuf, "%s님이 접속하였습니다.\n", ur.userNick);
    for(i=0;i<rUsernum;i++)
    {
        if(userList[i].roomSd == ur.roomSd)
            send(userList[i].userSd, sbuf, strlen(sbuf), 0);
    }

    while(1)
    {
        memset(sbuf, 0, sizeof(sbuf));
        memset(inbuf, 0, sizeof(inbuf));
        fgets(inbuf, sizeof(inbuf), stdin);
        sprintf(sbuf, "[공지] : %s\n", inbuf);
        for(i=0;i<rUsernum;i++)
            send(userList[i].userSd, sbuf, strlen(sbuf), 0);
    }
}

int ServerSetting(char *ip, int port)
{
    int SocketSd;
    SocketSd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in servaddr = {0};
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    if(bind(SocketSd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
    {
        perror("bind error");
        close(SocketSd);
        return -1;
    }
    
    if(listen(SocketSd, 5) == -1)
    {
        perror("listen error");
        close(SocketSd);
        return -1;
    }
    return SocketSd;
}

void SignalExit(int no)
{
    printf("서버를 종료합니다.\n");
    close(roomSd);
    close(sd);
    exit(0);
}
