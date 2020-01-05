#include "http_web.h"

int main(void)
{
		int server_sock=-1;
		u_short port=0;
		int client_sock=-1;
		struct sockaddr_in client_name;
		socklen_t client_name_len = sizeof(client_name);
		

		server_sock= startup(&port);
		printf("http_web running on port %d\n",port);

		while(1)
		{
				client_sock=accept(server_sock,(struct sockaddr *)&client_name,&client_name_len);
				if(client_sock==-1)
						error_die("accept");
				accept_request(client_sock);
		}
		close(server_sock);
		
		return 0;
}

int startup(u_short *port)
{
		int httpd=0;
		struct sockaddr_in name;

		httpd=socket(PF_INET,SOCK_STREAM,0);
		if(httpd==-1)
				error_die("socket");
		memset(&name,0,sizeof(name));
		name.sin_family=AF_INET;
		name.sin_port=htons(*port);
		name.sin_addr.s_addr=htonl(INADDR_ANY);
		if(bind(httpd,(struct sockaddr *)&name,sizeof(name))<0)
				error_die("bind");
		if(*port==0)
		{
				int namelen=sizeof(name);
				if(getsockname(httpd,(struct sockaddr *)&name,&namelen) ==-1)
						error_die("getsockname");
				*port=ntohs(name.sin_port);
		}
		if(listen(httpd,5)<0)
				error_die("listen");

		return (httpd);
		
}
					

void accept_request(int client)
{
		char buf[1024];
		int numchars;
		char method[255];
		char url[255];
		char path[512];
		size_t i=0,j=0;
		struct stat st;
		int cgi=0;
		char *query_string = NULL;

		numchars = get_line(client,buf,sizeof(buf));

		while(!ISspace(buf[j])&&(i<sizeof(method)-1))
		{
				method[i]=buf[j];
				i++;
				j++;
		}
		method[i]='\0';
	    
	    if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))  
		{  
				unimplemented(client);  
                return;  
		}

		if(strcasecmp(method,"POST") == 0)
				cgi=1;

		i=0;
		while(ISspace(buf[j]) && (j<sizeof(buf)))
			   j++;
	    while(!ISspace(buf[j]) && (i<sizeof(buf)))
		{
			url[i]=buf[j];
		    i++;
		    j++;
		}
	    url[i]='\0';

		if(strcasecmp(method,"GET")==0)
		{
				query_string=url;
				while((*query_string!='?') && (*query_string !='\0'))
						query_string++;
				if(*query_string=='?')
				{
						cgi=1;
						*query_string='\0';
						query_string++;
				}
		}

		sprintf(path,"htdocs%s",url);
		if(path[strlen(path)-1]=='/')
				strcat(path,"index.html");
		if(stat(path,&st)==-1)
		{
				while((numchars>0)&&strcmp("\n",buf))
						numchars=get_line(client,buf,sizeof(buf));
				not_found(client);
		}
		else
		{
				if((st.st_mode&S_IFMT)==S_IFDIR)
						strcat(path,"/index.html");
				if((st.st_mode&S_IXUSR) || (st.st_mode &S_IXGRP) || (st.st_mode & S_IXOTH) ) 
						cgi=1;
				if(!cgi)
						serve_file(client,path);
				else
						execute_cgi(client,path,method,query_string);
		}
		close(client);
}	



