# FFamily
fopen, fread, fwrite, fclose

These are set of file related library calls that intercepted. This document explains in depth of how it is done.

## Common Behaviour
* fopen, fwrite and fclose communicate with the server through a function called net.c::fopenConnection()
* fread does not require the communication with the server
* fread simulate the reading of a file

## fopen
~~~~
FILE *fopen(const char *path, const char *mode) {
	printf("[lib.so] Hooked fopen\n");
	int (*old_fopen)(const char *path, const char *mode);

	// save original fopen
	old_fopen = dlsym(RTLD_NEXT, "fopen");

	char sendMessage[1024];
	memset(sendMessage, 0, sizeof(sendMessage));
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
~~~~

* fopen sends either one of the following packet to the server
 * fopen,read,[FILENAME]
: When the parameter *mode* include "r". When it is successful, it will get the file content, which it will save onto the memory.

 * fopen,write,[FILENAME]
: When the parameter *mode* include "w". When it is successful, it will get the content of packet indicating success, which it will save onto the memory.

 * In case, where there are both "r" and "w" in the *mode* parameter, "r" takes the precedence over "w". This is because it is usually safe to allow "read" then to "write" or even allow both.
* SIDE_EFFECT: Current server is vulnerable to directory traversal attack because it is not checking the value of the [FILENAME].

* If the result of fopenConnection is 0, then it creates a tmp FILE struct. This FILE* is only used for fclose and thus server will not be able to write any content to a file in the donor.
 * SIDE_EFFECT: Under current protocol, there is no way to check if the fopen operation on the server side has succeeded or not. This is because the file's content could be "success" or "failure".

* If the result of fopenConnection is 0, then it indicates that there was a failure in network related layer and thus should return NULL which is the return value of vanilla fopen if it fails for some reason.

* addKeyValuePairFile(FILE* key) adds the key-value pair on its own data memory. The returned value is an address to a heap of PAGE_SIZE. 

 * SIDE_EFFECT: Even in the case of failure, perror might not work correctly if it is implemented in the untrusted program.

## fread

~~~~
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
~~~~

* fread simulates reading a file. The only difference is that it reads from the memory and not the file.
* getMappedAddrFile(): this function queries the memory location that is mapped internally in the addKeyValuePairFile(FILE* key) function.
 * SIDE_EFFECT: Because I was not able to come up with a clean and trustworthy way of differentiating the FILE* current getMappedAddrFile returns last added FILE*. 
* retrievePageOfFile(): maintains the currentHead just like how files have the current head like indicator. It returns PAGE_SIZE for efficiency.
 * SIDE_EFFECT: The current bottleneck of the file size is PAGE_SIZE because the communication between donor and server was not easy to implement for multiple bidirectional communication (Synchronization issue). Current communication between the two happens just once for each direction where each packet is at most PAGE_SIZE (4096 bytes). 

## fwrite
~~~~
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
~~~~
* fwrite is the simplest among all the others because it has only type of packet.
 * fwrite,[CONTENT]
 * SIDE_EFFECT: Behavior not examined when trying to write more than PAGE_SIZE


## fclose
~~~~
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
~~~~

* fclose has commented out part of communication with the server
* Main reason for this is because the FILE* is not easy to handle. (Check README.md gridS Internal Challenge for detail)
