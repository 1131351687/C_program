#include "game.h"

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>



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
void init(Game *game)
{
    srand((unsigned int)time(NULL));
    
    game->snakeX[0] = 10;
    game->snakeY[0] = 5;

    game->snakeX[1] = 9;
    game->snakeY[1] = 5;

    game->snakeX[2] = 8;
    game->snakeY[2] = 5;

    game->snakeLength = 3;
    game->direction = RIGHT;
    game->foodX = WIDTH / 2 + 5;
    game->foodY = HEIGHT / 2 - 5;

    game->running = 1;
}

void draw(Game *game)
{
    gotoxy(0, 0);
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int inSnake = 0;
            for (int i = 0; i < game->snakeLength; i++)
            {
                if (game->snakeX[i] == x && game->snakeY[i] == y)
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
            else if (x == game->foodX && y == game->foodY)
            {
                screenBuffer[y][x] = 'F';
            }
            else
            {
                screenBuffer[y][x] = ' ';
            }
            
        }
        screenBuffer[y][WIDTH] = '\0';
    }
    for (int y = 0; y < HEIGHT; y++)
    {
        printf("%s\n", screenBuffer[y]);
    }
    fflush(stdout);
}

void input(Game *game)
{
    if (_kbhit())
    {
        char ch = _getch();
        int new_direction = game->direction;
        if (ch == 'w')
        {
            new_direction = UP;
        }
        else if (ch == 's')
        {
            new_direction = DOWN;
        }
        else if (ch == 'a')
        {
            new_direction = LEFT;
        }
        else if (ch == 'd')
        {
            new_direction = RIGHT;
        }
        if ((game->direction == UP && new_direction != DOWN) ||
            (game->direction == DOWN && new_direction != UP) ||
            (game->direction == LEFT && new_direction != RIGHT) ||
            (game->direction == RIGHT && new_direction != LEFT))
        {
            game->direction = new_direction;
        }
    }
}

bool isOnSnake(Game *game, int x, int y)
{
    for (int i = 0; i < game->snakeLength; i++)
    {
        if (game->snakeX[i] == x && game->snakeY[i] == y)
        {
            return true;
        }
    }
    return false;
}
void update(Game *game)
{
    for (int i = game->snakeLength - 1; i > 0; i--)
    {
        game->snakeX[i] = game->snakeX[i - 1];
        game->snakeY[i] = game->snakeY[i - 1];
    }
    if (game->direction == UP)
    {
        game->snakeY[0]--;
    }
    else if (game->direction == DOWN)
    {
        game->snakeY[0]++;
    }
    else if (game->direction == LEFT)
    {
        game->snakeX[0]--;
    }
    else if (game->direction == RIGHT)
    {
        game->snakeX[0]++;
    }

    if (game->snakeX[0] == game->foodX && game->snakeY[0] == game->foodY)
    {
        game->snakeLength++;
        do
        {
            game->foodX = rand() % (WIDTH - 2) + 1;
            game->foodY = rand() % (HEIGHT - 2) + 1;
        } while (isOnSnake(game, game->foodX, game->foodY) && (game->snakeLength < (WIDTH - 2) * (HEIGHT - 2)));
    }
}

int check(Game *game)
{

    for (int i = 1; i < game->snakeLength; i++)
    {
        bool flag = false;
        if (game->snakeX[0] == game->snakeX[i] && game->snakeY[0] == game->snakeY[i])
        {
            flag = true;
        }
        if (flag)
        {
            game->running = 0;
        }
    }
    if (game->snakeX[0] <= 0 || game->snakeX[0] >= WIDTH - 1 ||
        game->snakeY[0] <= 0 || game->snakeY[0] >= HEIGHT - 1)
    {
        game->running = 0;
    }

    return game->running;

}