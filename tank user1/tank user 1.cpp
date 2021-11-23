#include<iostream>
#include <winsock2.h>
#include<conio.h>
#include<Windows.h>
#include<ctime>
#include <stdlib.h>
#include <process.h>


#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define STOP 115 
#define SPACE 32
#define PORT 23000 /*������ Ŭ���̾�Ʈ���� �����͸� �ְ� ���� ��Ʈ��ȣ*/


#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)


class tank_p {
public:
	int map[10][10] = { 0, };
	int coordinate[4] = { 0, };
	int coordinate2[5] = { 0, };
	int coordinate3[5]={ 0, };
	virtual void move(void) = 0;
	tank_p();
};
tank_p::tank_p() {
	map[1][3] = 1;
	map[1][4] = 1;
	map[2][3] = 1;
	map[2][4] = 1;
	map[4][0] = 1;
	map[4][1] = 1;
	map[5][0] = 1;
	map[5][1] = 1;
	map[3][7] = 1;
	map[3][8] = 1;
	map[4][7] = 1;
	map[4][8] = 1;
	map[6][3] = 1;
	map[6][4] = 1;
	map[7][3] = 1;
	map[7][4] = 1;
	map[7][7] = 1;
	map[7][8] = 1;
	map[8][7] = 1;
	map[8][8] = 1;
}
class tank_c : public tank_p {
private:
	WSADATA wsdata;
	SOCKADDR_IN serverAddress;
	SOCKET clientSocket;
	int key = STOP;
	int tmp = 0;
	int index[3] = { 0, };
	int a[2] = { 0 , };
	int me[4] = { 0 };
public:
	void Cli_St();
	void move(void);
	void print();
	void set();
	void con();
};

void tank_c::Cli_St() {
	/*WS_32.DLL �� �ʱ�ȭ */
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
		std::cout << "WS2_32.DLL �� �ʱ�ȭ �ϴµ� �����߽��ϴ�. \n";
		return;
	}
	/*socket �Լ��� �̿��ؼ� clientSocket�� ���� */
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*�ּ��� ���� ����*/
	serverAddress.sin_family = AF_INET;
	/*������ ��Ʈ��ȣ ����*/
	serverAddress.sin_port = htons(PORT);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		perror("Could not open data file");
		//cout << "������ �����ϴµ� ���� �߽��ϴ� " << endl;
		return;
	}
	std::cout << "\n  ��ũ���� ������ ���� �߽��ϴ� \n";
	recv(clientSocket, (char*)me, sizeof(me), 0);
	for (int i = 0; i < 4; i++) {
		me[i] = ntohl(me[i]);
	}
	coordinate[0] = me[1];
	coordinate[1] = me[2];
	coordinate[2] = me[3];
	map[coordinate[0]][coordinate[1]] = coordinate[2];
	recv(clientSocket, (char*)index, sizeof(index), 0);
	for (int i = 0; i < 2; i++) {
		index[i] = ntohl(index[i]);
	} 
	system("cls");
	std::cout << "\n  ��ũ������ �����ϰڽ��ϴ�. \n";
	Sleep(3000);

}

void tank_c::con() {
	if (coordinate[2] == 0) {
		map[coordinate[0]][coordinate[1]] = 0;
	}
	if (coordinate2[2] != 0 && map[coordinate2[0]][coordinate2[1]] != 6 && map[coordinate2[0]][coordinate2[1]] != 7) {
		map[coordinate2[0]][coordinate2[1]] = 0;
	}
	if (coordinate3[2] != 0 && map[coordinate3[0]][coordinate3[1]] != 6 && map[coordinate3[0]][coordinate3[1]] != 7) {
		map[coordinate3[0]][coordinate3[1]] = 0;
	}
	for (int i = 0; i < 4; i++) {
		coordinate[i] = ntohl(coordinate[i]);
	}
	if (me[0] == 1) {
		send(clientSocket, (char*)coordinate, sizeof(coordinate), 0);
		recv(clientSocket, (char*)coordinate2, sizeof(coordinate2), 0);
		recv(clientSocket, (char*)coordinate3, sizeof(coordinate3), 0);
	}
	if (me[0] == 2) {
		recv(clientSocket, (char*)coordinate2, sizeof(coordinate2), 0);
		send(clientSocket, (char*)coordinate, sizeof(coordinate), 0);
		recv(clientSocket, (char*)coordinate3, sizeof(coordinate3), 0);
	}
	if (me[0] == 3) {
		recv(clientSocket, (char*)coordinate2, sizeof(coordinate2), 0);
		recv(clientSocket, (char*)coordinate3, sizeof(coordinate3), 0);
		send(clientSocket, (char*)coordinate, sizeof(coordinate), 0);
	}
	for (int i = 0; i < 4; i++) {
		coordinate[i] = ntohl(coordinate[i]);
		coordinate2[i] = ntohl(coordinate2[i]);
		coordinate3[i] = ntohl(coordinate3[i]);
	}
	if (coordinate2[3] == 1) {
		if (coordinate2[2] == 2) {
			for (int i = 1; i < 4; i++) {
				if (coordinate2[1] + i > 9 || map[coordinate2[0]][coordinate2[1] + i] == 1) {
					break;
				}
				map[coordinate2[0]][coordinate2[1] + i] = 6;
			}
		}
		if (coordinate2[2] == 3) {
			for (int i = 1; i < 4; i++) {
				if (coordinate2[1] - i < 0 || map[coordinate2[0]][coordinate2[1] - i] == 1) {
					break;
				}
				map[coordinate2[0]][coordinate2[1] - i] = 6;
			}
		}
		if (coordinate2[2] == 4) {
			for (int i = 1; i < 4; i++) {
				if (coordinate2[0] - i < 0 || map[coordinate2[0] - i][coordinate2[1]] == 1) {
					break;
				}
				map[coordinate2[0] - i][coordinate2[1]] = 7;
			}
		}
		if (coordinate2[2] == 5) {
			for (int i = 1; i < 4; i++) {
				if (coordinate2[0] + i > 9 || map[coordinate2[0] + i][coordinate2[1]] == 1) {
					break;
				}
				map[coordinate2[0] + i][coordinate2[1]] = 7;
			}
		}
	}
	if (coordinate3[3] == 1) {
		if (coordinate3[2] == 2) {
			for (int i = 1; i < 4; i++) {
				if (coordinate3[1] + i > 9 || map[coordinate3[0]][coordinate3[1] + i] == 1) {
					break;
				}
				map[coordinate3[0]][coordinate3[1] + i] = 6;
			}
		}
		if (coordinate3[2] == 3) {
			for (int i = 1; i < 4; i++) {
				if (coordinate3[1] - i < 0 || map[coordinate3[0]][coordinate3[1] - i] == 1) {
					break;
				}
				map[coordinate3[0]][coordinate3[1] - i] = 6;
			}
		}
		if (coordinate3[2] == 4) {
			for (int i = 1; i < 4; i++) {
				if (coordinate3[0] - i < 0 || map[coordinate3[0] - i][coordinate3[1]] == 1) {
					break;
				}
				map[coordinate3[0] - i][coordinate3[1]] = 7;
			}
		}
		if (coordinate3[2] == 5) {
			for (int i = 1; i < 4; i++) {
				if (coordinate3[0] + i > 9 || map[coordinate3[0] + i][coordinate3[1]] == 1) {
					break;
				}
				map[coordinate3[0] + i][coordinate3[1]] = 7;
			}
		}
	}
	if (map[coordinate[0]][coordinate[1]] == 6 || map[coordinate[0]][coordinate[1]] == 7 && a[1] == 0) {
		a[1] = 1;
		coordinate[2] = 0;
	}
	if (map[coordinate2[0]][coordinate2[1]] == 6) {
		map[coordinate2[0]][coordinate2[1]] = 6;
	}
	else if (map[coordinate2[0]][coordinate2[1]] == 7) {
		map[coordinate2[0]][coordinate2[1]] = 7;
	}
	else {
		if (coordinate2[2] != 0) {
			map[coordinate2[0]][coordinate2[1]] = coordinate2[2];

		}
	}
	if (map[coordinate3[0]][coordinate3[1]] == 6) {
		map[coordinate3[0]][coordinate3[1]] = 6;
	}
	else if (map[coordinate3[0]][coordinate3[1]] == 7) {
		map[coordinate3[0]][coordinate3[1]] = 7;
	}
	else {
		if (coordinate3[2] != 0) {
			map[coordinate3[0]][coordinate3[1]] = coordinate3[2];

		}
	}
	print();
	coordinate[3] = 0;
}

void tank_c::move(void) {
	while (1) {
		if (a[1] == 0) {
		if (kbhit()) {
			tmp = _getch();
			if (tmp == 224) tmp = _getch();
			if (tmp == UP || tmp == DOWN || tmp == RIGHT || tmp == LEFT || tmp == STOP || tmp == SPACE) {
				key = tmp;
			}
		}
		if (key == STOP) {
			a[0] = 1;
			break;
		}
		else if (key == UP || key == DOWN || key == RIGHT || key == LEFT) {
			a[0] = 0;
		}
			switch (key) {
			case UP:
				if (coordinate[0] != 0 && map[coordinate[0] - 1][coordinate[1]] == 0) {
					map[coordinate[0] - 1][coordinate[1]] = 4;
					map[coordinate[0]][coordinate[1]] = 0;
					coordinate[0] -= 1;
					coordinate[2] = 4;
				}
				break; // coordinate[0] + 1 != coordinate2[0] && coordinate[0] + 1 != coordinate3[0]  (coordinate[0] + 1 != coordinate2[0] || coordinate[0] + 1 != coordinate3[0]) && 
			case DOWN:
				if (coordinate[0] != 9 && map[coordinate[0] + 1][coordinate[1]] == 0 ) {
						map[coordinate[0] + 1][coordinate[1]] = 5;
						map[coordinate[0]][coordinate[1]] = 0;
						coordinate[0] += 1;
						coordinate[2] = 5;
				}
				break;
			case RIGHT:
				if (coordinate[1] != 9 && map[coordinate[0]][coordinate[1] + 1] == 0) {
						map[coordinate[0]][coordinate[1] + 1] = 2;
						map[coordinate[0]][coordinate[1]] = 0;
						coordinate[1] += 1;
						coordinate[2] = 2;
				}
				break;
			case LEFT:
				if (coordinate[1] != 0 && map[coordinate[0]][coordinate[1] - 1] == 0) {
						map[coordinate[0]][coordinate[1] - 1] = 3;
						map[coordinate[0]][coordinate[1]] = 0;
						coordinate[1] -= 1;
						coordinate[2] = 3;
				}
				break;
			case SPACE:
				coordinate[3] = 1;
				if (coordinate[2] == 2) {
					if (a[0] == 1) {
						key = STOP;
						a[0] = 0;
					}
					else if (a[0] == 0) { key = RIGHT; }
					for (int i = 1; i < 4; i++) {
						if (coordinate[1] + i > 9 || map[coordinate[0]][coordinate[1] + i] == 1) {
							break;
						}
						map[coordinate[0]][coordinate[1] + i] = 6;

					}
				}
				if (coordinate[2] == 3) {
					if (a[0] == 1) {
						key = STOP;
					}
					else if (a[0] == 0) { key = LEFT; }
					for (int i = 1; i < 4; i++) {
						if (coordinate[1] - i < 0 || map[coordinate[0]][coordinate[1] - i] == 1) {
							break;
						}
						map[coordinate[0]][coordinate[1] - i] = 6;
					}
				}
				if (coordinate[2] == 4) {
					if (a[0] == 1) {
						key = STOP;
					}
					else if (a[0] == 0) { key = UP; }
					for (int i = 1; i < 4; i++) {
						if (coordinate[0] - i < 0 || map[coordinate[0] - i][coordinate[1]] == 1) {
							break;
						}
						map[coordinate[0] - i][coordinate[1]] = 7;
					}
				}
				if (coordinate[2] == 5) {
					if (a[0] == 1) {
						key = STOP;
					}
					else 	if (a[0] == 0) { key = DOWN; }
					for (int i = 1; i < 4; i++) {
						if (coordinate[0] + i > 9 || map[coordinate[0] + i][coordinate[1]] == 1) {
							break;
						}
						map[coordinate[0] + i][coordinate[1]] = 7;
					}
				}
				break;
			}
		}
		con();
	}
}
void tank_c::print() {
	int e[2] = { 0, };
	int b = 0;
	system("cls");
	if (index[0] == 1) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (coordinate[0] == i && coordinate[1] == j && coordinate[2] != 0 && map[coordinate[0]][coordinate[1]] != 0) {
					if (map[i][j] == 2) std::cout << "��";
					if (map[i][j] == 3) std::cout << "��";
					if (map[i][j] == 4) std::cout << "��";
					if (map[i][j] == 5) std::cout << "��";
				}
				else {
					if (map[i][j] == 0) std::cout << "��";
					if (map[i][j] == 1) std::cout << "��";
					if (map[i][j] == 2) std::cout << "��";
					if (map[i][j] == 3) std::cout << "��";
					if (map[i][j] == 4) std::cout << "��";
					if (map[i][j] == 5) std::cout << "��";
					if (map[i][j] == 6) std::cout << "��";
					if (map[i][j] == 7) std::cout << "��";
				}
			}
			std::cout << "\n";
		}
		std::cout << "\n--------------------\n\n";
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				if (i == 0) {
					if (coordinate[1] == j && coordinate[2] != 0 && map[coordinate[0]][coordinate[1]] != 0) {
						if (map[coordinate[0]][j] == 2) std::cout << "��";
						if (map[coordinate[0]][j] == 3) std::cout << "��";
						if (map[coordinate[0]][j] == 4) std::cout << "��";
						if (map[coordinate[0]][j] == 5) std::cout << "��";
					}
					else {
						if (map[coordinate[0]][j] == 0) std::cout << "  ";
						else if (map[coordinate[0]][j] == 1) std::cout << "��";
						else if (map[coordinate[0]][j] == 2) std::cout << "��";
						else if (map[coordinate[0]][j] == 3) std::cout << "��";
						else if (map[coordinate[0]][j] == 4) std::cout << "��";
						else if (map[coordinate[0]][j] == 5) std::cout << "��";
						else if (map[coordinate[0]][j] == 6) std::cout << "��";
					}
				}
				if (i == 1) std::cout << "��";
			}
			std::cout << "\n";
		}
	}
	if (index[0] == 2) {
		if ( (coordinate[2] == 2 && coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6) || (coordinate[2] == 2 && coordinate[0] == coordinate3[0] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6)) {
			if (coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[0] == coordinate3[0] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "��";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "��";
						else if (map[i][j] == 3) std::cout << "��";
						else if (map[i][j] == 4) std::cout << "��";
						else if (map[i][j] == 5) std::cout << "��";
						else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
					}
					else {
						if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
						else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
						else if (map[i][j] == 1) std::cout << "��";
						else std::cout << "��";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j)  std::cout << "��";
						else if (coordinate2[1] == j && e[0] == 1) {
							 if (map[coordinate2[0]][coordinate2[1]] == 2) std::cout << "��";
							else if (map[coordinate2[0]][coordinate2[1]] == 3) std::cout << "��";
							else if (map[coordinate2[0]][coordinate2[1]] == 4) std::cout << "��";
							else if (map[coordinate2[0]][coordinate2[1]] == 5) std::cout << "��";
							else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
						}
						else if ( coordinate3[1] == j && e[1] == 1) {
							if (map[coordinate3[0]][coordinate3[1]] == 2) std::cout << "��";
							if (map[coordinate3[0]][coordinate3[1]] == 3) std::cout << "��";
							if (map[coordinate3[0]][coordinate3[1]] == 4) std::cout << "��";
							if (map[coordinate3[0]][coordinate3[1]] == 5) std::cout << "��";
						}
						else {
							if (map[coordinate[0]][j] == 6 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
							else if (map[coordinate[0]][j] == 7 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
							else if (map[coordinate[0]][j] == 1) std::cout << "��";
							else  std::cout << "  ";
						}
					}
					if (i == 1) std::cout << "��";
				}
				std::cout << "\n";
			}
		}
		else if ((coordinate[2] == 3 && coordinate[0] == coordinate2[0] && coordinate[1] - coordinate2[1] > -1 && coordinate[1] - coordinate2[1] < 6) || (coordinate[2] == 3 && coordinate[0] == coordinate3[0] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6) ) {
			if (coordinate[0] == coordinate2[0] && coordinate[1] - coordinate2[1] > -1 && coordinate[1] - coordinate2[1] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[0] == coordinate3[0] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "��";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "��";
						else if (map[i][j] == 3) std::cout << "��";
						else if (map[i][j] == 4) std::cout << "��";
						else if (map[i][j] == 5) std::cout << "��";
						else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "��";
					}
					else {
						if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "��";
						else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "��";
						else if (map[i][j] == 1) std::cout << "��";
						else std::cout << "��";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j) std::cout << "��";
						else if (coordinate2[1] == j && e[0] == 1) {
							if (map[coordinate2[0]][coordinate2[1]] == 2) std::cout << "��";
							else if (map[coordinate2[0]][coordinate2[1]] == 3) std::cout << "��";
							else  if (map[coordinate2[0]][coordinate2[1]] == 4) std::cout << "��";
							else  if (map[coordinate2[0]][coordinate2[1]] == 5) std::cout << "��";
							else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j)std::cout << "��";
						}
						else if (coordinate3[1] == j && e[1] == 1) {
							if (map[coordinate3[0]][coordinate3[1]] == 2) std::cout << "��";
							if (map[coordinate3[0]][coordinate3[1]] == 3) std::cout << "��";
							if (map[coordinate3[0]][coordinate3[1]] == 4) std::cout << "��";
							if (map[coordinate3[0]][coordinate3[1]] == 5) std::cout << "��";
						}
						else {
							if (map[coordinate[0]][j] == 6 && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "��";
							else if (map[coordinate[0]][j] == 7 && coordinate[1] > j && coordinate[1] - 5 <= j)std::cout << "��";
							else if (map[coordinate[0]][j] == 1)  std::cout << "��";
							else  std::cout << "  ";
						}
					}
					if (i == 1)std::cout << "��";
				}
				std::cout << "\n";
			}
		}
		else if ((coordinate[2] == 4 && coordinate[1] == coordinate2[1] && coordinate[0] - coordinate2[0] > -1 && coordinate[0] - coordinate2[0] < 6) || (coordinate[2] == 4 && coordinate[1] == coordinate3[1] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6)) {
			if (coordinate[1] == coordinate2[1] && coordinate[0] - coordinate2[0] > -1 && coordinate[0] - coordinate2[0] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[1] == coordinate3[1] && coordinate[0] - coordinate3[0] > -1 && coordinate[0] - coordinate3[0] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "��";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "��";
						else if (map[i][j] == 3) std::cout << "��";
						else if (map[i][j] == 4) std::cout << "��";
						else if (map[i][j] == 5) std::cout << "��";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "��";
					}
					else {
						if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "��";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "��";
						else if (map[i][j] == 1) std::cout << "��";
						else std::cout << "��";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j) {
							if (map[coordinate[0]][j] == 4) std::cout << "��";
							if (map[coordinate[0]][j] == 5) std::cout << "��";
						}
						else {
							if (map[coordinate[0]][j] == 0) std::cout << "  ";
							if (map[coordinate[0]][j] == 1) std::cout << "��";
						}
					}
					if (i == 1) std::cout << "��";
				}
				std::cout << "\n";
			}
		}
		else if ( (coordinate[2] == 5 && coordinate[1] == coordinate2[1] && coordinate2[0] - coordinate[0] > -1 && coordinate2[0] - coordinate[0] < 6) || (coordinate[2] == 5 && coordinate[1] == coordinate3[1] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6) ) {
			if (coordinate[1] == coordinate2[1] && coordinate2[0] - coordinate[0] > -1 && coordinate2[0] - coordinate[0] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[1] == coordinate3[1] && coordinate3[0] - coordinate[0] > -1 && coordinate3[0] - coordinate[0] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "��";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "��";
						else if (map[i][j] == 3) std::cout << "��";
						else if (map[i][j] == 4) std::cout << "��";
						else if (map[i][j] == 5) std::cout << "��";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "��";
					}
					else {
						if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "��";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "��";
						else if (map[i][j] == 1) std::cout << "��";
						else std::cout << "��";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j) {
							if (map[coordinate[0]][j] == 4) std::cout << "��";
							if (map[coordinate[0]][j] == 5) std::cout << "��";
						}
						else {
							if (map[coordinate[0]][j] == 0) std::cout << "  ";
							if (map[coordinate[0]][j] == 1) std::cout << "��";
						}
					}
					if (i == 1) std::cout << "��";
				}
				std::cout << "\n";
			}
		}
		else {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j && coordinate[2] !=0 ) {
						if (map[i][j] == 2) std::cout << "��";
						if (map[i][j] == 3) std::cout << "��";
						if (map[i][j] == 4) std::cout << "��";
						if (map[i][j] == 5) std::cout << "��";
					}
					else {
						if (coordinate[2] == 2 ) {
							if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
							else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
							else if (map[i][j] == 1) std::cout << "��";
							else if (map[i][j] == 0) std::cout << "��";
							else std::cout << "��";
						}
						else if (coordinate[2] == 3) {
							if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "��";
							else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "��";
							else if (map[i][j] == 1) std::cout << "��";
							else if (map[i][j] == 0) std::cout << "��";
							else std::cout << "��";
						}
						else if (coordinate[2] == 4 ) {
							if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "��";
							else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "��";
							else if (map[i][j] == 1) std::cout << "��";
							else if (map[i][j] == 0) std::cout << "��";
							else std::cout << "��";
						}
						else if (coordinate[2] == 5) {
							if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "��";
							else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "��";
							else if (map[i][j] == 1) std::cout << "��";
							else if (map[i][j] == 0) std::cout << "��";
							else std::cout << "��";
						}
						else {
							if (map[i][j] == 1) std::cout << "��";
							else std::cout << "��";
						}
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j) {
							if (map[coordinate[0]][j] == 2) std::cout << "��";
							if (map[coordinate[0]][j] == 3) std::cout << "��";
							if (map[coordinate[0]][j] == 4) std::cout << "��";
							if (map[coordinate[0]][j] == 5) std::cout << "��";
						}
						else {
							if (coordinate[2] == 2) {
								if (map[coordinate[0]][j] == 6 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
								else if (map[coordinate[0]][j] == 7 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "��";
								else if (map[coordinate[0]][j] == 1) std::cout << "��";
								else std::cout << "  ";
							}
							else if (coordinate[2] == 3) {
								if (map[coordinate[0]][j] == 6 && coordinate[1] > j && coordinate[1] - 5 <= j)  std::cout << "��";
								else if (map[coordinate[0]][j] == 7  && coordinate[1] > j && coordinate[1] - 5 <= j)  std::cout << "��";
								else if (map[coordinate[0]][j] == 1)  std::cout << "��";
								else  std::cout << "  ";
							}
						}
					}
					if (i == 1) std::cout << "��";
				}
				std::cout << "\n";
			}
		}
	}
	if (coordinate[3] == 1 || coordinate2[3] == 1 || coordinate3[3] == 1) {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (map[i][j] == 6 || map[i][j] == 7) map[i][j] = 0;
			}
		}
	}
	if (coordinate[2] == 0) {
		std::cout << "\n����� ��ũ�� �ν������ϴ�. \n";
	}

	Sleep(index[1]);
}
void tank_c::set() {
	Cli_St();
	while (1) {
		con();
		move();
	}
}
int main() {
	tank_c a;
	a.set();

	//a.set();
}