#define _WINSOCK_DEPRECATED_NO_WARNINGS//Ϊ��ʹ��inet_ntoaʱ�����־���
#include <WinSock2.h>
#include <tchar.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")//Winsock��������

int _tmain(int argc, _TCHAR* argv)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);//����Winsock��İ汾��
	err = WSAStartup(wVersionRequested, &wsaData);//��ʼ��Winsock��
	if (err != 0)
		return 0;

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)//�жϷ��صİ汾�ŶԲ���
	{
		WSACleanup();
		return 0;
	}

	SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);//�½�һ���׽���

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//��������IP
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);//�������ļ����˿�

	//�������������������
	err = connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (SOCKET_ERROR == err)//�ж������Ƿ�ɹ�
	{
		printf("���ӷ�����ʧ�ܣ�����������Ƿ�����\n");
		return 0;
	}
	char recvBuf[100];
	recv(socketClient, recvBuf, 100, 0);//�������Է���������Ϣ
	printf("�յ����Է������˵���Ϣ��%s\n", recvBuf);//��ӡ�յ�����Ϣ
	send(socketClient, "��ã�������", strlen("��ã�������") + 1, 0);//�������������Ϣ

	closesocket(socketClient);//�ر��׽���

	WSACleanup();//�ͷ��׽��ֿ�

	system("pause");

	return 0;
}