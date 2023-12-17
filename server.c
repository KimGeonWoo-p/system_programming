#include "kvs.h"

//for network
#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTENQ 1024
#define MAXLINE 10000

//compile project with dynamic linking!!
int main(int argc, char* argv[])
{
	//check if there is any argument
	if (argc < 2)
	{
		printf("no enough argument\n");
		return -1;
	}

	//connet with server
	int listenfd, connfd, port, clientlen;
	struct sockaddr_in serveraddr, clientaddr;
	char buf[MAXLINE];
	
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	
	port = atoi(argv[1]);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short) port);
	
	if (bind(listenfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) == -1)
	{
		perror("failed to bind");
		return -1;
	}
	
	if (listen(listenfd, LISTENQ) < 0)
		return -1;
	
	printf("Waiting for request ... \n");
	
	clientlen = sizeof(clientaddr);
	if ((connfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen)) < 0)
	{
		perror("accept");
		return -1;
	}
	
	char* caddrp = inet_ntoa(clientaddr.sin_addr);
	printf("Connected to %s\n", caddrp);

	//open kvs
	kvs_t* kvs = kvs_open();

	//open file to store result.
	FILE* fp = fopen("answer.dat", "w");

	write(connfd, "CONNECTED!!", 10);

	int n;
	int lines;
	while(1)
	{
		if (!(n = read(connfd, buf, MAXLINE)))
			break;
		printf("%ld bytes are read!!\n", strlen(buf));
		char* bp;
		char* op = strtok_r(buf, ",", &bp);
		char* key = strtok_r(NULL, ",", &bp);
		char* value = strtok_r(NULL, ",", &bp);
		char* result;
		
		if (op[0] == 'g')
		{
			result = get(kvs, key);
			if (!result)
			{
				printf("there is no data on key %s\n", key);
				result = "-1";
			}
			fprintf(fp, "%s\n", result);
		}
		else
			fprintf(fp, "%d\n", set(kvs, key, value));
		write(connfd, "RECEIVED!!", 10);
		lines++;
		memset(buf, 0, MAXLINE);
	}
	
	printf("%d operations!!\n", lines);
	printf("kvs' size: %d\n", kvs->items);
	kvs_close(kvs);
   
   	fclose(fp);
   	return 0;
}

