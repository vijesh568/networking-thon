
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
#include <pthread.h>


#define PORT "13323"

typedef enum vb_profile_channels_t{
	MCHAI_CHANNEL = 0,
	GRAPHICS_CHANNEL = 1,
	OS_CHANNEL = 2,
	STATIC_CHANNEL=3
}VB_PROFILE_CHANNEL_TYPE;

typedef struct server_data{
	pthread_t server_thread_id;
	pthread_t server_sock_id;

}vb_server_data_t;

typedef struct vb_server_thread_load_t{
	int sockid;
	int maxclients; /* Unused */
	
}vb_server_thread_payload;

typedef struct vb_common_thread_load_t{
	void* data;
}vb_common_thread_payload;

typedef struct vb_channel_configure_data_t{
	VB_PROFILE_CHANNEL_TYPE channeltype;
	int subtype;
	int option;
}vb_channel_config_data;


vb_server_data_t server_gdt={0};
int run_server=1;/* 1 or 0*/

void *get_in_addr(struct sockaddr *sa)
{
	    if (sa->sa_family == AF_INET) {
		            return &(((struct sockaddr_in*)sa)->sin_addr);
			        }

	        return &(((struct sockaddr_in6*)sa)->sin6_addr);
			
}

int visualbox_read_data(int sockfd, int len, 

void visualbox_server_handler(void* payload)
{

	vb_common_thread_payload *load = (vb_common_thread_payload*)payload;
	vb_server_thread_payload *data = (vb_server_thread_payload*)load->data;
	int socketfd;
	vb_channel_config_data readdata;
	
	if(data != NULL)
	{

	
		socketfd = data->sockid;
		while(run_server)
		{

			ret = visualbox_read_data(socketfd,sizeof(vb_channel_config_data),&readdata);
			if(ret == 0)
			{
				
			}

			

		

		}
	       
	}
	
	/* listen */
	printf("Listening\n");
	if (listen(sockfd, 5) == -1) {
		printf("Error listening to socket\n");
		return -1;
	}

	/* accept */


	while (1) {
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *) &client_addr,
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

int visualbox_osport_create_thread(vb_common_thread_payload* payload)
{
	pthread_t thread_id ;
	
	if (pthread_create( &thread_id , NULL ,  visualbox_server_handler , (void*) payload) !=0)
	{
		perror("visualbox_osport_create_thread: ERROR: could not create thread - ");
		return -1;
		
	}

	return thread_id;
	
}
int visualbox_start_server(int sockfd,int clients_supported)
{

	vb_common_thread_payload *payload;
	vb_server_thread_payload *load;
	
	int ret=-1;


	load = (vb_server_thread_payload *) malloc(sizeof(vb_server_thread_payload));
	payload = (vb_common_thread_payload *) malloc(sizeof(vb_common_thread_payload));
	
	assert(load != NULL);
	assert(payload != NULL);
	
	load->sockid = sockfd;
	load->maxclients = clients_supported;

	payload->data=(void*)load;
	
	ret = visualbox_osport_create_thread(payload);

	return ret;


}

#if 0
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



}
#endif

int visualbox_configure_server(int clients_supported, int socktype,int port)
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
	hints.ai_socktype = socktype;


	if (getaddrinfo(NULL, port, &hints, &res)) {
		perror("visualbox_configure_server ERROR! getaddrinfo failed on port %d",port);
		return -1;
	}

	for (temp = res; temp != NULL; temp = temp->ai_next) {
		sockfd =
		    socket(temp->ai_family, temp->ai_socktype,
			   temp->ai_protocol);
		
		if (sockfd == -1) {
			perror("visualbox_configure_server WARN: socket() failed: ");
			continue;
		}

		if (setsockopt
		    (sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		     sizeof(int)) == -1) {
			perror("visualbox_configure_server WARN: setscokopt failed : ");
			     errno);
			continue;
		}
		if (bind(sockfd, temp->ai_addr, temp->ai_addrlen) == -1) {
			perror("visualbox_configure_server WARN : bind  failure ");
			printf("visualbox_configure_server WARN binding the socket to port\n");
	        inet_ntop(temp->ai_family, get_in_addr((struct sockaddr *)temp->ai_addr),(char*) s, sizeof s);
			printf("visualbox_configure_server INFO server:bind failed  from %s\n", s);
			continue;
		}
		break;
	}
	if (temp == NULL) {
		printf("visualbox_configure_server : ERROR!! bind failed for all available ports in localhost\n");
		return -1;

	}
	freeaddrinfo(res);

	return visualbox_start_server(sockfd,clients_supported);
	

}

