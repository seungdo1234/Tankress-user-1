#include<iostream>
#include <winsock2.h>
#include<conio.h>
#include<Windows.h>
#include<ctime>
#include <stdlib.h>
#include <process.h>


using namespace std;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define STOP 115 
#define SPACE 32
#define PORT 23000 /*서버와 클라이언트간에 데이터를 주고 받을 포트번호*/
#define BUFFER_SIZE  100 


#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)


class tank_p {
public:
	int map[10][10] = { 0, };
	int coordinate[3] = { 8,0,2 };
	int coordinate2[3] = { 0, };
	int coordinate3[3]={ 0, };
	virtual void move(void) = 0;
	tank_p();
};
tank_p::tank_p() {
	map[8][0] = 2;
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
public:
	void Cli_St();
	void move(void);
	void print();
	void set();
	void con();
};

void tank_c::Cli_St() {

	/*WS_32.DLL 을 초기화 */
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
		cout << "WS2_32.DLL 을 초기화 하는데 실패했습니다. " << endl;
		return;
	}
	/*socket 함수를 이용해서 clientSocket을 생성 */
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*주소의 종류 대입*/
	serverAddress.sin_family = AF_INET;
	/*서버의 포트번호 대입*/
	serverAddress.sin_port = htons(PORT);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == -1)
	{
		perror("Could not open data file");
		//cout << "서버에 접속하는데 실패 했습니다 " << endl;
		return;
	}
	cout << "\n  탱크리스 서버에 접속 했습니다 " << endl;
	recv(clientSocket, (char*)index, sizeof(index), 0);
	for (int i = 0; i < 3; i++) {
		index[i] = ntohl(index[i]);
	}
	system("cls");
	cout << "\n  탱크리스를 시작하겠습니다. " << endl;
	Sleep(3000);

}

void tank_c::con() {
	for (int i = 0; i < 3; i++) {
		coordinate[i] = ntohl(coordinate[i]);
	}
	send(clientSocket, (char*)coordinate, sizeof(coordinate), 0);
	recv(clientSocket, (char*)coordinate2, sizeof(coordinate2), 0);
	recv(clientSocket, (char*)coordinate3, sizeof(coordinate3), 0);
	for (int i = 0; i < 3; i++) {
		coordinate[i] = ntohl(coordinate[i]);
		coordinate2[i] = ntohl(coordinate2[i]);
		coordinate3[i] = ntohl(coordinate3[i]);
	}
	map[coordinate2[0]][coordinate2[1]] = coordinate2[2];
	map[coordinate3[0]][coordinate3[1]] = coordinate3[2];
	print();
	map[coordinate2[0]][coordinate2[1]] = 0;
	map[coordinate3[0]][coordinate3[1]] = 0;
}

void tank_c::move(void) {
	while (1) {
		if (kbhit()) {
			tmp = _getch();
			if (tmp == 224) tmp = _getch();
			if (tmp == UP || tmp == DOWN || tmp == RIGHT || tmp == LEFT || tmp == STOP) {
				key = tmp;
			}
		}
		if (key == STOP) break;
		switch (key) {
		case UP:
			if (coordinate[0] != 0 && map[coordinate[0] - 1][coordinate[1]] != 1) {
				map[coordinate[0] - 1][coordinate[1]] = 4;
				map[coordinate[0]][coordinate[1]] = 0;
				coordinate[0] -= 1;
				coordinate[2] = 4;
			}
			break;
		case DOWN:
			if (coordinate[0] != 9 && map[coordinate[0] + 1][coordinate[1]] != 1) {
				map[coordinate[0] + 1][coordinate[1]] = 5;
				map[coordinate[0]][coordinate[1]] = 0;
				coordinate[0] += 1;
				coordinate[2] = 5;
			}
			break;
		case RIGHT:
			if (coordinate[1] != 9 && map[coordinate[0]][coordinate[1] + 1] != 1) {
				map[coordinate[0]][coordinate[1] + 1] = 2;
				map[coordinate[0]][coordinate[1]] = 0;
				coordinate[1] += 1;
				coordinate[2] = 2;
			}
			break;
		case LEFT:
			if (coordinate[1] != 0 && map[coordinate[0]][coordinate[1] - 1] != 1) {
				map[coordinate[0]][coordinate[1] - 1] = 3;
				map[coordinate[0]][coordinate[1]] = 0;
				coordinate[1] -= 1;
				coordinate[2] = 3;
			}
			break;
		}
		con();
	}
}

void tank_c::print() {
	int e[2] = { 0, };
	if (index[1] ==  1) {
		system("cls");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (coordinate[0] == i && coordinate[1] == j) {
					if (map[i][j] == 2) cout << "▶";
					if (map[i][j] == 3) cout << "◀";
					if (map[i][j] == 4) cout << "▲";
					if (map[i][j] == 5) cout << "▼";
				}
				else {
					if (map[i][j] == 0) cout << "□";
					if (map[i][j] == 1) cout << "■";
					if (map[i][j] == 2) cout << "▷";
					if (map[i][j] == 3) cout << "◁";
					if (map[i][j] == 4) cout << "△";
					if (map[i][j] == 5) cout << "▽";
				}
			}
			cout << endl;
		}
		cout << "\n--------------------\n" << endl;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				if (i == 0) {
					if (coordinate[1] == j ) {
						if (map[coordinate[0]][j] == 2) cout << "▶";
						if (map[coordinate[0]][j] == 3) cout << "◀";
						if (map[coordinate[0]][j] == 4) cout << "▲";
						if (map[coordinate[0]][j] == 5) cout << "▼";
					}
					else {
						if (map[coordinate[0]][j] == 0) cout << "  ";
						if (map[coordinate[0]][j] == 1) cout << "■";
						if (map[coordinate[0]][j] == 2) cout << "▷";
						if (map[coordinate[0]][j] == 3) cout << "◁";
						if (map[coordinate[0]][j] == 4) cout << "△";
						if (map[coordinate[0]][j] == 5) cout << "▽";
					}
				}
				if (i == 1) cout << "□";
			}
			cout << endl;
		}
	}
	if (index[1] == 2) {
		system("cls");
		if ((coordinate[2] == 2 && coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6) || (coordinate[2] == 2 && coordinate[0] == coordinate3[0] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6)) {
			if (coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6) e[0] = 1;
			if (coordinate[0] == coordinate3[0] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) cout << "▶";
					else if (coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else if (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else {
						if (map[i][j] == 1) cout << "■";
						else cout << "□";
					}
				}
				cout << endl;
			}
			cout << "1" << endl;
			e[0] = 0;
			e[1] = 0;
		}
		else if ((coordinate[2] == 3 && coordinate[0] == coordinate2[0] && coordinate[1] - coordinate2[1] > -1 && coordinate[1] - coordinate2[1] < 6) || (coordinate[2] == 3 && coordinate[0] == coordinate3[0] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6)) {
			if (coordinate[0] == coordinate2[0] && coordinate[1] - coordinate2[1] > -1 && coordinate[1] - coordinate2[1] < 6) e[0] = 1;
			if (coordinate[0] == coordinate3[0] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) cout << "◀";
					else if (coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else if (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else {
						if (map[i][j] == 1) cout << "■";
						else cout << "□";
					}
				}
				cout << endl;
			}
			cout << "2" << endl;
			e[0] = 0;
			e[1] = 0;
		}
		else if ((coordinate[2] == 4 && coordinate[1] == coordinate2[1] && coordinate[0] - coordinate2[0] > -1 && coordinate[0] - coordinate2[0] < 6) || (coordinate[2] == 4 && coordinate[1] == coordinate3[1] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6)) {
			if (coordinate[1] == coordinate2[1] && coordinate[0] - coordinate2[0] > -1 && coordinate[0] - coordinate2[0] < 6) e[0] = 1;
			if (coordinate[1] == coordinate3[1] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) cout << "▲";
					else if (coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else if (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else {
						if (map[i][j] == 1) cout << "■";
						else cout << "□";
					}
				}
				cout << endl;
			}
			cout << "3" << endl;
			e[0] = 0;
			e[1] = 0;
		}
		else if ((coordinate[2] == 5 && coordinate[1] == coordinate2[1] && coordinate2[0] - coordinate[0] > -1 && coordinate2[0] - coordinate[0] < 6) || (coordinate[2] == 5 && coordinate[1] == coordinate3[1] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6)) {
			if (coordinate[1] == coordinate2[1] && coordinate2[0] - coordinate[0] > -1 && coordinate2[0] - coordinate[0] < 6) e[0] = 1;
			if (coordinate[1] == coordinate3[1] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) cout << "▼";
					else if (coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else if (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1) {
						if (map[i][j] == 2) cout << "▷";
						if (map[i][j] == 3) cout << "◁";
						if (map[i][j] == 4) cout << "△";
						if (map[i][j] == 5) cout << "▽";
					}
					else {
						if (map[i][j] == 1) cout << "■";
						else cout << "□";
					}
				}
				cout << endl;
			}
			cout << "4" << endl;
			e[0] = 0;
			e[1] = 0;
		}
		else {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) {
						if (map[i][j] == 2) cout << "▶";
						if (map[i][j] == 3) cout << "◀";
						if (map[i][j] == 4) cout << "▲";
						if (map[i][j] == 5) cout << "▼";
					}
					else {
						if (map[i][j] == 1) cout << "■";
						else cout << "□";
					}
				}
				cout << endl;
			}
}
	}
	
	Sleep(index[2]);
}
			/*
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) {
						if (map[i][j] == 2) {
							cout << "▶";
							if (coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6) {
								if (map[coordinate2[0]][coordinate2[1]] == 2) cout << "▷";
								if (map[coordinate2[0]][coordinate2[1]] == 3) cout << "◁";
								if (map[coordinate2[0]][coordinate2[1]] == 4) cout << "△";
								if (map[coordinate2[0]][coordinate2[1]] == 5) cout << "▽";
							}
						}
						if (map[i][j] == 3) {
							cout << "◀";
							if (coordinate[0] == coordinate2[0] && coordinate[1] - coordinate2[1] > -1 && coordinate[1] - coordinate2[1] < 6) {
								if (map[coordinate2[0]][coordinate2[1]] == 2) cout << "▷";
								if (map[coordinate2[0]][coordinate2[1]] == 3) cout << "◁";
								if (map[coordinate2[0]][coordinate2[1]] == 4) cout << "△";
								if (map[coordinate2[0]][coordinate2[1]] == 5) cout << "▽";
							}
						}
						if (map[i][j] == 4) {
							cout << "▲";
							if (coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6) {
								if (map[coordinate2[0]][coordinate2[1]] == 2) cout << "▷";
								if (map[coordinate2[0]][coordinate2[1]] == 3) cout << "◁";
								if (map[coordinate2[0]][coordinate2[1]] == 4) cout << "△";
								if (map[coordinate2[0]][coordinate2[1]] == 5) cout << "▽";
							}
						}
						if (map[i][j] == 5) {
							cout << "▼";
							if (coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6) {
								if (map[coordinate2[0]][coordinate2[1]] == 2) cout << "▷";
								if (map[coordinate2[0]][coordinate2[1]] == 3) cout << "◁";
								if (map[coordinate2[0]][coordinate2[1]] == 4) cout << "△";
								if (map[coordinate2[0]][coordinate2[1]] == 5) cout << "▽";
							}
						}
					}
					else {
						if (map[i][j] == 0) cout << "□";
						if (map[i][j] == 1) cout << "■";
					}
				}
				cout << endl;
			}
		}

		else {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) {
						if (map[i][j] == 2) cout << "▶";
						if (map[i][j] == 3) cout << "◀";
						if (map[i][j] == 4) cout << "▲";
						if (map[i][j] == 5) cout << "▼";
					}
					else {
						if (map[i][j] == 1) cout << "■";
						else cout << "□";
					}
				}
				cout << endl;
			}
			cout << "\n\n 1번 플레이어 좌표  " << coordinate[0] << "   " << coordinate[1] << endl;
			cout << " 2번 플레이어 좌표  " << coordinate2[0] <<"   "<< coordinate2[1] << endl;
			cout << " 3번 플레이어 좌표  " << coordinate3[0] << "   " << coordinate3[1] << endl;
		}
		cout << "\n--------------------\n" << endl;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				if (i == 0) {
					if (coordinate[1] == j) {
						if (map[coordinate[0]][j] == 2) cout << "▶";
						if (map[coordinate[0]][j] == 3) cout << "◀";
						if (map[coordinate[0]][j] == 4) cout << "▲";
						if (map[coordinate[0]][j] == 5) cout << "▼";
					}
					else {
						if (map[coordinate[0]][j] == 0) cout << "  ";
						if (map[coordinate[0]][j] == 1) cout << "■";
						if (map[coordinate[0]][j] == 2) cout << "▷";
						if (map[coordinate[0]][j] == 3) cout << "◁";
						if (map[coordinate[0]][j] == 4) cout << "△";
						if (map[coordinate[0]][j] == 5) cout << "▽";
					}
				}
				if (i == 1) cout << "□";
			}
			cout << endl;
		}
	}
	*/
void tank_c::set() {
	Cli_St();
	con();
	print();
	while (1) {
		move();
		con();
	}
}
int main() {
	tank_c a;
	a.set();

	//a.set();
}