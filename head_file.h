#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>


typedef struct sockaddr SA;

#define MAXLINE 8192

#define RIO_BUFSIZE 8192
typedef struct{
		int rio_fd;
		int rio_cnt;
		char *rio_bufptr;
		char rio_buf[RIO_BUFSIZE];
}rio_t;

int open_listen_sock(int portno);
