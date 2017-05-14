#include "net.h"

void connection(char* sentMessage) {
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.90s\r\n", sentMessage);
        write(connfd, sendBuff, strlen(sendBuff));
        printf("Snoopy Detected: %s", sendBuff);

        read(connfd, sendBuff, strlen(sendBuff));
        printf("%s\n", sendBuff);

        close(connfd);
        sleep(1);
    }
}

void fopenConnection(char* path, char* addr) {
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1024];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if (inet_pton(AF_INET, addr, &serv_addr.sin_addr) <= 0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    while ( (n = write(sockfd, path, sizeof(path) - 1)) > 0)
    {
        // wait for response back from snoopy

        read(sockfd, recvBuff, strlen(recvBuff));
        printf("Response from snoopy: %s\n", recvBuff);

        close(sockfd);
        sleep(1);
    }
}