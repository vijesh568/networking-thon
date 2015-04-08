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
	int len;
	char buf[12];
};


int main()
{

	int ret =0;
	char *buf ="helloworld";
	struct type test;
	test.a =10;
	test.len = 4;
	memset(test.buf,'\0',12);
	strcpy(test.buf,"hihi");

	ret = VISUALBOX_Configure_Server(1,SOCK_STREAM,"12323");
	printf("Size of test = %d\n",sizeof(test));
	while(VISUALBOX_Sendto_Client((char*)&test,sizeof(test)));
	printf("sent helloworld\n");
	while(1);
	return 0;
}
