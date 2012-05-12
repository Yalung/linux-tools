/*simple, without error handling*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERV_PORT 8888

int main(int argc, char **argv)
{
	int listenfd, connfd;
	char buffer[1];
	struct sockaddr_in servaddr;
	int opt = 1;
	int len = sizeof(opt);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,len);
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, SOMAXCONN);
	connfd = accept(listenfd, NULL, NULL);

	for ( ; ; ) {
		read(connfd, buffer, 1);
		write(connfd, buffer, 1);
	}
}
