#include<iostream>
#include<string>
#include<winsock2.h>
//소켓이란 데이터의 종착지로 Host(기기)간의 데이터 종착지
//데이터의 종착지는 Host가 아니라 프로세스 인데 Host=> 원하는 프로세스로 보내주는 길 과 같은 느낌
void ShowErrorMessage(std::string message) {
	std::cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock을 초기화 합니다.
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);// TCP 소켓을 생성합니다.
	//socket(네트워크 주소 체계,소켓 타입, 프로토콜)
	//네트워크 주소 체계:IPv4(AF_INET,PF_INET),IPv6(AF_INET6)
	//소켓 타입:TCP(SOCK_STREAM),UDP(SOCK_DGRAM)
	//프로토콜:원거리 통신 장비 사이에서 메세지를 주고 받는 양식과 규칙의 체계
	//프로토콜: TCP(IPPROPTO_TCP), UDP(IPPROTO_UDP)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);//4바이트 정수를 네트워크 바이트 형식
	//자신의 호스트에 들어오는 패킷을 무조건 수신(이는 하나의 컴퓨터에도 다른 주소가 있을수 있어서 이용)
	//패킷은 데이터 전송의 단위로서  자세한 내용은 이곳을 참조 https://youngq.tistory.com/73
	serverAddress.sin_port = htons(serverPort);
	if(bind(serverSocket,(SOCKADDR*)&serverAddress,sizeof(serverAddress))==SOCKET_ERROR)
		ShowErrorMessage("bind()");//bind(소켓 변수, 서버 주소 구조체, 서버 주소 구조체의 크기)

	std::cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		ShowErrorMessage("listen()");
	}//listen(소켓 변수, 백 로그 큐의 크기
	//백 로그란 동시에 연결을 시도하는 최대 클라이언트 수를 의미한다.
	std::cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	//accept(소켓 변수, 클라이언트 주소 구조체 변수, 클라이언트 주소 구조체 크기)
	std::cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		//recv(소켓 변수,문자열 버퍼,문자열 버프 크기,플래그) 틀별한 경우가 아니라면 0을 설정한다.
		received[length] = NULL;
		std::cout << "[클라이언트 메세지]: " << received << '\n';
		std::cout << "[메시지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			std::cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
		//send(소켓 변수, 문자열 버퍼, 분자열 버퍼의 크기, 플래그)
	}
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;


}