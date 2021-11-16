#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<ctime>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define STOP 115 
#define SPACE 32

using namespace std;


class tank_p {
public:
	int map[10][10] = { 0, };
	int tx = 0, ty = 8;
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
	int key = 0;
	int tmp = 0;
public:
	void move(void);
	void print();
	void set();
};

void tank_c::move(void) {
	while (1) {
		if (kbhit()) {
			tmp = _getch();
			if(tmp == 224) tmp = _getch();
			if (tmp == UP || tmp == DOWN || tmp == RIGHT || tmp == LEFT || tmp == STOP) {
				key = tmp;
			}
		}
		if (key == STOP) break;
		switch (key) {
		case UP: {
				if (ty != 0 && map[ty - 1][tx] != 1) {
					map[ty - 1][tx] = 4;
					map[ty][tx] = 0;
					ty--;
			}
			break;
		}
		case DOWN: {
				if (ty != 9 && map[ty + 1][tx] != 1) {
					map[ty + 1][tx] = 5;
					map[ty][tx] = 0;
					ty++;
				}
			break;
		}
		case RIGHT: {
				if (tx != 9 && map[ty][tx + 1] != 1) {
					map[ty][tx + 1] = 2;
					map[ty][tx] = 0;
					tx++;
				}
			break;
		}
		case LEFT: {
				if (tx != 0 && map[ty][tx - 1] != 1) {
					map[ty][tx - 1] = 3;
					map[ty][tx] = 0;
					tx--;
				}
			break;
		}
		}
			print();
			Sleep(500);
	}
}
void tank_c::print() {
	system("cls");
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (map[i][j] == 0) cout << "¡à";
			if (map[i][j] == 1) cout << "¡á";
			if (map[i][j] == 2) cout << "¢º";
			if (map[i][j] == 3) cout << "¢¸";
			if (map[i][j] == 4) cout << "¡ã";
			if (map[i][j] == 5) cout << "¡å";
		}
		cout << endl;
	}
	cout << "\n--------------------\n" << endl;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			if (i == 0) {
				if (map[ty][j] == 0) cout << "  ";
				if (map[ty][j] == 1) cout << "¡á";
				if (map[ty][j] == 2) cout << "¢º";
				if (map[ty][j] == 3) cout << "¢¸";
				if (map[ty][j] == 4) cout << "¡ã";
				if (map[ty][j] == 5) cout << "¡å";
			}
			if (i == 1) cout << "¡à";
		}
		cout << endl;
	}

}
void tank_c::set() {
	print();
	while (1) {
		move();
	}
}
int main() {
	tank_c a;
	a.set();
}