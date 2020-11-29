/* 
��ǻ�� ��Ʈ��ũ���� ������ ����� ���� '������'�͵� ���� ������ �����Ѵ�.
�ΰ��� ��ǻ�Ͱ� ����� �� ���� �� ��ǻ���� ������ ���ؼ� �����͸� ��ȯ�Ѵ�.
���� ���α׷����� ũ�� TCP�� UDP ������� ������.

TCP�� ������ ���������̴�. UDP�� ���� �ӵ��� �������� �ŷڼ� �ִ� �������� �ۼ����� ����.
���ͳ� ������ �����͸� �ְ� ���� ���� ��� ������ ȿ�������� �����ϱ� ���� �����͸� ���� ���� ��Ŷ���� ������ �ۼ����մϴ�.

UDP�� �񿬰��� ���������Դϴ� TCP�� ���ؼ� ����� �ŷڼ��� ���������� ���� ������ �ۼ����� �����մϴ�.
*/

//C++ TCP ���� ��� ���α׷�
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

void ShowErrorMessage(std::string message) {
	std::cout << "[���� �߻�]:" << message << "\n";
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876;
	char received[256];
	std::string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientSocket == INVALID_SOCKET){
		ShowErrorMessage("socket()");
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	//127.0.0.1�� �ڱ��ڽ��� ��ǻ�ͷ� ��ӵ� IP �ּ�
	//inet_addr�� ���ڿ�IP �� ��Ʈ��ũ ����Ʈ(long) �������� �ٲ���
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)))
		ShowErrorMessage("connect()");
	std::cout << "[���� ����] connect()\n";

	while (1) {
		std::cout << "[�޼��� ����]: ";
		getline(std::cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			std::cout << "[���� ����]\n";
			break;
		}
		std::cout << "[���� �޽���]: " << received << "\n";
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}