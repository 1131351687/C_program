#include "game.h"

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#define WIDTH 40
#define HEIGHT 20

int snakeX[100];
int snakeY[100];
int snakeLength = 3;
int direction = 4; // 1: up, 2: down, 3: left, 4: right
int foodX, foodY;
char screenBuffer[HEIGHT][WIDTH + 1]; // 用于存储屏幕内容的缓冲区


void gotoxy(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        pos
    );
}
void init()
{
    snakeX[0] = 10;
    snakeY[0] = 5;

    snakeX[1] = 9;
    snakeY[1] = 5;

    snakeX[2] = 8;
    snakeY[2] = 5;

    foodX = WIDTH / 2 + 5;
    foodY = HEIGHT / 2 - 5;
}

void draw()
{
    gotoxy(0, 0);
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int inSnake = 0;
            for (int i = 0; i < snakeLength; i++)
            {
                if (snakeX[i] == x && snakeY[i] == y)
                {
                    inSnake = 1;
                    break;
                }
            }
            if (
                x == 0 || x == WIDTH - 1 ||
                y == 0 || y == HEIGHT - 1)
            {
                screenBuffer[y][x] = '*';
            }
            else if (inSnake)
            {
                screenBuffer[y][x] = 'O';
            }
            else if (x == foodX && y == foodY)
            {
                screenBuffer[y][x] = 'F';
            }
            else
            {
                screenBuffer[y][x] = ' ';
            }
            screenBuffer[y][WIDTH] = '\0';
        }
    }
    for (int y = 0; y < HEIGHT; y++)
    {
        printf("%s\n", screenBuffer[y]);
    }
    fflush(stdout);
}

void input()
{
    if (_kbhit())
    {
        char ch = _getch();
        int new_direction = direction;
        if (ch == 'w')
        {
            new_direction = 1;
        }
        else if (ch == 's')
        {
            new_direction = 2;
        }
        else if (ch == 'a')
        {
            new_direction = 3;
        }
        else if (ch == 'd')
        {
            new_direction = 4;
        }
        if ((direction == 1 && new_direction != 2) ||
            (direction == 2 && new_direction != 1) ||
            (direction == 3 && new_direction != 4) ||
            (direction == 4 && new_direction != 3))
        {
            direction = new_direction;
        }
    }
}

bool isOnSnake(int x, int y)
{
    for (int i = 0; i < snakeLength; i++)
    {
        if (snakeX[i] == x && snakeY[i] == y)
        {
            return true;
        }
    }
    return false;
}
void update()
{
    for (int i = snakeLength - 1; i > 0; i--)
    {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    if (direction == 1)
    {
        snakeY[0]--;
    }
    else if (direction == 2)
    {
        snakeY[0]++;
    }
    else if (direction == 3)
    {
        snakeX[0]--;
    }
    else if (direction == 4)
    {
        snakeX[0]++;
    }

    if (snakeX[0] == foodX && snakeY[0] == foodY)
    {
        snakeLength++;
        do
        {
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;
        } while (isOnSnake(foodX, foodY) && (snakeLength < (WIDTH - 2) * (HEIGHT - 2)));
    }
}

int check()
{

    for (int i = 1; i < snakeLength; i++)
    {
        bool flag = false;
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        {
            flag = true;
        }
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
        {
            flag = true;
        }
        if (flag)
        {
            printf("Game Over!\n");
            return 0;
        }
    }
    if (snakeX[0] <= 0 || snakeX[0] >= WIDTH - 1 ||
        snakeY[0] <= 0 || snakeY[0] >= HEIGHT - 1)
    {
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}