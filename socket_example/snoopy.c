#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

void server(){
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    char recvBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        ticks = time(NULL);
        //snprintf(sendBuff, sizeof(sendBuff), "%.90s\r\n", sentMessage);

        //read in filename
        read(connfd, recvBuff, sizeof(recvBuff)-1);
        printf("Read in filename from donor: %s\n", recvBuff);

        //trim off \r\n
        int len = strcspn(recvBuff, "\r");
        char* filename = malloc(200);
        strncpy(filename, recvBuff, len);

        //preform fopen
        FILE *fp;
        char * line = NULL;
        size_t leng = 0;
        ssize_t read;
        int counter = 0;

        char buffer[20][100];
        
        //read the file
        fp = fopen(filename,"r");
        while ((read = getline(&line, &leng, fp)) != -1) {
            strcpy(buffer[counter], line);
            //printf("%s", line);
            counter++;
        }

        fclose(fp);

        int j;
        for(j = 0; j < counter ; j++){
            printf("Read from file: %s\n", buffer[j]);
        }

        //contents aquired
        //send it back to the donor

        strcpy(sendBuff, buffer[0]);
        printf("Sending: %s\n", sendBuff);
        write(connfd, sendBuff, sizeof(sendBuff)-1);

        close(connfd);
        sleep(1);
     }
}

int main(int argc, char *argv[])
{
    // read in filename from socket
    server();
}