#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>


#include "game.h"




int main()
{
    init();
    while (check() == 1)
    {

        input();
        update();
        draw();
        Sleep(50);
    }

    return 0;
}
