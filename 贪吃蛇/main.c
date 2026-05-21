#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>


#include "game.h"


int main()
{
    Game game;
    init(&game);  // 初始化游戏状态

    while (game.running)
    {

        input(&game);
        update(&game);
        check(&game);
        draw(&game);
        Sleep(50);
    }
    printf("Game Over!\n");

    return 0;
}
