#include "stdafx.h"
#include "Client.h"

//#pragma comment(lib, "Ws2_32.lib")
//������
Client::Client()
{
	int addrLen = sizeof(SOCKADDR_IN);
	memset(&serverAddr, 0, addrLen);

	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
		cout << "Error - Can not load 'winsock.dll' file" << endl;
	}
	CreateSocker();


	serverAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = 8888;

	//recvFunc = std::bind(&Client::recvAction, this, std::placeholders::_1, std::placeholders::_2);

}

//�Ҹ���
Client::~Client()
{
	Close();
}

// ���� ����
void Client::CreateSocker()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Error - Invalid socket" << endl;
	}

}

//���� ��û
void Client::Connect()
{
	if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cout << WSAGetLastError();
		Close();
		cout << "Error - Fail to connect " << endl;
	}
	else {
		//cout << "Server Connected\n* Enter Message->" << endl;
	}
}


 
//�޼��� �޴� ������
DWORD WINAPI Client::revcThread(void *data)
{
	auto myThis = (Client*)data;
	myThis->revctmp = "test";
	while (true)
	{
		myThis->recvbuffer = recv(myThis->clientSocket, myThis->buffer, 1024, 0);
		if (myThis->recvbuffer > 0) {
			printf("TRACE - Receive message : %s (%d bytes)\n* Enter Message\n->", myThis->buffer, myThis->recvbuffer);
			//myThis->recvAction(myThis->buffer,myThis->recvbuffer);
			myThis->recvAction(myThis->buffer);
		}	
	
	}

	return 0;
}
void Client::recvAction(char* buffer)
{
	
	string command;
	string *token;

	token = StringSplit(buffer, "|");
	
	if (atoi(token[4].c_str()) == 1)
	{
		cout << "���̵� ���� ���" << endl;
		name = token[0];
		command = name + "|0|1|13";
		Send(command);

		//	*(client.buffer) = NULL;
		
	}

	if (atoi(token[4].c_str()) == 0)
	{
		cout << "�̹� ��� ���� ���̵� �Դϴ�." << endl;
		cout << "���̵� �ٽ� �Է����ּ��� : ";
		cin >> name;
		command = name + "|0|1|12";
		Send(command);
	}
	if (atoi(token[3].c_str()) == 11)
	{
		cout << "�� ����" << endl;
	}
}

bool Client::getRecvcheck()
{
	return recvcheck;
}

//Ŭ�� ����
void Client::clientStart()
{
	Connect();
	hThread = CreateThread(NULL, 0, revcThread, (void*)this, 0, NULL);
	CloseHandle(hThread);
}

//�޼��� ������
void Client::Send(string command) {
	char * _msg = new char[1024];
	strcpy_s(_msg, command.size() + 1, command.c_str());
	int _len;
	_len = command.size() + 1;
	//cout << _msg;
	int sendBytes = send(clientSocket, _msg, _len, 0);
	if (sendBytes > 0) {
		//printf("TRACE - Send message : %s (%d bytes)\n", _msg, _len);
	}
}

void Client::Close()
{
	closesocket(clientSocket);
	WSACleanup();
}

string* Client::StringSplit(string strTarget, string strTok)
{
	int     nCutPos;
	int     nIndex = 0;
	string* strResult = new string[1024];

	while ((nCutPos = strTarget.find_first_of(strTok)) != strTarget.npos)
	{
		if (nCutPos > 0)
		{
			strResult[nIndex++] = strTarget.substr(0, nCutPos);
		}
		strTarget = strTarget.substr(nCutPos + 1);
	}

	if (strTarget.length() > 0)
	{
		strResult[nIndex++] = strTarget.substr(0, nCutPos);
	}
	return strResult;
}

string Client::getName()
{
	return name;
}



//���� ����
/*
static DWORD WINAPI revcThread(void *data);
// thread Handle ����
HANDLE hThread;
Client();
~Client();

void CreateSocker(); //���ϻ��� ������
void Connect(); //���� ��û
void Read(); //�޽��� ���
void Send(); //�޽��� ������
void Close(); //��������
*/