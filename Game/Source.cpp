#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include <thread>
#include<time.h>
#define screen_x 120
#define screen_y 40
int s = 0;
HANDLE wHnd;
CHAR_INFO consoleBuffer[screen_x * screen_y];
COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
struct bullet {
	int x;
	int y;
	int state;
};
bullet b[10];

int setConsole(int, int);
void fill_buffer_to_console();
void setcursor(bool);
void gotoxy(int, int);
char cursor(int, int);
void setcolor(int, int);
void draw_player(int, int);
void clear_player(int, int);
void draw_crystal(int, int);
void draw_bullet(int, int);
void clear_bullet(int, int);
void score(int);
void clear_buffer();

int main() {
	clear_buffer();
	setConsole(screen_x, screen_y);
	setcursor(0);
	char ch = '.';
	char direction = 0;
	char directiony = 0;
	srand(time(NULL));
	int px = 40, py = 19;
	int n = 0;
	int by[10];
	int by3[10];
	int dl = 0, dr = 0, ds = 0, dn = 0;
	draw_player(px, py);
	do {
		draw_crystal(40, 20);

		if (_kbhit()) {


			ch = _getch();
			if (ch == 'a') { direction = 'L'; }
			if (ch == 'd') { direction = 'R'; }
			if (ch == 'w') { directiony = 'N'; }
			if (ch == 's') { directiony = 'S'; }
			if (ch == 'x') { direction = 0; directiony = 0; }
			if (ch == 'W') { directiony = 'N'; direction = 0; }
			if (ch == 'S') { directiony = 'S'; direction = 0; }
			if (ch == 'A') { direction = 'L';  directiony = 0; }
			if (ch == 'D') { direction = 'R'; directiony = 0; }
			if (ch == 'i' && b[n].state == 0) { b[n].state = 1; b[n].x = px;  b[n].y = py - 1; }
			if (ch == 'l' && b[n].state == 0) { b[n].state = 2; b[n].x = px + 2;  b[n].y = py; }
			if (ch == 'k' && b[n].state == 0) { b[n].state = 3; b[n].x = px;  b[n].y = py + 1; }
			if (ch == 'j' && b[n].state == 0) { b[n].state = 4; b[n].x = px - 2;  b[n].y = py; }
			n++;
			if (n > 9)n = 0;
			fflush(stdin);
		}
		if (direction == 'L' || direction == 'R' || directiony == 'N' || directiony == 'S')
		{
			if (direction == 'L' && px >= 1) { dl += 1; if (dl%2==0) { clear_player(px, py); draw_player(--px, py); } }
			else if (direction == 'L' && px < 1)direction = 0;
			if (direction == 'R' && px < 80) { dr += 1; if (dr % 2 == 0) { clear_player(px, py); draw_player(++px, py); } }
			else if (direction == 'R' && px >= 80)direction = 0;
			if (directiony == 'S' && py < 39) { ds += 1; if (ds % 4 == 0) { clear_player(px, py); draw_player(px, ++py); } }
			else if (directiony == 'S' && py >= 39)direction = 0;
			if (directiony == 'N' && py >= 1) { dn += 1; if (dn % 4 == 0) { clear_player(px, py); draw_player(px, --py); } }
			else if (directiony == 'N' && py < 1)direction = 0;
		}
		for (int i = 0; i < 10; i++) {
			if (b[i].state == 1)
			{
				by[i] += 1;
				if (by[i]%2==0){
					clear_bullet(b[i].x, b[i].y);
				if (b[i].y >= 1) {
					if (cursor(b[i].x, b[i].y - 1) == 'O') {
						b[i].state = 0;
						std::thread q(Beep, 900, 500);
						q.detach();
						score(1);
					}
					else draw_bullet(b[i].x, --b[i].y);

				}
				else b[i].state = 0;
				fill_buffer_to_console();
			    }
				
			}
			if (b[i].state == 2)
			{
				
					clear_bullet(b[i].x, b[i].y);
					if (b[i].x < 80) {
						if (cursor(b[i].x + 1, b[i].y) == 'O') {
							b[i].state = 0;
							std::thread q(Beep, 900, 500);
							q.detach();
							score(1);
						}
						else draw_bullet(++b[i].x, b[i].y);

					}
					else b[i].state = 0;
					fill_buffer_to_console();
				
			}
			if (b[i].state == 3)
			{
				by3[i] += 1;
				if (by3[i] % 2 == 0) {
					clear_bullet(b[i].x, b[i].y);
					if (b[i].y < 39) {
						if (cursor(b[i].x, b[i].y + 1) == 'O') {
							b[i].state = 0;
							std::thread q(Beep, 900, 500);
							q.detach();
							score(1);
						}
						else draw_bullet(b[i].x, ++b[i].y);

					}
					else b[i].state = 0;
					fill_buffer_to_console();
				}
				
			}
			if (b[i].state == 4)
			{
				
					clear_bullet(b[i].x, b[i].y);
					if (b[i].x > 1) {
						if (cursor(b[i].x - 1, b[i].y) == 'O') {
							b[i].state = 0;
							std::thread q(Beep, 900, 500);
							q.detach();
							score(1);
						}
						else draw_bullet(--b[i].x, b[i].y);

					}
					else b[i].state = 0;
					fill_buffer_to_console();
				
			}
		}
		fill_buffer_to_console();


		Sleep(100);

	} while (ch != VK_ESCAPE);

	return 0;
}

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}
void fill_buffer_to_console()
{
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos,
		&windowSize);
}
void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}
void setcolor(int fg, int bg)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void draw_player(int x, int y) {
	consoleBuffer[x - 1 + screen_x * y].Char.AsciiChar = '|';
	consoleBuffer[x - 1 + screen_x * y].Attributes = 7;
	consoleBuffer[x + screen_x * y].Char.AsciiChar = 'o';
	consoleBuffer[x + screen_x * y].Attributes = 7;
	consoleBuffer[x + 1 + screen_x * y].Char.AsciiChar = '|';
	consoleBuffer[x + 1 + screen_x * y].Attributes = 7;
}
void draw_crystal(int x, int y) {
	consoleBuffer[x - 1 + screen_x * y].Char.AsciiChar = '<';
	consoleBuffer[x - 1 + screen_x * y].Attributes = 9;
	consoleBuffer[x + screen_x * y].Char.AsciiChar = 4;
	consoleBuffer[x + screen_x * y].Attributes = 9;
	consoleBuffer[x + 1 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 1 + screen_x * y].Attributes = 9;
	consoleBuffer[x + 2 + screen_x * y].Char.AsciiChar = '>';
	consoleBuffer[x + 2 + screen_x * y].Attributes = 9;
}
void clear_player(int x, int y) {
	consoleBuffer[x - 1 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x - 1 + screen_x * y].Attributes = 0;
	consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + screen_x * y].Attributes = 0;
	consoleBuffer[x + 1 + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + 1 + screen_x * y].Attributes = 0;
}
void draw_bullet(int x, int y) {

	consoleBuffer[x + screen_x * y].Char.AsciiChar = 'o';
	consoleBuffer[x + screen_x * y].Attributes = 2;

}
void clear_bullet(int x, int y) {
	consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
	consoleBuffer[x + screen_x * y].Attributes = 0;
}
void score(int x) {
	setcolor(2, 0);
	gotoxy(82, 0);
	s += x;
	printf("Score : %d", s);
}
void clear_buffer() {
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';
			consoleBuffer[x + screen_x * y].Attributes = 7;
		}
	}
}