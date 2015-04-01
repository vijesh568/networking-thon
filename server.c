
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


#define PORT "23232"
int main()
{
	struct addrinfo hints, *res, *temp;
	int yes = 1;		/* to reuse addr */
	int sinsize=0;
	struct sockaddr_storage their_addr;
	memset(&hints, 0, sizeof(hints));

	/* fill hints */
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;


	if (getaddrinfo(NULL, PORT, &hints, &res)) {
		printf("Error!!!\n");
	}

	for (temp = res; temp != NULL; temp = temp->ai_next) {
		sockfd =
		    socket(res->ai_family, res->ai_socktype,
			   res->ai_protocol);
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
			printf("Error binding the socket to port\n");
			continue;
		}
	}
	if (temp == NULL) {
		printf("bind failed\n");
		return -1;

	}
	freeaddrinfo(res);

	/* listen */
	if (listen(sockfd, 5) == -1) {
		printf("Error listening to socket\n");
		return -1;
	}

	/* accept */


	while (1) {
		sin_size = sizeof their_addr;
		new_fd =   accept(sockfd, (struct sockaddr *) &their_addr,
			   &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		/* do operation */
		if(fork()=!=)
		{
			            close(sockfd); // child doesn't need the listener
				                if (send(new_fd, "Hello, world!", 13, 0) == -1)
							                perror("send");
						            close(new_fd);
							                exit(0);

		}
		close(new_fd);
		

	}
	return 0;



}
