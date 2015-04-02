
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

typedef struct vb_client_conn_prop_t {
	int fd;
	union {
		struct sockaddr_in *sinaddr;
		struct sockaddr_in6 *sin6addr;
	}addr;
	struct vb_client_conn_prop_t* next;
}vb_conn_prop_st;


vb_server_data_t server_gdt={0};
int run_server=1;/* 1 or 0*/
int g_vb_clients = 0;
vb_conn_prop_st* g_vb_conn_list_head = NULL;
vb_conn_prop_st* g_vb_conn_list_tail = NULL;


void *get_in_addr(struct sockaddr *sa)
{
	    if (sa->sa_family == AF_INET) {
		            return &(((struct sockaddr_in*)sa)->sin_addr);
			        }

	        return &(((struct sockaddr_in6*)sa)->sin6_addr);
			
}

int visualbox_read_data(int sockfd, int len);

vb_conn_prop_st* visualbox_get_conn_list_slot()
{

	return g_vb_conn_list_tail;

}

void visualbox_manage_conn_prop(int new_fd, vb_conn_prop_st* conn_st)
{

	vb_conn_prop_st *loc = NULL;

	conn_st->fd = new_fd;
	if(client_addr->ss_family == AF_INET)
	{
		conn_st->addr.sinaddr = (struct sockaddr_in*)client_addr;
	}
	else if(client_addr->ss_family = AF_INET6)
	{
			conn_st->addr.sinaddr = (struct sockaddr_in6*)client_addr;
	}
	g_vb_clients++;
	loc = vb_get_conn_list_slot();
	if (!loc)
	{
		g_vb_conn_list_head= conn_st;
	}
	else
	{
		loc->next = conn_st;
		g_vb_conn_list_tail = conn_st;
	}
	return;

}


void visualbox_read_handler( void* payload)
{



}
void visualbox_server_handler(void* payload)
{

	int sin_size;
	int new_fd;
	int ret = -1;
	struct sockaddr_storage *client_addr =  NULL;
	int socketfd;
	
	vb_conn_prop_st *conn_st = NULL;
	vb_common_thread_payload *payload = NULL;

	vb_common_thread_payload *load = (vb_common_thread_payload*)payload;
	vb_server_thread_payload *data = (vb_server_thread_payload*)load->data;

	
	
	if(data != NULL)
	{
		socketfd = data->sockid;
		
		/* listen */
		 printf("visualbox_server_handler : INFO Listening\n");
		 if (listen(socketfd, data->maxclients) == -1) {
			 perror("visualbox_server_handler :Error listening to socket");
			 return -1;
		 }
		
		 while(run_server)
		 {
				sin_size = sizeof (struct sockaddr_storage);
				client_addr = (struct sockaddr_storage*)malloc(sizeof(struct sockaddr_storage));
				
				new_fd = accept(socketfd, (struct sockaddr *) &client_addr,sin_size);
				if (new_fd == -1) {
					perror("visualbox_server_handler : ERROR! in accept");
					continue;
				}
			
		     	conn_st = (vb_conn_prop_st*)malloc(sizeof(vb_conn_prop_st));

				visualbox_manage_conn_prop(new_fd,conn_st);
				
				payload->data = (void*)conn_st;
				ret = visualbox_osport_create_thread(payload,(void*)visualbox_read_handler);
				
		
		 }
		 close(socketfd);



		}
		return 0;	       
}
	



int visualbox_osport_create_thread(vb_common_thread_payload* payload, void* handler)
{
	pthread_t thread_id ;
	
	if (pthread_create( &thread_id , NULL ,  handler , (void*) payload) !=0)
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
	
	ret = visualbox_osport_create_thread(payload,(void*)visualbox_server_handler);

	return ret;


}


int VISUALBOX_Configure_Server(int clients_supported, int socktype,int port)
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

