#define _CRT_SECURE_NO_WARNINGS 1
#include "Game.h"
#include "UI.h"
#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>


enum ExitStatus
{
	QUIT,
	KILLED_BY_WALL,
	KILLED_BY_SELF
};

static void SetPos(int x, int y)// �ƶ���굽x��y����ע�⣬���������������Ļ�ģ���������Ϸ�����
{
	COORD point = { x, y };
	HANDLE HOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(HOutput, point);
}

static void CoordinatePosAtXY(const struct UI *pUI, int x, int y)
{
	SetPos(pUI->MarginLeft + (1 + x) * pUI->BlockWidth,
		pUI->MarginTop + 1 + y);
}

static void ResetCursorPosition(const struct UI *pUI)
{
	SetPos(0, pUI->WindowHeight - 1);
}

static void InitializeSnake(struct Snake *pSnake);// ��ʼ���߽ṹ��

static bool IsOverlapSnake(int x, int y, const struct Snake *pSnake);// �ж�ʳ�������Ƿ�������ص�

static void GenerateFood(struct Game *pGame);// ����һ��ʳ��

static struct Point GetNextPosition(const struct Snake *pSnake);// ��ȡ�߼������������

static bool IsWillEatFood(struct Point NextPoint, struct Point FoodPoint);// �ж����Ƿ񽫳Ե�ʳ��

static void GrowSnakeAndDisplay(struct Snake *pSnake, struct Point FoodPoint, const struct UI *pUI);// �����߲��ҽ�����ʾ

static void AddHead(struct Snake *pSnake, struct Point NextPoint, const struct UI *pUI);// ������ͷ

static void RemoveTail(struct Snake *pSnake, const struct UI *pUI);// ɾ����β

static void MoveSnakeAndDisplay(struct Snake *pSnake, struct Point NextPoint, const struct UI *pUI);// �ƶ��߲��ҽ�����ʾ

static bool IsKilledByWall(const struct Node *pHead, int width, int height);// ���Ƿ�ײǽ��

static bool IsKilledBySelf(const struct Node *pHead, const struct Snake *pSnake);// ���Ƿ�ײ�Լ���

static bool IsSnakeAlive(const struct Game *pGame, enum ExitStatus *exitStatus);// ���Ƿ���

static void HandleDirective(struct Game *pGame);// ������ָ��

static void DisplaySnake(const struct UI *pUI, const struct Snake *pSnake);// ��ʾ��������

static void _Pause();// ��ͣ


void CleanBodyColor(const struct UI *pUI, struct Game *pGame)//�����������ɫ
{
	UICleanBlockAtXY(pUI, pGame->FoodPoint.x, pGame->FoodPoint.y);
	CoordinatePosAtXY(pUI, pGame->FoodPoint.x, pGame->FoodPoint.y);
	printf(pUI->SnakeBlock);
	ResetCursorPosition(pUI);
}


static void InitializeSnake(struct Snake *pSnake)//��ʼ���ߵĽṹ��
{
	const int length = 3;
	const int x = 5;
	const int y = 5;
	int i;
	struct Node *pNode;

	pSnake->direction = RIGHT;
	pSnake->pBody = NULL;

	for (i = 0; i < length; i++) 
	{
		pNode = (struct Node *)malloc(sizeof(struct Node));
		pNode->point.x = x + i;
		pNode->point.y = y;
		pNode->pNext = pSnake->pBody;
		pSnake->pBody = pNode;
	}
}

static bool IsOverlapSnake(int x, int y, const struct Snake *pSnake)// �ж�ʳ�������Ƿ�������ص�
{
	struct Node *pNode;

	for (pNode = pSnake->pBody; pNode != NULL; pNode = pNode->pNext)
	{
		if (pNode->point.x == x && pNode->point.y == y) 
		{
			return true;
		}
	}

	return false;
}

static void GenerateFood(struct Game *pGame)// ����һ��ʳ��
{
	int x, y;

	do 
	{
		x = rand() % (pGame->Width - 4);
		y = rand() % (pGame->Height - 2);
	} while (IsOverlapSnake(x, y, &pGame->snake));

	pGame->FoodPoint.x = x;
	pGame->FoodPoint.y = y;
}

static struct Point GetNextPosition(const struct Snake *pSnake)// ��ȡ�߼������������
{
	int nextX, nextY;

	nextX = pSnake->pBody->point.x;
	nextY = pSnake->pBody->point.y;

	switch (pSnake->direction) 
	{
	case UP:
		nextY -= 1;
		break;
	case DOWN:
		nextY += 1;
		break;
	case LEFT:
		nextX -= 1;
		break;
	case RIGHT:
		nextX += 1;
		break;
	}

	struct Point point = 
	{
		.x = nextX,
		.y = nextY
	};

	return point;
}

static bool IsWillEatFood(struct Point NextPoint, struct Point FoodPoint)// �ж����Ƿ񽫳Ե�ʳ��
{
	return NextPoint.x == FoodPoint.x && NextPoint.y == FoodPoint.y;
}

static void GrowSnakeAndDisplay(struct Snake *pSnake, struct Point FoodPoint, const struct UI *pUI)// �����߲��ҽ�����ʾ
{
	struct Node *pHead = (struct Node *)malloc(sizeof(struct Node));
	pHead->point.x = FoodPoint.x;
	pHead->point.y = FoodPoint.y;

	pHead->pNext = pSnake->pBody;
	pSnake->pBody = pHead;
	
	UIShowFoodAtXY(pUI, FoodPoint.x, FoodPoint.y);
}

static void AddHead(struct Snake *pSnake, struct Point NextPoint, const struct UI *pUI)// ������ͷ
{
	struct Node *pNode = (struct Node *)malloc(sizeof(struct Node));

	pNode->point.x = NextPoint.x;
	pNode->point.y = NextPoint.y;
	pNode->pNext = pSnake->pBody;

	pSnake->pBody = pNode;

	UIShowSnakeBlockAtXY(pUI, NextPoint.x, NextPoint.y);
}

static void RemoveTail(struct Snake *pSnake, const struct UI *pUI)// ɾ����β
{
	struct Node *pNode = pSnake->pBody;

	while (pNode->pNext->pNext != NULL) 
	{
		pNode = pNode->pNext;
	}

	UICleanBlockAtXY(pUI, pNode->pNext->point.x, pNode->pNext->point.y);

	free(pNode->pNext);
	pNode->pNext = NULL;
}

static void MoveSnakeAndDisplay(struct Snake *pSnake, struct Point NextPoint, const struct UI *pUI)// �ƶ��߲��ҽ�����ʾ
{
	RemoveTail(pSnake, pUI);
	AddHead(pSnake, NextPoint, pUI);
}

static bool IsKilledByWall(const struct Node *pHead, int width, int height)// ���Ƿ�ײǽ��
{
	if (pHead->point.x < 0 || pHead->point.x >= width - 1) 
	{
		return true;
	}

	if (pHead->point.y < 0 || pHead->point.y >= height - 1) 
	{
		return true;
	}

	return false;
}

static bool IsKilledBySelf(const struct Node *pHead, const struct Snake *pSnake)// ���Ƿ�ײ�Լ���
{
	struct Node *pNode;

	
	for (pNode = pSnake->pBody->pNext; pNode != NULL; pNode = pNode->pNext) 
	{
		if (pHead->point.x == pNode->point.x && pHead->point.y == pNode->point.y) 
		{
			return true;
		}
	}

	return false;
}

static bool IsSnakeAlive(const struct Game *pGame, enum ExitStatus *exitStatus)// ���Ƿ���
{
	struct Node *pHead = pGame->snake.pBody;

	if (IsKilledByWall(pHead, pGame->Width, pGame->Height)) 
	{
		*exitStatus = KILLED_BY_WALL;
		return false;
	}

	if (IsKilledBySelf(pHead, &pGame->snake)) 
	{
		*exitStatus = KILLED_BY_SELF;
		return false;
	}

	return true;
}

static void HandleDirective(struct Game *pGame)// ������ָ��
{
	if (GetAsyncKeyState(VK_UP) && pGame->snake.direction != DOWN) {
		pGame->snake.direction = UP;
	}
	else if (GetAsyncKeyState(VK_DOWN) && pGame->snake.direction != UP) {
		pGame->snake.direction = DOWN;
	}
	else if (GetAsyncKeyState(VK_LEFT) && pGame->snake.direction != RIGHT) {
		pGame->snake.direction = LEFT;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && pGame->snake.direction != LEFT) {
		pGame->snake.direction = RIGHT;
	}
}

static void DisplaySnake(const struct UI *pUI, const struct Snake *pSnake)// ��ʾ��������
{
	struct Node *pNode;

	if (pSnake->pBody == NULL) {
		return;
	}

	for (pNode = pSnake->pBody; pNode != NULL; pNode = pNode->pNext) 
	{
		UIShowSnakeBlockAtXY(pUI, pNode->point.x, pNode->point.y);
	}
}

struct Game * CreateGame()
{
	struct Game *pGame = (struct Game *)malloc(sizeof(struct Game));
	pGame->Width = 29;
	pGame->Height = 27;	
	pGame->Score = 0;
	pGame->ScorePerFood = 10;

	
	InitializeSnake(&pGame->snake);
	GenerateFood(pGame);

	return pGame;
}

void StartGame(struct Game *pGame)
{
	
	struct UI *pUI = UIInit(pGame->Width, pGame->Height);
	enum ExitStatus exitStatus = QUIT;
	int time = 300;

	UIShowWelcome(pUI);
	UIShowGameWindow(pUI, pGame->Score, pGame->ScorePerFood);
	DisplaySnake(pUI, &pGame->snake);
	UIShowFoodAtXY(pUI, pGame->FoodPoint.x, pGame->FoodPoint.y);


	while (1) 
	{
		if (GetAsyncKeyState(VK_ESCAPE)) 
		{
			break;
		}
		else if (GetAsyncKeyState(VK_SPACE))
		{
			_Pause();
		}
		HandleDirective(pGame);// ������ָ��
		struct Point NextPoint = GetNextPosition(&pGame->snake);

		if (IsWillEatFood(NextPoint, pGame->FoodPoint)) {
			pGame->Score += pGame->ScorePerFood;
			UIShowScore(pUI, pGame->Score, pGame->ScorePerFood);
			GrowSnakeAndDisplay(&pGame->snake, pGame->FoodPoint, pUI);
			CleanBodyColor(pUI, pGame);
			
			GenerateFood(pGame);

			UIShowFoodAtXY(pUI, pGame->FoodPoint.x, pGame->FoodPoint.y);
		}
		else {
			MoveSnakeAndDisplay(&pGame->snake, NextPoint, pUI);
		}

		if (!IsSnakeAlive(pGame, &exitStatus)) {
			break;
		}
		if (GetAsyncKeyState(VK_F1))
		{
			if (pGame->ScorePerFood < 60)
			{
				pGame->ScorePerFood = pGame->ScorePerFood + 10;
				time = time - 50;
				UIShowScore(pUI, pGame->Score, pGame->ScorePerFood);//��ʱ���µ����÷�
			}
		}
		if (GetAsyncKeyState(VK_F2))
		{
			if (pGame->ScorePerFood > 10)
			{
				pGame->ScorePerFood = pGame->ScorePerFood - 10;
				time = time + 50;
				UIShowScore(pUI, pGame->Score, pGame->ScorePerFood);//��ʱ���µ����÷�
			}
		}

		Sleep(time);
	}

	char *messages[3];
	messages[QUIT] = "��Ϸ����";
	messages[KILLED_BY_WALL] = "��Ϸ������ײ��ǽ��";
	messages[KILLED_BY_SELF] = "��Ϸ������ײ���Լ���";
	UIShowMessage(pUI, messages[exitStatus]);

	UIDestory(pUI);
}

void DestroyGame(struct Game *pGame)
{
	struct Node *pNode, *pNext;

	for (pNode = pGame->snake.pBody; pNode != NULL; pNode = pNext) 
	{
		pNext = pNode->pNext;
		free(pNode);
	}
	free(pGame);
}


static void _Pause()
{
	while (1)
	{
		Sleep(300);
		if (GetAsyncKeyState(VK_SPACE))
		{
			break;
		}
	}
}