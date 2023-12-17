#include "kvs.h"
#include <string.h>
#include <unistd.h>

//for network
#include <arpa/inet.h>

#define MAXLINE 10000

//compile project with dynamic linking!!
int main(int argc, char* argv[])
{
	//Connect to Server!!
	int clientfd, port;
	struct sockaddr_in serveraddr;
	char *host;
	
	char* source;

	if (argc < 3)
	{
		printf("not enough argument!!\n");
		return -1;
	}
	
	host = argv[1];
	port = atoi(argv[2]);
	//check if there is any argument
	if (argc == 4)
	{
		source = argv[3];
	} else
	{
		source = "cluster004_web.trc";
	}

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}
	
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons((unsigned short) port);
	serveraddr.sin_addr.s_addr = inet_addr(host);
	
	if (connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		perror("connect");
		return -1;
	}

	//Open file!!
	FILE* fp;
	if (!(fp = fopen(source, "r")))
	{
		printf("failed to open file!!\n");
		return -1;
	}
	
	int wbytes;
	char buf[MAXLINE];
	
	while(read(clientfd, buf, MAXLINE) < 0);

	while(!feof(fp))
	{
		fscanf(fp, "%s\n", buf);
		if ((wbytes=write(clientfd, buf, strlen(buf))) < strlen(buf))
			printf("failed to send message\n");
		printf("write %d bytes!!\n", wbytes);
		while(read(clientfd, buf, MAXLINE) < 0);
	}
	
	fclose(fp);
	close(clientfd);
   	return 0;
}

