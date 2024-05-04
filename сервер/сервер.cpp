#include "myfile.h";
SOCKET Connections[100];
int Counter = 0;

void ClientHandler(int index) {
	srand(time(0));
	int randnum = rand() % 100;
	char msg[256];
	cout << "Загаданное число:" << randnum << endl;
	while (true) {
		int check = recv(Connections[index], msg, sizeof(msg), NULL);
		int guess = atoi(msg);
		if (check > 0) {
			if (guess == randnum) {
				char win[] = "Вы угадали число! Поздравляю, возьмите с полки пирожок!";
				send(Connections[index], win, sizeof(win), NULL);
				break;
			}
			else if (guess < randnum) {
				char bl[] = "Загаданное число больше. Повторите попытку:";
				send(Connections[index], bl, sizeof(bl), NULL);
			}
			else {
				char mn[] = "Загаданное число меньше. Повторите попытку:";
				send(Connections[index], mn, sizeof(mn), NULL);
			}
		}
		else if (check == -1) {
			cout << "\n!!!Соединение с сервером потеряно!!!";
			closesocket(Connections[index]);
			WSACleanup();
			exit(0);
		}
		else exit(0);
	}
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			cout << "Ошибка подключения\n";
		}
		else {
			cout << "Клиент " << i + 1 << " подключился!\n";
			char msg[256] = "Здравствуйте, правила игры: угадать число и зачесть Юленьке лабу:) \nНачните угадывать число:";
			send(newConnection, msg, sizeof(msg), NULL);

			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}
	closesocket(sListen);
	WSACleanup();
	return 0;
}