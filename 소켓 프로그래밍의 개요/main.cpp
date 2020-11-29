/* 
컴퓨터 내트워크에서 소켓은 통신을 위한 '종착지'와도 같은 역할을 수행한다.
두개의 컴퓨터가 통신을 할 떄는 각 컴퓨터의 소켓을 통해서 데이터를 교환한다.
소켓 프로그래밍은 크게 TCP와 UDP 방식으로 나뉜다.

TCP는 연결형 프로토콜이다. UDP에 비해 속도가 느리지만 신뢰성 있는 데이터의 송수신을 보장.
인터넷 내에서 데이터를 주고 받을 떄는 경로 설정을 효과적으로 수행하기 위해 데이터를 여러 개의 패킷으로 나누어 송수신합니다.

UDP는 비연결형 프로토콜입니다 TCP에 비해서 통신의 신뢰성이 부족하지만 빠른 데이터 송수신이 가능합니다.
*/

//C++ TCP 에코 통신 프로그램
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <winsock2.h>

void ShowErrorMessage(std::string message) {
	std::cout << "[오류 발생]:" << message << "\n";
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
	//127.0.0.1은 자기자신의 컴퓨터로 약속된 IP 주소
	//inet_addr는 문자열IP 를 네트워크 바이트(long) 형식으로 바꿔줌
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)))
		ShowErrorMessage("connect()");
	std::cout << "[현재 상태] connect()\n";

	while (1) {
		std::cout << "[메세지 전송]: ";
		getline(std::cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			std::cout << "[서버 종료]\n";
			break;
		}
		std::cout << "[서버 메시지]: " << received << "\n";
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}