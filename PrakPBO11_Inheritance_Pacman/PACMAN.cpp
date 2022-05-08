#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
using namespace std;

void gotoxy(int x,int y)
{
	COORD k = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),k);
}
class CObjek
{
public:
	int X, Y;
	char Z;
	CObjek(int x=0,int y=0,char z=NULL)
	{
		X = x;
		Y = y;
		Z = z;
	}
	int getX() { return X; }
	int getY() { return Y; }
	char getChar() { return Z; }
	void setX(int x) { X = x; }
	void setY(int y) { Y = y; }
	void setChar(char z) { Z = z; }
	void draw() { gotoxy(X, Y); cout << Z; }
};
class CMoveableObject :public CObjek
{
public:
	int DX, DY,lastX,lastY;
	CMoveableObject(int x = 0, int y = 0, char z = NULL, int dx = 0, int dy = 0) :CObjek(x, y, z)
	{
		DX = dx;
		DY = dy;
		lastX = x;
		lastY = y;
	}
	int getDX(){return DX;}
	int getDY(){return DY;}
	void setDX(int x){DX = x;}
	void setDY(int y){DY = y;}
	void draw() { gotoxy(X, Y); cout << Z; }
	void updates()
	{
		lastX = X;
		lastY = Y;
		X += DX;
		Y += DY;
	}
	void SetDirection(int direction)
	{
		if (direction == 1)
		{
			DX = 1;
			DY = 0;
		}
		else if (direction == 2)
		{
			DX = -1;
			DY = 0;
		}
		else if (direction == 3)
		{
			DX = 0;
			DY = 1;
		}
		else if (direction == 4)
		{
			DX = 0;
			DY = -1;
		}
	}
};
class CPlayer :public CMoveableObject
{
protected:
	int direction;
public:
	CPlayer(int dir=0,int x = 1, int y = 1, char z = 'o' , int dx = 0, int dy = 0) :CMoveableObject(x, y, z, dx, dy)
	{
		direction = dir;
	}
	
	void updates()
	{
		lastX = X;
		lastY = Y;
		X += DX;
		Y += DY;
		if (X <= 1)
		{
			if (DX == 1 || DX == -1) {
				DX = 0;
				DY = 0;
			}
		}
		else if (X>=28)
		{
			if (DX == 1 || DX == -1) {
				DX = 0;
				DY = 0;
			}
		}
		else if (Y <= 1)
		{
			if (DY == 1 || DY == -1) {
				DX = 0;
				DY = 0;
			}
		}
		else if (Y >= 13)
		{
			if (DY == 1 || DY == -1) {
				DX = 0;
				DY = 0;
			}
		}
		if ((X == 1 && Y == 1)||(X==1&&Y==13)||(X==28&&Y==1)||(X==28&&Y==13))
		{
			DX = 0;
			DY = 0;
		}
	}
	void draw()
	{
		gotoxy(lastX, lastY); cout << " ";
		gotoxy(X, Y); cout << Z;
	}
	void SetDirection(int i)
	{
		direction = i;
		if (direction==1)
		{
			DX = 0;
			DY = -1;
		}
		else if (direction==2)
		{
			DX = 0;
			DY = 1;
		}
		else if (direction==3)
		{
			DX = -1;
			DY = 0;
		}
		else if (direction==4)
		{
			DX = 1;
			DY = 0;
		}
		else if (direction==5)
		{
			DX = 0;
			DY = 0;
		}
	}
};
class CEnemy:public CMoveableObject
{
public:
	CMoveableObject enemy[5];
	CEnemy(int x=0,int y=0,char z='#',int dx=0,int dy=0,int j=3):CMoveableObject(x,y,z,dx,dy)
	{
		lastX = x;
		lastY = y;
	}
	void SetDirection()
	{
		int direction;
		srand(time(NULL));
		for (int i = 0; i < 5; i++) {
			direction = rand() % 4 + 1;
			enemy[i].setX(rand() % 22 + 2);
			enemy[i].setY(rand() % 10 + 2);
			if (direction == 1)
			{
				enemy[i].DX = 1;
				enemy[i].DY = 0;
			}
			else if (direction == 2)
			{
				enemy[i].DX = -1;
				enemy[i].DY = 0;
			}
			else if (direction == 3)
			{
				enemy[i].DX = 0;
				enemy[i].DY = 1;
			}
			else if (direction == 4)
			{
				enemy[i].DX = 0;
				enemy[i].DY = -1;
			}
		}
	}
	void updates()
	{
		for (int i = 0; i < 5; i++) {
			enemy[i].lastX = enemy[i].getX(), enemy[i].lastY = enemy[i].getY();
			enemy[i].X += enemy[i].DX;
			enemy[i].Y += enemy[i].DY;
			if (enemy[i].X >= 28)
				enemy[i].DX = -1;
			if (enemy[i].X <= 1)
				enemy[i].DX = 1;
			if (enemy[i].Y >= 13)
				enemy[i].DY = -1;
			if (enemy[i].Y <= 1)
				enemy[i].DY = 1;
		}
	}
	void draw()
	{
		for (int i = 0; i < 5; i++) {
			gotoxy(enemy[i].lastX, enemy[i].lastY); cout << " ";
			gotoxy(enemy[i].X, enemy[i].Y); cout << Z;
		}
	}
	
};
class CFood :public CObjek
{
public:
	CObjek**food;
	int foodX,foodY;
	CFood(int x = 0, int y = 0, char z = 0, int a=14,int b=7) :CObjek(x, y, z)
	{ 
		foodX = b;
		foodY = a;
		food = new CObjek*[foodX];
		for (int i = 0; i < foodX; i++)
			food[i] = new CObjek[foodY];
		for (int i = 0; i < foodX; i++)
		{
			for (int j = 0; j < foodY; j++)
			{
				food[i][j].setChar(' ');
			}
		}
	}
	void initialize()
	{
		for (int i = 0; i < foodX; i++)
		{
			for (int j = 0; j < foodY; j++)
			{
				food[i][j].setChar('.');
			}
		}
	}
	void draw()
	{
		for (int i = 0; i < foodX; i++)
		{
			for (int j = 0; j < foodY; j++)
			{
				gotoxy(CObjek::X + j * 2, CObjek::Y + i * 2);
				cout<<food[i][j].Z;
				
			}
		}
	}
	
};
class CGarisHorizon : public CObjek
{
protected:
	int panjang;
public:
	CGarisHorizon(int x = 0, int y = 0, char z = NULL, int p = 0) :CObjek(x, y, z)
	{
		panjang = p;
	}
	int getPanjang()
	{
		return panjang;
	}
	void setPanjang(int p)
	{
		panjang = p;
	}

};
class CGarisVertikal : public CGarisHorizon
{
public:
	CGarisVertikal(int x = 0,int y=0,int z=NULL,int p=0):CGarisHorizon(x,y,z,p)
	{ }
};
class CArena:public CGarisVertikal
{
protected:
	int tinggi;
public:
	CArena(int x = 0, int y = 0, char z = NULL, int p = 0, int t = 0) :CGarisVertikal(x, y, z, p)
	{
		tinggi = t;
	}
	int getTinggi()
	{
		return tinggi;
	}
	void setTinggi(int t)
	{
		tinggi = t;
	}
	void draw()
	{
		for (int i = 0; i < panjang; i++)
		{
			for (int j = 0; j < tinggi; j++)
			{
				gotoxy(X + i, Y + j);
				if (j == 0 || j == tinggi - 1)
				{
					Z = '-';
					cout << Z;
				}
				else if (i == 0 || i == panjang - 1)
				{
					Z = '|';
					cout << Z;
				}
				else
					cout << " ";
			}
			cout << endl;
		}
	}
};
class CGame
{
protected:
	CPlayer player;
	CArena area;
	CFood makanan;
	CEnemy musuh;
	char inp;
	int score=0;
	bool status=false;
	int counter = 0;
	int temp;
public:
	void installAll()
	{
		area.setTinggi(15);
		area.setPanjang(30);
		player.X = 1;
		player.Y = 1;
		player.Z = 'o';
		player.DX = 0;
		player.DY = 0;
		musuh.setChar('#');
		musuh.SetDirection();
		makanan.X = 1;
		makanan.Y = 1;
		makanan.Z = '.';
		makanan.initialize();
		area.draw();
	
	}
	void move()
	{
		if (_kbhit())
		{
			inp = _getch();
			if (inp == 'W' || inp == 'w')
			{
				player.SetDirection(1);

			}
			if (inp == 'A' || inp == 'a')
			{
				player.SetDirection(3);
			}
			if (inp == 'S' || inp == 's')
			{
				player.SetDirection(2);
			}
			if (inp == 'D' || inp == 'd')
			{
				player.SetDirection(4);
			}
			if (inp == ' ')
			{
				player.SetDirection(5);
			}
		}
	}

	void updates()	
	{
		DWORD waktu = GetTickCount();
		waktu /= 1000;
		DWORD seconds = waktu % 60;
		waktu /= 3600;
		musuh.updates();
		player.updates();
		for (int i = 0; i < 7; i++) {
			for (int j = 0; j < 14; j++) {
				if (player.X == makanan.food[i][j].X+j*2+1 && player.Y == makanan.food[i][j].Y+i*2+1&&makanan.Z=='.')
				{
					score++; 
					makanan.food[i][j] = ' ';
					if (status == false)
						counter++;
				}
			}
		}
		gotoxy(35, 10); cout << "Score: " << score;
		if (status == false) {
			gotoxy(35, 8); cout << "Phase Counter: " << 20 - counter << " Left";
		}
		if (counter == 20)
		{
			status = true;
			counter = 0;
			temp = seconds;
		}
		gotoxy(35, 9);
		if (status==true)
		{
			player.Z = 'O';
			cout << "Phase: ON"<<"[";
			cout << seconds-temp<< "]";
			if (seconds-temp==10)
			{
				status = false;
				system("cls");
				area.draw();
				player.Z = 'o';
			}
		}
		else
			cout << "Phase: OFF";
		if (status == false) {
			for (int i = 0; i < 5; i++) {
				if (inp == 'D' || inp == 'd') {
					if (player.X == musuh.enemy[i].X && player.Y == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
					else if (player.X == musuh.enemy[i].X && player.Y - 1 == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
				}
				if (inp == 'A' || inp == 'a')
				{
					if (player.X == musuh.enemy[i].X && player.Y == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
					else if (player.X + 1 == musuh.enemy[i].X && player.Y == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
				}

				if (inp == 'S' || inp == 's')
				{
					if (player.X == musuh.enemy[i].X && player.Y == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
					else if (player.X == musuh.enemy[i].X && player.Y + 1 == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
				}
				if (inp == 'W' || inp == 'w')
				{
					if (player.X == musuh.enemy[i].X && player.Y == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
					else if (player.X - 1 == musuh.enemy[i].X && player.Y == musuh.enemy[i].Y)
					{
						system("cls");
						cout << "You Died..." << endl;
						inp = 27;
						system("pause");
					}
				}
			}
		}
		if (score == 98)
		{
			system("cls");
			cout << "YOU WIN!!" << endl;
			inp = 27;
			system("pause");
		}
	}
	void draw()
	{
		player.draw();
		musuh.draw();
		makanan.draw();
	}
	void run()
	{
		installAll();
		musuh.SetDirection();
		while (inp != 27)
		{
			draw();
			updates();
			move();
			Sleep(50);
		}
	}
};
void main()
{
	CGame g;
	g.run();
	system("cls");
}
