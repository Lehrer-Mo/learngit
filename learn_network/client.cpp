#define _WINSOCK_DEPRECATED_NO_WARNINGS//为了使用inet_ntoa时不出现警告
#include <WinSock2.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")//Winsock库的引入库

int _tmain(int argc, _TCHAR* argv)
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

	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);//新建一个套接字

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//服务器的IP
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);//服务器的监听端口

	//向服务器发出连接请求
	err = connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == err)//判断连接是否成功
	{
		printf("连接服务器失败，请检查服务器是否启动\n");
		return 0;
	}
	char recvBuf[100];
	recv(socketClient, recvBuf, 100, 0);//接收来自服务器的消息
	printf("收到来自服务器端的信息：%s\n", recvBuf);//打印收到的信息
	send(socketClient, "你好，服务器", strlen("你好，服务器") + 1, 0);//向服务器发送信息

	closesocket(socketClient);//关闭套接字

	WSACleanup();//释放套接字库

	system("pause");

	return 0;
}