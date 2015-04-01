
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>


#define PORT "13323"




void *get_in_addr(struct sockaddr *sa)
{
	    if (sa->sa_family == AF_INET) {
		            return &(((struct sockaddr_in*)sa)->sin_addr);
			        }

	        return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
int main()
{
	struct addrinfo hints, *res, *temp;
	int yes = 1;		/* to reuse addr */
	int sin_size = 0;
	struct sockaddr_storage their_addr;
	memset(&hints, 0, sizeof(hints));
	int sockfd,new_fd;
	char s[INET6_ADDRSTRLEN]={'\0'};

	/* fill hints */
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;


	if (getaddrinfo(NULL, PORT, &hints, &res)) {
		printf("Error!!!\n");
	}

	for (temp = res; temp != NULL; temp = temp->ai_next) {
		sockfd =
		    socket(temp->ai_family, temp->ai_socktype,
			   temp->ai_protocol);
		if (sockfd == -1) {
			printf("Error!! socket creation failed\n");
			continue;
		}

		if (setsockopt
		    (sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		     sizeof(int)) == -1) {
			printf
			    ("Error setting option to socket, errno=%d\n",
			     errno);
			continue;
		}
		if (bind(sockfd, temp->ai_addr, temp->ai_addrlen) == -1) {
			perror("bind  failure");
			printf("Error binding the socket to port\n");
		        inet_ntop(temp->ai_family, get_in_addr((struct sockaddr *)temp->ai_addr),
							            (char*) s, sizeof s);
			printf("server:bind failed  from %s\n", s);
			continue;
		}
		break;
	}
	if (temp == NULL) {
		printf("bind failed\n");
		return -1;

	}
	freeaddrinfo(res);

	/* listen */
	printf("Listening\n");
	if (listen(sockfd, 5) == -1) {
		printf("Error listening to socket\n");
		return -1;
	}

	/* accept */


	while (1) {
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *) &their_addr,
				&sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

	printf("Connection request available\n");
		/* do operation */
		if (fork()  !=0) {
			close(sockfd);	// child doesn't need the listener
			if (send(new_fd, "Hello, world!", 13, 0) == -1)
				perror("send");
			close(new_fd);
			printf("exiting child\n");
			exit(0);

		}
		close(new_fd);
		printf("looping\n");

	}
	close(sockfd);
	return 0;



}

