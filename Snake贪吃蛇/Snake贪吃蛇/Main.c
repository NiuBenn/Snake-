#define _CRT_SECURE_NO_WARNINGS 1
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "UI.h"

int main()
{
	
	srand((unsigned int)time(NULL));// ���ø��ݵ�ǰʱ��������������
	struct Game *pGame = CreateGame();
	StartGame(pGame);
	DestroyGame(pGame);

	system("pause");
	return 0;
}