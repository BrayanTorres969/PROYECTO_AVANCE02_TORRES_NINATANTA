#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>

using namespace std;


#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80




void gotoxy(int x, int y) {
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);

}

void ocultarCursor() {

	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(hCon, &cci);

}


void pintar_marco() {
	for (int i = 2; i < 78; i++)
	{
		gotoxy(i, 3); printf("%c", 205);
		gotoxy(i, 33); printf("%c", 205);
	}

	for (int i = 4; i < 33; i++)
	{
		gotoxy(2, i); printf("%c", 186);
		gotoxy(77, i); printf("%c", 186);
	}

	gotoxy(2, 3); printf("%c", 201);
	gotoxy(2, 33); printf("%c", 200);
	gotoxy(77, 3); printf("%c", 187);
	gotoxy(77, 33); printf("%c", 188);
}

class AERONAVE {
	int x, y;
	int corazones;
	int vidas;

public:

	AERONAVE(int _x, int _y, int _corazones, int _vidas) : x(_x), y(_y), corazones(_corazones), vidas(_vidas) {}
	int getX() { return x; }
	int getY() { return y; }
	int getVidas() { return vidas; }
	void restarCorazones() { corazones--; }
	void dibujar();
	void borrar();
	void mover();
	void pintar_salud();
	void morir();
};



void AERONAVE::dibujar() {
	gotoxy(x, y); printf("  %c", 65);
	gotoxy(x, y + 1); printf("%c%c%c%c%c", 60, 123, 42, 125, 62);
	gotoxy(x, y + 2); printf("%c%c %c%c", 30, 89, 89, 30);

}

void AERONAVE::borrar() {
	gotoxy(x, y); printf("         ");
	gotoxy(x, y + 1); printf("        ");
	gotoxy(x, y + 2); printf("         ");
}

void AERONAVE::mover() {
	list<char> teclasPresionadas; // Lista para almacenar las teclas presionadas

	// Captura todas las teclas presionadas
	while (_kbhit()) {
		teclasPresionadas.push_back(_getch());
	}

	// Procesa todas las teclas presionadas
	for (char tecla : teclasPresionadas) {
		borrar();
		if (tecla == IZQUIERDA && x > 3) x -= 2;
		if (tecla == DERECHA && x + 6 < 77) x += 2;
		if (tecla == ARRIBA && y > 5) y -= 2;
		if (tecla == ABAJO && y + 4 < 33) y += 2;
	}

	dibujar();
	pintar_salud();
}


void AERONAVE::pintar_salud() {
	gotoxy(50, 2); printf("VIDAS %d", vidas);
	gotoxy(64, 2); printf("Salud");
	gotoxy(70, 2); printf("   ");
	for (int i = 0; i < corazones; i++)
	{
		gotoxy(70 + i, 2); printf("%c", 3);
	}
}

void AERONAVE::morir() {

	if (corazones == 0)
	{
		borrar();
		gotoxy(x, y);   printf("   **   ");
		gotoxy(x, y + 1); printf("  ****  ");
		gotoxy(x, y + 2); printf("   **   ");
		Sleep(200);

		borrar();
		gotoxy(x, y);   printf(" *  **  *");
		gotoxy(x, y + 1); printf("   *****  ");
		gotoxy(x, y + 2); printf(" *  **  *");
		Sleep(200);

		borrar();
		vidas--;
		corazones = 3;
		pintar_salud();
		dibujar();
	}
}


class ALIEN {
	int x, y;

public:
	ALIEN(int _x, int _y) : x(_x), y(_y) {}
	void dibujar();
	void mover();
	void borrar();
	void impactar(class AERONAVE &A);
};

void ALIEN::dibujar() {
	borrar();
	gotoxy(x, y); printf("%c%c%c%c", 123, 64, 64, 125);
	gotoxy(x, y + 1); printf("%c%c%c%c", 47, 34, 34, 92);
	Sleep(100);

	borrar();
	gotoxy(x, y); printf("%c%c%c%c", 123, 64, 64, 125);
	gotoxy(x, y + 1); printf(" %c%c%  ", 92, 47);
	Sleep(100);

	borrar();

}
void ALIEN::mover() {
	y++;
	// Verifica si el alien ha llegado al borde inferior
	if (y >= 32)
	{
		// Si el alien ha llegado al borde inferior, establece su posición en la parte superior de la pantalla
		x = rand() % 71 + 4;
		y = 4;
	}

	dibujar();

}

void ALIEN::borrar() {

	gotoxy(x, y); printf("         ");
	gotoxy(x, y + 1); printf("       ");

}

void ALIEN::impactar(AERONAVE &A) {
	if (x >= A.getX() && x <= A.getX() + 5 && y >= A.getY() && y <= A.getY())
	{
		A.restarCorazones();
		A.dibujar();
		A.pintar_salud();

		x = rand() % 71 + 4;
		y = 4;
	}
}


class MISIL {
	int x, y;

public:
	MISIL(int _x, int _y): x(_x), y(_y){}
	int getX() { return x; }
	int getY() { return y; }
	void mover();
	bool fuera();

};

void MISIL::mover() {
	gotoxy(x, y); printf(" ");
	y--;
	gotoxy(x, y); printf("%c",184);
}

bool MISIL::fuera() {
	if (y == 4) return true;
	return false;
}

int main()
{
	ocultarCursor();
	pintar_marco();
	AERONAVE AN(7, 7, 3, 1);
	AN.dibujar();
	AN.pintar_salud();

	ALIEN alien(10, 4);

	list<MISIL*> M;
	list<MISIL*>::iterator it;


	bool fin_juego = false;
	while (!fin_juego) {
		if (_kbhit()) {
			char tecla = _getch();
			if (tecla == 'x') {
				M.push_back(new MISIL(AN.getX() + 2, AN.getY() - 1));
			}
		}

		for (it = M.begin(); it != M.end();) {
			(*it)->mover();

			if ((*it)->fuera()) {
				gotoxy((*it)->getX(), (*it)->getY()); printf(" ");
				delete (*it);
				it = M.erase(it); // Borra el elemento y actualiza el iterador
			}
			else {
				++it; // Incrementa el iterador solo si no se borra ningún elemento
			}
		}

		alien.mover();
		alien.impactar(AN);
		AN.morir();
		AN.mover();
		Sleep(10);

		if (AN.getVidas() == 0) {
			fin_juego = true;
		}
	}

	gotoxy(35, 15);
	printf("FIN DEL JUEGO");

	return 0;
}

