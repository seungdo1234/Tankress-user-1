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
#define PORT 23000 /*서버와 클라이언트간에 데이터를 주고 받을 포트번호*/


#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)

HANDLE  mutex;
unsigned  threadID;

class tank_p {
public:
	virtual void move(void) = 0; // 탱크움직이는 추상클래스
	int map[10][10] = { 0, }; //맵
	int coordinate[6] = { 0, }; //각 좌표
	int coordinate2[6] = { 0, };
	int coordinate3[6] = { 0, };
	SOCKET clientSocket; 
	int a[2] = { 0 , }; // 클라이언트 죽음
	int index[3] = { 0, }; // 이동시간, 모드설정, 클라이언트접속수 
	int me[4] = { 0 }; // 서버에서오는 내 상태좌표값
	int key = STOP; 
	int y = 2, q = 0;  //클라이언트 끝내는 변수
	tank_p(); //장애물 찍는 생성자
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
class tank_c : public tank_p { //자식 클래스
private:
	WSADATA wsdata;
	SOCKADDR_IN serverAddress;
	SOCKET clientSocket;
public:
	void Cli_St(); //서버접속
	void move(void); //움직임
	void print(); //맵 출력
	void set(); // 게임 시작 
	void con(); // 서버로 좌표값을 보내고 받는 함수
	~tank_c(); // 승패 여부 출력 소멸자
};

void tank_c::Cli_St() {
	/*WS_32.DLL 을 초기화 */
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
		std::cout << "WS2_32.DLL 을 초기화 하는데 실패했습니다. \n";
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
	std::cout << "\n  탱크리스 서버에 접속 했습니다 \n";
	recv(clientSocket, (char*)me, sizeof(me), 0);
	for (int i = 0; i < 4; i++) {
		me[i] = ntohl(me[i]);
	}
	coordinate[0] = me[1];
	coordinate[1] = me[2];
	coordinate[2] = me[3];
	map[coordinate[0]][coordinate[1]] = coordinate[2];
	recv(clientSocket, (char*)index, sizeof(index), 0);
	for (int i = 0; i < 3; i++) {
		index[i] = ntohl(index[i]);
	}
	system("cls");
	std::cout << "\n\n  탱크리스를 시작하겠습니다. \n";
	Sleep(4000);

}

void tank_c::con() {
	if (coordinate[2] == 0) {
		map[coordinate[0]][coordinate[1]] = 0;
	}
	if (index[2] > 1) {
		if (coordinate2[2] != 0 && map[coordinate2[0]][coordinate2[1]] != 6 && map[coordinate2[0]][coordinate2[1]] != 7) {
			map[coordinate2[0]][coordinate2[1]] = 0;
		}
	}
	if (index[2] > 2) {
		if (coordinate3[2] != 0 && map[coordinate3[0]][coordinate3[1]] != 6 && map[coordinate3[0]][coordinate3[1]] != 7) {
			map[coordinate3[0]][coordinate3[1]] = 0;
		}
	}
	for (int i = 0; i < 6; i++) {
		coordinate[i] = ntohl(coordinate[i]);
	}
	if (me[0] == 1) {
		send(clientSocket, (char*)coordinate, sizeof(coordinate), 0);
		if (q == 1) {
			y = -1;
			coordinate[2] = ntohl(coordinate[2]);
			return;
		}
		else {
			q = 0;
		}
		if (index[2] > 1) {
			y = recv(clientSocket, (char*)coordinate2, sizeof(coordinate2), 0);
		}
		if (index[2] > 2) {
			y = recv(clientSocket, (char*)coordinate3, sizeof(coordinate3), 0);
		}
		if (y <= 0) {
			return;
		}
	}
	if (me[0] == 2) {
		y = recv(clientSocket, (char*)coordinate2, sizeof(coordinate2), 0);
		send(clientSocket, (char*)coordinate, sizeof(coordinate), 0);
		if (q == 1) {
			y = -1;
			coordinate[2] = ntohl(coordinate[2]);
			return;
		}
		else {
			q = 0;
		}
		if (index[2] > 2) {
			y = recv(clientSocket, (char*)coordinate3, sizeof(coordinate3), 0);
		}
		if (y <= 0) {
			return;
		}
	}
	if (me[0] == 3) {
		y = recv(clientSocket, (char*)coordinate2, sizeof(coordinate2), 0);
		y = recv(clientSocket, (char*)coordinate3, sizeof(coordinate3), 0);
		send(clientSocket, (char*)coordinate, sizeof(coordinate), 0);
		if (q == 1) {
			y = -1;
			coordinate[2] = ntohl(coordinate[2]);
			return;
		}
		else {
			q = 0;
		}
		if (y <= 0) {
			return;
		}
	}
	for (int i = 0; i < 6; i++) {
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
	if ((map[coordinate[0]][coordinate[1]] == 6 || map[coordinate[0]][coordinate[1]] == 7 && a[1] == 0)) {
		a[1] = 1;
		coordinate[2] = 0;
		key = STOP;
	}
	print();
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (map[i][j] == 2 || map[i][j] == 3 || map[i][j] == 4 || map[i][j] == 5) {
				q++;
			}
		}
	}
	coordinate[3] = 0;
}

void tank_c::move(void) {
	if (a[1] == 0) {
		while (1) {
			if (kbhit()) {
				int 
					
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
				break; 
			case DOWN:
				if (coordinate[0] != 9 && map[coordinate[0] + 1][coordinate[1]] == 0) {
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
			con();
			if (y <= 0) {
				return;
			}
		}
	}
}
void tank_c::print() {
	int e[2] = { 0, };
	int b = 0;
	system("cls");
	std::cout << "\n " << me[0] << "번 플레이어 입니다. \n";
	if (index[0] == 1) {
		std::cout << "\n    비안개 모드 \n\n";
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				if (coordinate[0] == i && coordinate[1] == j && coordinate[2] != 0 && map[coordinate[0]][coordinate[1]] != 0 && coordinate[2] !=0 ) {
					if (map[i][j] == 2) std::cout << "▶";
					if (map[i][j] == 3) std::cout << "◀";
					if (map[i][j] == 4) std::cout << "▲";
					if (map[i][j] == 5) std::cout << "▼";
				}
				else {
					if (map[i][j] == 0) std::cout << "□";
					if (map[i][j] == 1) std::cout << "■";
					if (map[i][j] == 2) std::cout << "▷";
					if (map[i][j] == 3) std::cout << "◁";
					if (map[i][j] == 4) std::cout << "△";
					if (map[i][j] == 5) std::cout << "▽";
					if (map[i][j] == 6) std::cout << "ㅡ";
					if (map[i][j] == 7) std::cout << "ㅣ";
				}
			}
			std::cout << "\n";
		}
		std::cout << "\n--------------------\n\n";
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 10; j++) {
				if (i == 0) {
					if (coordinate[1] == j && coordinate[2] != 0 && map[coordinate[0]][coordinate[1]] != 0 && coordinate[0] != 0) {
						if (map[coordinate[0]][j] == 2) std::cout << "▶";
						if (map[coordinate[0]][j] == 3) std::cout << "◀";
						if (map[coordinate[0]][j] == 4) std::cout << "▲";
						if (map[coordinate[0]][j] == 5) std::cout << "▼";
					}
					else {
						if (map[coordinate[0]][j] == 1) std::cout << "■";
						else if (map[coordinate[0]][j] == 2) std::cout << "▷";
						else if (map[coordinate[0]][j] == 3) std::cout << "◁";
						else if (map[coordinate[0]][j] == 4) std::cout << "△";
						else if (map[coordinate[0]][j] == 5) std::cout << "▽";
						else if (map[coordinate[0]][j] == 6) std::cout << "ㅡ";
						else if (map[coordinate[0]][j] == 7) std::cout << "ㅣ";
						else  std::cout << "  ";
					}
				}
				if (i == 1) std::cout << "□";
			}
			std::cout << "\n";
		}
	}
	if (index[0] == 2) {
		std::cout << "\n    안개 모드 \n\n";
		if ((coordinate[2] == 2 && coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6) || (coordinate[2] == 2 && coordinate[0] == coordinate3[0] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6)) {
			if (coordinate[0] == coordinate2[0] && coordinate2[1] - coordinate[1] > -1 && coordinate2[1] - coordinate[1] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[0] == coordinate3[0] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "▶";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "▷";
						else if (map[i][j] == 3) std::cout << "◁";
						else if (map[i][j] == 4) std::cout << "△";
						else if (map[i][j] == 5) std::cout << "▽";
						else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅡ";
					}
					else {
						if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅡ";
						else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅣ";
						else if (map[i][j] == 1) std::cout << "■";
						else std::cout << "□";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j)  std::cout << "▶";
						else if (coordinate2[1] == j && e[0] == 1) {
							if (map[coordinate2[0]][coordinate2[1]] == 2) std::cout << "▷";
							else if (map[coordinate2[0]][coordinate2[1]] == 3) std::cout << "◁";
							else if (map[coordinate2[0]][coordinate2[1]] == 4) std::cout << "△";
							else if (map[coordinate2[0]][coordinate2[1]] == 5) std::cout << "▽";
							else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅡ";
						}
						else if (coordinate3[1] == j && e[1] == 1) {
							if (map[coordinate3[0]][coordinate3[1]] == 2) std::cout << "▷";
							if (map[coordinate3[0]][coordinate3[1]] == 3) std::cout << "◁";
							if (map[coordinate3[0]][coordinate3[1]] == 4) std::cout << "△";
							if (map[coordinate3[0]][coordinate3[1]] == 5) std::cout << "▽";
						}
						else {
							if (map[coordinate[0]][j] == 6 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅡ";
							else if (map[coordinate[0]][j] == 7 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅣ";
							else if (map[coordinate[0]][j] == 1) std::cout << "■";
							else  std::cout << "  ";
						}
					}
					if (i == 1) std::cout << "□";
				}
				std::cout << "\n";
			}
		}
		else if ((coordinate[2] == 3 && coordinate[0] == coordinate2[0] && coordinate[1] - coordinate2[1] > -1 && coordinate[1] - coordinate2[1] < 6) || (coordinate[2] == 3 && coordinate[0] == coordinate3[0] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6)) {
			if (coordinate[0] == coordinate2[0] && coordinate[1] - coordinate2[1] > -1 && coordinate[1] - coordinate2[1] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[0] == coordinate3[0] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "◀";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "▷";
						else if (map[i][j] == 3) std::cout << "◁";
						else if (map[i][j] == 4) std::cout << "△";
						else if (map[i][j] == 5) std::cout << "▽";
						else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "ㅡ";
					}
					else {
						if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "ㅡ";
						else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "ㅣ";
						else if (map[i][j] == 1) std::cout << "■";
						else std::cout << "□";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j) std::cout << "◀";
						else if (coordinate2[1] == j && e[0] == 1) {
							if (map[coordinate2[0]][coordinate2[1]] == 2) std::cout << "▷";
							else if (map[coordinate2[0]][coordinate2[1]] == 3) std::cout << "◁";
							else  if (map[coordinate2[0]][coordinate2[1]] == 4) std::cout << "△";
							else  if (map[coordinate2[0]][coordinate2[1]] == 5) std::cout << "▽";
							else if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j)std::cout << "ㅡ";
						}
						else if (coordinate3[1] == j && e[1] == 1) {
							if (map[coordinate3[0]][coordinate3[1]] == 2) std::cout << "▷";
							if (map[coordinate3[0]][coordinate3[1]] == 3) std::cout << "◁";
							if (map[coordinate3[0]][coordinate3[1]] == 4) std::cout << "△";
							if (map[coordinate3[0]][coordinate3[1]] == 5) std::cout << "▽";
						}
						else {
							if (map[coordinate[0]][j] == 6 && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "ㅡ";
							else if (map[coordinate[0]][j] == 7 && coordinate[1] > j && coordinate[1] - 5 <= j)std::cout << "ㅣ";
							else if (map[coordinate[0]][j] == 1)  std::cout << "■";
							else  std::cout << "  ";
						}
					}
					if (i == 1)std::cout << "□";
				}
				std::cout << "\n";
			}
		}
		else if ((coordinate[2] == 4 && coordinate[1] == coordinate2[1] && coordinate[0] - coordinate2[0] > -1 && coordinate[0] - coordinate2[0] < 6) || (coordinate[2] == 4 && coordinate[1] == coordinate3[1] && coordinate[1] - coordinate3[1] > -1 && coordinate[1] - coordinate3[1] < 6)) {
			if (coordinate[1] == coordinate2[1] && coordinate[0] - coordinate2[0] > -1 && coordinate[0] - coordinate2[0] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[1] == coordinate3[1] && coordinate[0] - coordinate3[0] > -1 && coordinate[0] - coordinate3[0] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "▲";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "▷";
						else if (map[i][j] == 3) std::cout << "◁";
						else if (map[i][j] == 4) std::cout << "△";
						else if (map[i][j] == 5) std::cout << "▽";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "ㅣ";
					}
					else {
						if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "ㅡ";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "ㅣ";
						else if (map[i][j] == 1) std::cout << "■";
						else std::cout << "□";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j) {
							if (map[coordinate[0]][j] == 4) std::cout << "▲";
						}
						else {
							if (map[coordinate[0]][j] == 0) std::cout << "  ";
							if (map[coordinate[0]][j] == 1) std::cout << "■";
						}
					}
					if (i == 1) std::cout << "□";
				}
				std::cout << "\n";
			}
		}
		else if ((coordinate[2] == 5 && coordinate[1] == coordinate2[1] && coordinate2[0] - coordinate[0] > -1 && coordinate2[0] - coordinate[0] < 6) || (coordinate[2] == 5 && coordinate[1] == coordinate3[1] && coordinate3[1] - coordinate[1] > -1 && coordinate3[1] - coordinate[1] < 6)) {
			if (coordinate[1] == coordinate2[1] && coordinate2[0] - coordinate[0] > -1 && coordinate2[0] - coordinate[0] < 6 && coordinate2[2] != 0) e[0] = 1;
			if (coordinate[1] == coordinate3[1] && coordinate3[0] - coordinate[0] > -1 && coordinate3[0] - coordinate[0] < 6 && coordinate3[2] != 0) e[1] = 1;
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j) std::cout << "▼";
					else if ((coordinate2[0] == i && coordinate2[1] == j && e[0] == 1) || (coordinate3[0] == i && coordinate3[1] == j && e[1] == 1)) {
						if (map[i][j] == 2) std::cout << "▷";
						else if (map[i][j] == 3) std::cout << "◁";
						else if (map[i][j] == 4) std::cout << "△";
						else if (map[i][j] == 5) std::cout << "▽";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "ㅣ";
					}
					else {
						if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "ㅡ";
						else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "ㅣ";
						else if (map[i][j] == 1) std::cout << "■";
						else std::cout << "□";
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 10; j++) {
					if (i == 0) {
						if (coordinate[1] == j) {
							if (map[coordinate[0]][j] == 5) std::cout << "▼";
						}
						else {
							if (map[coordinate[0]][j] == 0) std::cout << "  ";
							if (map[coordinate[0]][j] == 1) std::cout << "■";
						}
					}
					if (i == 1) std::cout << "□";
				}
				std::cout << "\n";
			}
		}
		else {
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					if (coordinate[0] == i && coordinate[1] == j && coordinate[2] != 0) {
						if (map[i][j] == 2) std::cout << "▶";
						if (map[i][j] == 3) std::cout << "◀";
						if (map[i][j] == 4) std::cout << "▲";
						if (map[i][j] == 5) std::cout << "▼";
					}
					else {
						if (coordinate[2] == 2) {
							if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅡ";
							else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅣ";
							else if (map[i][j] == 1) std::cout << "■";
							else if (map[i][j] == 0) std::cout << "□";
							else std::cout << "□";
						}
						else if (coordinate[2] == 3) {
							if (map[i][j] == 6 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "ㅡ";
							else if (map[i][j] == 7 && i == coordinate[0] && coordinate[1] > j && coordinate[1] - 5 <= j) std::cout << "ㅣ";
							else if (map[i][j] == 1) std::cout << "■";
							else if (map[i][j] == 0) std::cout << "□";
							else std::cout << "□";
						}
						else if (coordinate[2] == 4) {
							if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "ㅡ";
							else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] > i && coordinate[0] - 5 <= i) std::cout << "ㅣ";
							else if (map[i][j] == 1) std::cout << "■";
							else if (map[i][j] == 0) std::cout << "□";
							else std::cout << "□";
						}
						else if (coordinate[2] == 5) {
							if (map[i][j] == 6 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "ㅡ";
							else if (map[i][j] == 7 && j == coordinate[1] && coordinate[0] < i && coordinate[0] + 5 >= i) std::cout << "ㅣ";
							else if (map[i][j] == 1) std::cout << "■";
							else if (map[i][j] == 0) std::cout << "□";
							else std::cout << "□";
						}
						else {
							if (map[i][j] == 1) std::cout << "■";
							else std::cout << "□";
						}
					}
				}
				std::cout << "\n";
			}
			std::cout << "\n--------------------\n\n";
			if (coordinate[2] == 4 || coordinate[2] == 5) {
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 10; j++) {
						if (i == 0) {
							if (coordinate[1] == j) {
								if (map[coordinate[0]][j] == 5) std::cout << "▼";
								if (map[coordinate[0]][j] == 4) std::cout << "▲";
							}
							else {
								if (map[coordinate[0]][j] == 1) std::cout << "■";
								else std::cout << "  ";
							}
						}
						if (i == 1) std::cout << "□";
					}
					std::cout << "\n";
				}
			}
			else {
				for (int i = 0; i < 2; i++) {
					for (int j = 0; j < 10; j++) {
						if (i == 0) {
							if (coordinate[1] == j) {
								if (map[coordinate[0]][j] == 2) std::cout << "▶";
								if (map[coordinate[0]][j] == 3) std::cout << "◀";
							}
							else {
								if (coordinate[2] == 2) {
									if (map[coordinate[0]][j] == 6 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅡ";
									else if (map[coordinate[0]][j] == 7 && coordinate[1] < j && coordinate[1] + 5 >= j) std::cout << "ㅣ";
									else if (map[coordinate[0]][j] == 1) std::cout << "■";
									else std::cout << "  ";
								}
								else if (coordinate[2] == 3) {
									if (map[coordinate[0]][j] == 6 && coordinate[1] > j && coordinate[1] - 5 <= j)  std::cout << "ㅡ";
									else if (map[coordinate[0]][j] == 7 && coordinate[1] > j && coordinate[1] - 5 <= j)  std::cout << "ㅣ";
									else if (map[coordinate[0]][j] == 1)  std::cout << "■";
									else  std::cout << "  ";
								}
							}
						}
						if (i == 1) std::cout << "□";
					}
					std::cout << "\n";
				}
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
		std::cout << "\n당신의 탱크는 부숴졌습니다. \n";
	}
	Sleep(index[1]);
}
void tank_c::set() {
	Cli_St();
	while (1) {
		con();
		move();
		if (y <= 0) {
			return;
		}
	}
}
tank_c :: ~tank_c() {
	system("cls");
	closesocket(clientSocket);
	if (coordinate[2] !=0) {
		std::cout << "\n   You Win  !!!!!!\n\n";
	}
	else std::cout << "\n   You Lose  ㅠㅠ \n\n";

	system("PAUSE");
}
int main() {
	tank_c a;
	a.set();
}