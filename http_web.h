#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pthread.h>

#define ISspace(x) isspace((int)(x))  
  
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n" 

//初始化 Http 服务
int start_up(u_short *);
//处理一个从套接字上监听到的 HTTP 请求
void accept_request(int); 
//运行 cgi 程序
void run_cgi(int, const char *, const char *, const char *);
//处理执行 cgi 程序时出现的错误
void cannot_run(int);
//HTTP 404 
void not_found(int);
//读取 socket 套接字
void cat(int, FILE *);
//HTTP 400
void bad_request(int);
//显示错误信息
void error_die(const char *);
//读取套接字的一行
int get_line(int, char *, int);
//将HTTP 响应的头部写到套接字
void headers(int, const char *);
//把服务器文件返回给客户端（浏览器）
void serve_file(int, const char *);
//请求的method不被支持
void unimplemented(int);
