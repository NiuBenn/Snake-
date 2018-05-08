#define _CRT_SECURE_NO_WARNINGS 1
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "UI.h"

int main()
{
	
	srand((unsigned int)time(NULL));// 设置根据当前时间戳设置随机种子
	struct Game *pGame = CreateGame();
	StartGame(pGame);
	DestroyGame(pGame);

	system("pause");
	return 0;
}