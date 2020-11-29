#include<iostream>
#include<string>
#include<winsock2.h>
//�����̶� �������� �������� Host(���)���� ������ ������
//�������� �������� Host�� �ƴ϶� ���μ��� �ε� Host=> ���ϴ� ���μ����� �����ִ� �� �� ���� ����
void ShowErrorMessage(std::string message) {
	std::cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock�� �ʱ�ȭ �մϴ�.
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0);// TCP ������ �����մϴ�.
	//socket(��Ʈ��ũ �ּ� ü��,���� Ÿ��, ��������)
	//��Ʈ��ũ �ּ� ü��:IPv4(AF_INET,PF_INET),IPv6(AF_INET6)
	//���� Ÿ��:TCP(SOCK_STREAM),UDP(SOCK_DGRAM)
	//��������:���Ÿ� ��� ��� ���̿��� �޼����� �ְ� �޴� ��İ� ��Ģ�� ü��
	//��������: TCP(IPPROPTO_TCP), UDP(IPPROTO_UDP)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);//4����Ʈ ������ ��Ʈ��ũ ����Ʈ ����
	//�ڽ��� ȣ��Ʈ�� ������ ��Ŷ�� ������ ����(�̴� �ϳ��� ��ǻ�Ϳ��� �ٸ� �ּҰ� ������ �־ �̿�)
	//��Ŷ�� ������ ������ �����μ�  �ڼ��� ������ �̰��� ���� https://youngq.tistory.com/73
	serverAddress.sin_port = htons(serverPort);
	if(bind(serverSocket,(SOCKADDR*)&serverAddress,sizeof(serverAddress))==SOCKET_ERROR)
		ShowErrorMessage("bind()");//bind(���� ����, ���� �ּ� ����ü, ���� �ּ� ����ü�� ũ��)

	std::cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) {
		ShowErrorMessage("listen()");
	}//listen(���� ����, �� �α� ť�� ũ��
	//�� �α׶� ���ÿ� ������ �õ��ϴ� �ִ� Ŭ���̾�Ʈ ���� �ǹ��Ѵ�.
	std::cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	//accept(���� ����, Ŭ���̾�Ʈ �ּ� ����ü ����, Ŭ���̾�Ʈ �ּ� ����ü ũ��)
	std::cout << "[���� ����] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		//recv(���� ����,���ڿ� ����,���ڿ� ���� ũ��,�÷���) Ʋ���� ��찡 �ƴ϶�� 0�� �����Ѵ�.
		received[length] = NULL;
		std::cout << "[Ŭ���̾�Ʈ �޼���]: " << received << '\n';
		std::cout << "[�޽��� ����]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			std::cout << "[���� ����]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
		//send(���� ����, ���ڿ� ����, ���ڿ� ������ ũ��, �÷���)
	}
	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;


}