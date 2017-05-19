#include "libex.h"

int puts(const char *message)
{
	int (*old_puts)(const char *message);
	int result;

	// save original puts
	old_puts = dlsym(RTLD_NEXT, "puts");

	// If the message is Hello world!
	if (strcmp(message, "Hello world!\n") == 0)
	{
		// Print the following, using original puts
		result = old_puts("Goodbye, cruel world!n");
	}
	else
	{
		// if the string is different, use original puts() to echo
		result = old_puts(message);
	}
	return result;
}

FILE *fopen(const char *path, const char *mode) {
	printf("[lib.so] Hooked fopen\n");
	int (*old_fopen)(const char *path, const char *mode);

	// save original fopen
	old_fopen = dlsym(RTLD_NEXT, "fopen");

	char sendMessage[PAGE_SIZE];
	memset(sendMessage, 0, PAGE_SIZE);
	int pathLen = strlen(path);
	// int modeLen = strlen(mode);
	int index = 0;

	// create message to send
	strncpy(&sendMessage[index], "fopen,", 6);
	index += 6;
	if(strstr(mode, "r") != NULL){
		strncpy(&sendMessage[index], "read,", 5);
		index += 5;
	} else if(strstr(mode, "w") != NULL){
		strncpy(&sendMessage[index], "write,", 6);
		index += 6;
	}
	strncpy(&sendMessage[index], path, pathLen);
	index += pathLen;
	// printf("path: %s, pathLen:%d, mode: %s, modeLen:%d index:%d\n", path,pathLen, mode, modeLen, index);
	sendMessage[index] = '\0';

	char recvBuff[PAGE_SIZE];
	printf("[lib.so] Sending the current message: %s\n", sendMessage);
	int res = fopenConnection(sendMessage, SNOOPY_ADDR, recvBuff);

	// if server was successful opening it sending back the file information
	if(!res){
		// create dummy FILE*
		FILE* tmp = tmpfile();	

		// add to key-value structure, so that it can be referenced later
		void* page = addKeyValuePairFile(tmp);

		// copy file content onto memory
		memcpy(page, recvBuff, PAGE_SIZE);

		printf("[lib.so] Copied page: %s\n", (char*) page);

		return tmp;
	}

	return NULL;
}

int fclose(FILE *stream) {
	printf("[lib.so] Hooked fclose\n");
	int (*old_fclose)(FILE * stream);

	// save original fclose
	old_fclose = dlsym(RTLD_NEXT, "fclose");

	char sendMessage[PAGE_SIZE];
	memset(sendMessage, 0, PAGE_SIZE);

	// create message to send
	// strncpy(&sendMessage[index], "fclose,", 7);
	// index += 6;
	// strcpy(&sendMessage[index], filennameFromFileStruct(stream));
	
	// printf("[lib.so] Sending the current message: %s\n", sendMessage);
	// int res = fopenConnection(sendMessage, SNOOPY_ADDR, recvBuff);

	if (stream != NULL) {
		old_fclose(stream);

		void* ptr = getMappedAddrFile(stream);

		free(ptr);
	}

	return 0;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
	printf("[lib.so] Hooked fread\n");
	int (*old_fread)(void *ptr, size_t size, size_t nmemb, FILE *stream);

	// save original fread
	old_fread = dlsym(RTLD_NEXT, "fread");

	if(stream == NULL){
		return 0;
	}

	// rBuf
	char rBuf[PAGE_SIZE];

	// total bytes to read
	size_t nread = size*nmemb;
	printf("[lib.so] nread=%d\n", nread);

	// total read bytes
	size_t read_ret = 0;

	// byte copy of the memory that contains the file content and 
	for(size_t i = 0; i < nread; i += PAGE_SIZE){

		void* inner_addr = getMappedAddrFile(stream);

		// printf("[lib.so] Copied page: Addr: %x Content: %s\n", inner_addr, (char*) inner_addr);


		// retrieve the partial file content
		retrievePageOfFile(inner_addr, rBuf); // rBuf is page size

		// printf("[lib.so] returned from retrievePageOfFile: %s\n", rBuf);

		for(size_t j = 0; i + j < nread; j++){
			// dereference the params[i], i.e., the untrusted program's buffer to read
			((char*) ptr)[i + j] =  rBuf[j];

			// printf("[lib.so] rBuf[%d]=%c\n", j , rBuf[j]);

			// set return value of read to number of bytes read
			read_ret++;
		}

		printf("[lib.so] ptr:%s\n", (char*) ptr);
	}


	return read_ret;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
	printf("[lib.so] Hooked fwrite\n");
	int (*old_fwrite)(const void *ptr, size_t size, size_t nmemb, FILE *stream);

	// save original fwrite
	old_fwrite = dlsym(RTLD_NEXT, "fwrite");

	if(stream == NULL){
		return 0;
	}

	char sendbuf[PAGE_SIZE];
	char recvBuff[PAGE_SIZE];

	// concatenate strings for sendbuf
	memset(sendbuf, 0, PAGE_SIZE);
	strcat(sendbuf, "fwrite,");
	strncat(sendbuf, ptr, size*nmemb); // bug when params[2] > PAGE_SIZE - 6

	// inform server what to write
	fopenConnection(sendbuf, SNOOPY_ADDR, recvBuff);

	if(strcmp(recvBuff, "done")){
		return size*nmemb;
	}

	return 0;
}
