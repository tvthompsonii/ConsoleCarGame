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

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
bool enemyFlag[3];
char car[4][4] = { ' ', '[', ']', ' ',
                   '@', '[', ']', '@',
                   ' ', '[', ']', ' ',
                   '@', '[', ']', '@' };

int carPos = WIN_WIDTH / 2;
int score = 0;

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
            gotoxy(WIN_WIDTH - j, i); cout << "&";
        }
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        gotoxy(SCREEN_WIDTH, i); cout << "=";
    }
}

// Enemy Cars
void genEnemy(int ind)
{
    enemyX[ind] = 17 + rand() % (33);
}

void drawEnemy(int ind)
{
    if (enemyFlag[ind])
    {
        gotoxy(enemyX[ind], enemyY[ind]);     cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " ** ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ** ";
    }
}

void eraseEnemy(int ind)
{
    if (enemyFlag[ind])
    {
        gotoxy(enemyX[ind], enemyY[ind]);     cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
    }
}

void resetEnemy(int ind)
{
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

// Player Car
void drawCar()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(j + carPos, i + 22); cout << car[i][j];
        }
    }
}

void eraseCar()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            gotoxy(j + carPos, i + 22); cout << " ";
        }
    }
}

int collision()
{
    for (int ind = 0; ind < 2; ind++)
    {
        if (enemyFlag[ind])
        {
            if (enemyY[ind] + 4 >= 23)
            {
                if (enemyX[ind] + 3 >= carPos and enemyX[ind] <= carPos + 3)
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
}

// play the game
void playinitialize()
{
    carPos = -1 + WIN_WIDTH / 2;
    score = 0;
    enemyFlag[0] = true;
    enemyFlag[1] = false;
    enemyY[0] = enemyY[1] = 1;

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

    gotoxy(18, 5); cout << "Press any key to start...";
    _getch();
    gotoxy(18, 5); cout << "                         ";
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
                if (carPos > 18)
                    carPos -= 4;
            }
            if (ch == 'd' or ch == 'D' or ch == 77)
            {
                if (carPos < 50)
                    carPos += 4;
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
        Sleep(50);
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        // Spawn second enemy
        if (enemyY[0] == 12)
        {
            if (!enemyFlag[1])
                enemyFlag[1] = true;
        }
        // Move the enemies
        if (enemyFlag[0])
            enemyY[0] += 1;
        if (enemyFlag[1])
            enemyY[1] += 1;
        // Reset emy to top of screen
        if (enemyY[0] > SCREEN_HEIGHT - 4)
        {
            resetEnemy(0);
            score++;
            updateScore();
        }
        if (enemyY[1] > SCREEN_HEIGHT - 4)
        {
            resetEnemy(1);
            score++;
            updateScore();
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
