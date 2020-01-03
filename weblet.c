#include "head_file.h"

void process_trans(int fd);
void read_requesthdrs(rio_t *rp);
int is_static(char *uri);
void parse_static_uri(char *uri,char *filename);

void feed_static(int fd,char *filename,int filesize);
void get_flietype(char *filename,char *filetype);

void error_request(int fd,char *cause,char *errnum,char *shortmsg,char *description);

int main(int argc,char **argv)
{
		int listen_sock,conn_sock,port,clientlen;
		struct sockaddr_in clientaddr;

		if(argc!=2) {
				fprintf(stderr,"usage:%s<port>\n",argv[0]);
				exit(1);
		}

		port=atoi(argv[1]);

		listen_sock=open_listen_sock(port);
		while(1){
				clientlen=sizeof(clientaddr);
				conn_sock=accept(listen_sock,(SA *)&clientaddr,&clientlen);
				process_trans(conn_sock);
				close(conn_sock);
		}
}

void process_trans(int fd)
{
		int static_flag;
		struct stat sbuf;
		char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];

		char filename[MAXLINE],cgiargs[MAXLINE];
		rio_t rio;

		rio_readinitb(&rio,fd);
		rio_readlineb(&rio,buf,MAXLINE);
		sscanf(buf,"%s %s %s",method,uri,version);
		if(strcasecmp(method,"GET")){
				error_request(fd,method,"501","Not Implemented","weblet does not implement this method");
				return;
		}
		read_requesthdrs(&rio);

		static_flag=is_static(uri);
		if(static_flag)
				parse_static_uri(uri,filename);
		//else


		if(stat(filename,&sbuf)<0){
				error_request(fd,filename,"404","Not found","weblet could not find this file");
				return;
		}

		if(static_flag){
				if(!(S_ISREG(sbuf.st_mode))||(S_IRUSR&sbuf.st_mode)){
						error_request(fd,filename,"403","Forbidden","weblet is not permtted to read the file");
						return;
				}
				feed_static(fd,filename,sbuf.st_size);
		}
		//else  


}

int is_static(char *uri)
{
		if(!strstr(uri,"cgi-bin"))
				return 1;
		else
				return 0;
}

void error_request(int fd,char *cause,char *errnum,char *shortmsg,char *description)
{
		char buf[MAXLINE],body[MAXLINE];

		sprintf(body,"<html><title>error request</title>");
		sprintf(body,"%s<body bgcolor=""ffffff"">\r\n",body);
		sprintf(body,"%s%s:%s\r\n",body,errnum,shortmsg);
		sprintf(body,"%s<p>%s:%s\r\n",body,description,cause);
		sprintf(body,"%s<hr><em>weblet Web server</em>\r\n",body);

		sprintf(buf,"Http/1.0 %s\r\n",errnum,shortmsg);
		rio_writen(fd,buf,strlen(buf));
		sprintf(buf,"Content-type:text/html\r\n");
		rio_writen(fd,buf,strlen(buf));
		sprintf(buf,"Content-length:%d\t\n\r\n",(int)strlen(body));
		rio_writen(fd,buf,strlen(buf));
		rio_writen(fd,body,strlen(body));

}

void read_request(rio_t *rp)
{
		char buf[MAXLINE];

		rio_readlineb(rp,buf,MAXLINE);
		while(strcmp(buf,"\r\n")){
				printf("%s",buf);
				rio_readlineb(rp,buf,MAXLINE);
		}
		return;
}




