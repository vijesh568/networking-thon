#include <stdio.h>

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





int main()
{

	int ret =0;
	char *buf ="helloworld";
	ret = VISUALBOX_Configure_Server(1,SOCK_STREAM,"12323");
	ret = VISUALBOX_Sendto_Client(buf,strlen(buf));
	return 0;
}
