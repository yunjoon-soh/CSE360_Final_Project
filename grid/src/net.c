#include "net.h"

void connection(char* sentMessage) {
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	char sendBuff[1025];
	// time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	memset(sendBuff, 0, sizeof(sendBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while (1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		// ticks = time(NULL);
		snprintf(sendBuff, sizeof(sendBuff), "%.90s\r\n", sentMessage);
		write(connfd, sendBuff, strlen(sendBuff));
		printf("Snoopy Detected: %s", sendBuff);

		read(connfd, sendBuff, strlen(sendBuff));
		printf("%s\n", sendBuff);

		close(connfd);
		sleep(1);
	}
}

int fopenConnection(char* path, char* addr, char* recvBuff) {
	int sockfd = 0, n = 0;

	memset(recvBuff, 0, PAGE_SIZE);

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

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

	n = write(sockfd, path, PAGE_SIZE);
	printf("[net.c]Sent %d number of bytes: %s\n", n, path);

	// wait for response back from snoopy
	read(sockfd, recvBuff, PAGE_SIZE);
	printf("[net.c]Response from snoopy: %s\n", recvBuff);

	close(sockfd);

	return 0;
}

void server() {
	int BUF_SIZE = 4096;
	char sendBuff[BUF_SIZE];
	memset(sendBuff, 0, BUF_SIZE);
	char recvBuff[BUF_SIZE];
	memset(recvBuff, 0, BUF_SIZE);

	int listenfd = 0, connfd = 0;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	// FILE* used for fwrite
	char* writeFileName = NULL;
	FILE* writeFile = NULL;

	// setup socket
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while (1)
	{
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		int nRead = 0;
		char input[4][200];
		memset((void*) input, 0, 800);

		// read input from
		nRead = read(connfd, recvBuff, BUF_SIZE);
		if (nRead < 0) {
			perror("read");
			goto Failed;
		}
		printf("Input from donor[%d]: %s\n", nRead, recvBuff);

		// tokenize
		char *ch = strtok(recvBuff, ",");
		int i = 0;
		while (ch != NULL && i < 4) {
			strcpy(input[i], ch);
			printf("[%d]%s\n", i, input[i]);
			i++;
			ch = strtok (NULL, ",");
		}

		for (i = 0; i < 4; i++) {
			printf("Input[%d]:%s\n", i, input[i]);
		}


		if (strcmp(input[0], "fopen") == 0) {
			char filename[200];
			memset(filename, 0, 200);

			strncpy(filename, input[2], strlen(input[2]));

			//preform fopen
			FILE *fp;
			char * line = NULL;
			size_t leng = 0;
			ssize_t read;
			int counter = 0;

			if(strcmp(input[1], "read")){
				//read the file
				fp = fopen(filename, "r");
				if (fp == NULL) {
					strncpy(sendBuff, INVALID_FILE_NAME, strlen(INVALID_FILE_NAME));
					printf("%s: %s\n", INVALID_COMMAND, filename);
				} else {
					char buffer[20][100];

					while ((read = getline(&line, &leng, fp)) != -1) {
						strcpy(buffer[counter], line);
						counter++;
					}

					fclose(fp);

					int j;
					for (j = 0; j < counter ; j++) {
						printf("Read from file: %s\n", buffer[j]);
					}

					strcpy(sendBuff, buffer[0]);
				}
			}
			else if(strcmp(input[1], "write")){
				fp = fopen(filename, "w");
				if(fp == NULL){
					strncpy(sendBuff, INVALID_FILE_NAME, strlen(INVALID_FILE_NAME));
					printf("%s: %s\n", INVALID_COMMAND, filename);
				} else {
					writeFile = fp;
					writeFileName = filename;
				}
			}
			else {
				goto Failed;
			}
		} else if(strcmp(input[0], "fwrite") == 0){
			char filename[200];
			memset(filename, 0, 200);

			strncpy(filename, input[1], strlen(input[1]));
			// FILE* writeFile = findFile(filename);

			if(writeFile == NULL){
				strncpy(sendBuff, FILE_NOT_OPEN, strlen(FILE_NOT_OPEN));
				printf("%s: %s\n", FILE_NOT_OPEN, filename);
			} else {
				fwrite(input[1], strlen(input[1]), 1, writeFile);
			}
		} else if(strcmp(input[0], "fclose") == 0){
			char filename[200];
			memset(filename, 0, 200);

			strncpy(filename, input[1], strlen(input[1]));
			// FILE* closeFile = findFile(filename);
			FILE* closeFile = writeFile;
			if(closeFile == NULL){
				strncpy(sendBuff, FILE_NOT_OPEN, strlen(FILE_NOT_OPEN));
				printf("%s: %s\n", FILE_NOT_OPEN, filename);
			} else {
				fclose(closeFile);
			}
		}
		else {
Failed:
			strncpy(sendBuff, INVALID_COMMAND, strlen(INVALID_COMMAND));
		}

		//contents aquired
		//send it back to the donor
		printf("Sending: %s\n", sendBuff);
		write(connfd, sendBuff, sizeof(sendBuff) - 1);

		close(connfd);
	}
}