#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        printf("Input: %s\n", recvBuff);
        //parse it into eax, and such

        //char input[4][40];        
        //char *ch;
        //int i = 0;
        //ch = strtok(recvBuff, ",");
        //while (ch != NULL) {
        //    strcpy(input[i], ch);
        //    ch = strtok(NULL, " ,");
        //    i++;
        //}

        //
        // After this, do what you'd like with the input from donor
        //

        //trim the /r/n off of the filename
        int len = strcspn(recvBuff, "\r");
        char* filename = malloc(200);
        strncpy(filename, recvBuff, len);

        FILE *fp;
        char * line = NULL;
        size_t leng = 0;
        ssize_t read;
        int counter = 0;

        char buffer[20][100];
        
        //read the file
        printf("Snoopy: About to open the file %s\n", filename);
        fp = fopen(filename,"r");
        while ((read = getline(&line, &leng, fp)) != -1) {
            strcpy(buffer[counter], line);
            printf("%s", line);
            counter++;
        }

        printf("Snoopy: About to close the file %s\n", filename);
        fclose(fp);

        printf("Snoopy: About to print out the file %s\n", filename);
        int j;
        for(j = 0; j < counter ; j++){
            printf("Read from file: %s\n", buffer[j]);
        }

        //
        // Then return it back to the server
        //

        printf("Snoopy: About to fill sendBuff and send %s\n", filename);
        // fill sendbuff with info
        strcpy(sendBuff, buffer[0]);
        printf("Sending: %s\n", sendBuff);
        write(sockfd, sendBuff, sizeof(sendBuff)-1); 
    }

    return 0;
} 