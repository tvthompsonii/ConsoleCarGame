// ConsoleCarGame.cpp 
//
// This is a simple console "car" game where you try to race through slower
// traffic. All of this is based on a YouTube video by Angulars. I implemented
// if strictly to play around with Visual Studio.

#include <iostream>
#include <conio.h>
#include <dos.h>
#include <Windows.h>
#include <time.h>

// Screen defines the whole area
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
// Win describes only the playable area
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

struct Enemy
{
    int lane;
    int y;
    bool flag;
};
Enemy enemy[2];

char car[4][4] = { ' ', '[', ']', ' ',
                   '@', '[', ']', '@',
                   ' ', '[', ']', ' ',
                   '@', '[', ']', '@' };

int maxLane = 8;
int laneX[9] = { 17, 21, 25, 29, 33, 37, 41, 45, 49 };

int carLane = 5;
int score = 0;
int speed = 50;

void gotoxy(int x, int y)
{
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setCursor(bool visible, DWORD size)
{
    if (size == 0)
        size = 20;

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}


void drawBorder()
{
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            gotoxy(0 + j, i); cout << "&";
            gotoxy(WIN_WIDTH - j - 1, i); cout << "&";
        }
        gotoxy(SCREEN_WIDTH, i); cout << "&&";
    }
}

// Enemy Cars
void genEnemy(int ind)
{
    enemy[ind].lane = rand() % 9;
}

void drawEnemy(int ind)
{
    if (enemy[ind].flag)
    {
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y);     cout << "****";
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y + 1); cout << " ** ";
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y + 2); cout << "****";
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y + 3); cout << " ** ";
    }
}

void eraseEnemy(int ind)
{
    if (enemy[ind].flag)
    {
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y);     cout << "    ";
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y + 1); cout << "    ";
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y + 2); cout << "    ";
        gotoxy(laneX[enemy[ind].lane], enemy[ind].y + 3); cout << "    ";
    }
}

void resetEnemy(int ind)
{
    eraseEnemy(ind);
    enemy[ind].y = 1;
    genEnemy(ind);
}

// Player Car
void drawCar()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(j + laneX[carLane], i + 22); cout << car[i][j];
        }
    }
}

void eraseCar()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(j + laneX[carLane], i + 22); cout << " ";
        }
    }
}

int collision()
{
    for (int ind = 0; ind < 2; ind++)
    {
        if (enemy[ind].flag)
        {
            if (enemy[ind].y + 4 > 22)
            {
                if (enemy[ind].lane == carLane)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void gameover()
{
    int x = laneX[carLane] - 1;
    gotoxy(x, 21); cout << "      ";
    gotoxy(x, 22); cout << "@[ ]@ ";
    gotoxy(x, 23); cout << " [  ] ";
    gotoxy(x, 24); cout << " [ ] @";
    gotoxy(x, 25); cout << "  @[] ";
    gotoxy(x, 26); cout << "      ";
    Sleep(1000);

    system("cls");
    cout << endl;
    cout << "\t\t---------------------------" << endl;
    cout << "\t\t-------  Game Over  -------" << endl;
    cout << "\t\t-------             -------" << endl;
    cout << "\t\t------- Final Score -------" << endl;
    cout << "\t\t------- ";
    cout.width(11);
    cout << score << " -------" << endl;
    cout << "\t\t---------------------------" << endl;
    cout << "\t\tPress any key to go back to menu.";
    _getch();
}

void updateScore()
{
    gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
    if (score % 10 == 0)
    {
        speed -= 3;
        if (speed < 5)
            speed = 5;
    }
}

// play the game
void playinitialize()
{
    carLane = 5;
    score = 0;
    speed = 50;
    enemy[0].flag = true; 
    enemy[0].y = 1;
    enemy[1].flag = false; 
    enemy[1].y = 1;

    system("cls");
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    gotoxy(WIN_WIDTH + 6, 2);  cout << " Car Game";
    gotoxy(WIN_WIDTH + 6, 4);  cout << "-----------";
    gotoxy(WIN_WIDTH + 6, 6);  cout << "-----------";
    gotoxy(WIN_WIDTH + 4, 12); cout << "   Control";
    gotoxy(WIN_WIDTH + 4, 14); cout << "A Key - Left";
    gotoxy(WIN_WIDTH + 4, 15); cout << "D Key - Right";

    gotoxy(21, 5); cout << "Press any key to start...";
    _getch();
    gotoxy(21, 5); cout << "                         ";
}

void play()
{
    playinitialize();
    while (1)
    {
        if (_kbhit())
        {
            char ch = _getch();
            if (ch == 'a' or ch == 'A' or ch == 75)
            {
                if (carLane > 0)
                    carLane -= 1;
            }
            if (ch == 'd' or ch == 'D' or ch == 77)
            {
                if (carLane < maxLane)
                    carLane += 1;
            }
            if (ch == 27)
            {
                break;
            }
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);
        if (collision())
        {
            gameover();
            return;
        }
        Sleep(speed);
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        // Spawn second enemy
        if (enemy[0].y == 12)
        {
            if (!enemy[1].flag)
                enemy[1].flag = true;
        }
        for (int i=0; i<2; i++)
        {
            // Move the enemy
            if (enemy[i].flag)
                enemy[i].y += 1;
            // Reset enemy to top of screen
            if (enemy[i].y > SCREEN_HEIGHT - 4)
            {
                resetEnemy(i);
                score++;
                updateScore();
            }
        }
    }
}

void instructions()
{
    system("cls");
    cout << "Instructions" << endl;
    cout << "------------" << endl;
    cout << " Avoid Cars by moving left or right." << endl;
    cout << " Press 'a' to move left" << endl;
    cout << " Press 'd' to move right" << endl;
    cout << " Press 'escape' to exit" << endl << endl;
    cout << "Press any key to go back to menu..." << endl;
    _getch();
}

char menu()
{
    system("cls");
    gotoxy(10, 5); cout << "+------------------------+";
    gotoxy(10, 6); cout << "|        Car Game        |";
    gotoxy(10, 7); cout << "+------------------------+";
    gotoxy(10, 9); cout << "1. Start Game";
    gotoxy(10, 10); cout << "2. Instructions";
    gotoxy(10, 11); cout << "3. Quit";
    gotoxy(10, 13); cout << "Select option: ";

    return _getche();
}

int main()
{
    setCursor(0, 0);
    srand((unsigned)time(NULL));

    do {
        char op = menu();
        if (op == '1') play();
        else if (op == '2') instructions();
        else if (op == '3') exit(0);
    } while (1);

    return 0;
}
