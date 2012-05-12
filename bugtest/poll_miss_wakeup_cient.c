#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERV_PORT 8888

int main(int argc, char **argv)
{
	int sockfd;
	struct pollfd read_poll[1];
	struct sockaddr_in servaddr;
	char buffer[1] = {'V'};
	long alive = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr * ) &servaddr, sizeof(servaddr));
	read_poll[0].fd = sockfd;
	read_poll[0].events = POLLIN;
	write(sockfd, buffer, 1);

	for ( ; ; ) {
		poll(read_poll, 1, -1);
		if (read_poll[0].revents & (POLLIN | POLLERR)) {
			read(sockfd, buffer, 1);
			write(sockfd, buffer, 1);
		}
		printf("alive %ld times\n", alive++);
	}
}
