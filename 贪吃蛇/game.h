#ifndef GAME_H
#define GAME_H

#define WIDTH 40
#define HEIGHT 20
#define MAX_SNAKE_LENGTH 100


typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct      // 游戏状态
{
    int snakeX[MAX_SNAKE_LENGTH];
    int snakeY[MAX_SNAKE_LENGTH];

    int snakeLength;

    Direction direction;

    int foodX, foodY;

    int running; // 1: running, 0: game over
}Game;

void draw(Game *game);
void input(Game *game);
void update(Game *game);
int check(Game *game);
void init(Game *game);
void gotoxy(int x, int y);


#endif 