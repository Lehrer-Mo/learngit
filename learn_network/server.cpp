#define _WINSOCK_DEPRECATED_NO_WARNINGS//为了使用inet_ntoa时不出现警告
#include <WinSock2.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")//Winsock库的引入库

int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);//制作Winsock库的版本号
	err = WSAStartup(wVersionRequested, &wsaData);//初始化Winsock库
	if (err != 0)
		return 0;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)//判断返回的版本号对不对
	{
		WSACleanup();
		return 0;
	}

	//创建一个套接字，用于监听客户端的连接
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//使用当前主机任意可用IP

	//人为指定一个可用的IP地址
	//addrSrv.sin_addr.S_un.S_addr = inet_addr("192.168.1.2");

	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);//使用8000端口

	bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//绑定
	listen(sockSrv, 5);//监听

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		printf("--------等待客户端--------\n");

		//从连接请求队列中取出排在最前的一个客户端请求，如果队列为空就阻塞
		SOCKET sockConn = accept(sockSrv, (SOCKADDR*)&addrClient, &len);
		char senBuf[100];
		sprintf_s(senBuf, "欢迎登录服务器（%s）", inet_ntoa(addrClient.sin_addr));//组成字符串
		send(sockConn, senBuf, strlen(senBuf) + 1, 0);//发送字符串给客户端
		char recvBuf[100];
		recv(sockConn, recvBuf, 100, 0);//接收客户端信息
		printf("收到客户端信息：%s\n", recvBuf);//打印收到的客户端信息
		closesocket(sockConn);//关闭和客户端通信的套接字
		puts("是否继续监听？（y/n）");
		char ch[2];
		scanf_s("%s", ch, 2);//读控制台两个字符，包括回车符
		if (ch[0] != 'y')//如果不是y就退出循环
			break;
	}
	closesocket(sockSrv);//关闭监听套接字
	WSACleanup();//释放套接字库
	return 0;
}