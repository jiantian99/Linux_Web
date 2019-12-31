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
