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


struct type {
	int a;
	char buf[10];
};


int main()
{

	int ret =0;
	char *buf ="helloworld";
	struct type test;
	test.a =10;
	memset(test.buf,'\0',10);
	strcpy(test.buf,"hihi");

	ret = VISUALBOX_Configure_Server(1,SOCK_STREAM,"12323");
	while(VISUALBOX_Sendto_Client((char*)&test,strlen(buf)));
	printf("sent helloworld\n");
	while(1);
	return 0;
}
