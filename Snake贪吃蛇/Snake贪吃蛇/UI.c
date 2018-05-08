#define _CRT_SECURE_NO_WARNINGS 1
#include<stdlib.h>
#include<Windows.h>
#include<string.h>
#include<stdio.h>
#include"Snake.h"
#include"UI.h"


struct UI * UIInit(int Width, int Height)//UI初始化
{
	const int Left = 2;
	const int Top = 2;
	const int BlockWidth = 2;
	const int DescWidth = 40;

	struct UI *pUI = (struct UI *)malloc(sizeof(struct UI));
	pUI->GameWidth = Width;
	pUI->GameHeight = Height;
	pUI->MarginLeft = Left;
	pUI->MarginTop = Top;
	pUI->WindowWidth = Left + (Width + 2) * BlockWidth + DescWidth;
	pUI->WindowHeight = Top + Height + 2 + 3;
	pUI->FoodBlock = "█";
	pUI->SnakeBlock = "█";	
	pUI->WallBlock = "█";
	pUI->BlockWidth = strlen(pUI->WallBlock);

	char WindowInit[50];
	sprintf(WindowInit, "mode con cols=%d lines=%d", pUI->WindowWidth, pUI->WindowHeight);
	system(WindowInit);
	return pUI;
}

void UIShowWelcome(const struct UI *pUI)//显示游戏欢迎界面
{
	int i;
	const char *Messages[3] = {
		"欢迎来到贪吃蛇小游戏",
		"用↑.↓.←.→分别控制蛇的移动， F1为加速，F2为减速。",
		"加速将能得到更高的分数。"
	};

	i = 0;
	SetPos(pUI->WindowWidth / 2 - strlen(Messages[i]) / 2, pUI->WindowHeight / 2);
	printf("%s\n", Messages[i]);
	SetPos(pUI->WindowWidth / 2 - strlen(Messages[i]) / 2, pUI->WindowHeight / 2 + 2);
	system("pause");
	system("cls");

	i = 1;
	SetPos(pUI->WindowWidth / 2 - strlen(Messages[i]) / 2, pUI->WindowHeight / 2);
	printf("%s\n", Messages[i]);

	i = 2;
	SetPos(pUI->WindowWidth / 2 - strlen(Messages[i]) / 2, pUI->WindowHeight / 2 + 2);
	printf("%s\n", Messages[i]);
	SetPos(pUI->WindowWidth / 2 - strlen(Messages[i]) / 2, pUI->WindowHeight / 2 + 4);
	system("pause");
	system("cls");
}

void UIShowGameWindow(const struct UI *pUI, int score, int scorePerFood)// 显示游戏整体，包括墙、右边的信息，不包括蛇和食物的显示
{
	ShowWall(pUI);
	UIShowScore(pUI, score, scorePerFood);
	ShowDesc(pUI);

	ResetCursorPosition(pUI);
}

void UIShowFoodAtXY(const struct UI *pUI, int x, int y)// 在x，y处显示食物，x，y都是字符个数
{
	CoordinatePosAtXY(pUI, x, y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);//设置颜色
	printf(pUI->FoodBlock);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//设置颜色
	ResetCursorPosition(pUI);
}

void UIShowSnakeBlockAtXY(const struct UI *pUI, int x, int y)// 在x，y处显示蛇的一个结点，x，y都是字符个数
{
	CoordinatePosAtXY(pUI, x, y);
	printf(pUI->SnakeBlock);

	ResetCursorPosition(pUI);
}

void UICleanBlockAtXY(const struct UI *pUI, int x, int y)// 清空x，y处的显示块，x，y都是字符个数
{
	CoordinatePosAtXY(pUI, x, y);
	int i;

	for (i = 0; i < pUI->BlockWidth; i++) {
		printf(" ");
	}

	ResetCursorPosition(pUI);
}




void UIShowScore(const struct UI *pUI, int score, int scorePerFood)// 显示分数信息
{
	int blockWidth = strlen(pUI->WallBlock);
	int left = pUI->MarginLeft + (pUI->GameWidth + 2) * blockWidth + 2;
	SetPos(left, pUI->MarginTop + 8);
	printf("得分: %3d，每个食物得分: %3d", score, scorePerFood);

	ResetCursorPosition(pUI);
}

void UIShowMessage(const struct UI *pUI, const char *Message)// 在游戏区域居中显示游戏退出消息
{
	// 左填充宽度 + (1(左边界个数）+游戏区域的个数/2) * 每个字符的宽度
	// - 字符串的宽度 / 2
	SetPos(
		pUI->MarginLeft + (
		(1 + pUI->GameWidth / 2) * pUI->BlockWidth)
		- strlen(Message) / 2,
		pUI->MarginTop + 1 + pUI->GameHeight / 2);
	printf("%s\n", Message);

	ResetCursorPosition(pUI);
}

void UIDestory(struct UI *pUI)// 销毁 UI 资源
{
	free(pUI);
}

static void ShowWall(const struct UI *pUI)
{
	int i;
	//画上面墙
	SetPos(pUI->MarginLeft, pUI->MarginTop);
	for (i = 0; i <= pUI->GameWidth; i++)
	{
		printf("%s", pUI->WallBlock);
	}
	//画下面墙
	SetPos(pUI->MarginLeft, pUI->MarginTop+pUI->GameHeight);
	for (i = 0; i <= pUI->GameWidth; i++)
	{
		printf("%s", pUI->WallBlock);
	}
	//画左边墙
	for (i = 0; i <= pUI->GameHeight; i++)
	{
		SetPos(pUI->MarginLeft, pUI->MarginTop+i);
		printf("%s", pUI->WallBlock);
	}
	//画右边墙
	for (i = 0; i <= pUI->GameHeight; i++)
	{
		SetPos(pUI->MarginLeft+(pUI->GameWidth)*2, pUI->MarginTop + i);
		printf("%s", pUI->WallBlock);
	}
}

static void ShowDesc(const struct UI *pUI)
{
	int left = pUI->MarginLeft + (pUI->GameWidth + 2) * pUI->BlockWidth + 2;
	const char *Messages[] = {
		"不能穿墙，不能咬到自己。",
		"用 ↑ ↓ ← → 分别控制蛇的移动。",
		"F1 为加速，F2 为减速。",
		"ESC 退出游戏。 SPACE 暂停游戏。",
		"版权 @金翅鸟2012"
	};
	int i;

	for (i = 0; i < sizeof(Messages) / sizeof(char *); i++) {
		SetPos(left, pUI->MarginTop + 10 + i);
		printf("%s\n", Messages[i]);
	}
}

static void SetPos(int x, int y)// 移动光标到x，y处，注意，这里是相对整个屏幕的，而不是游戏区域的
{
	COORD point = { x, y };
	HANDLE HOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(HOutput, point);
}

static void CoordinatePosAtXY(const struct UI *pUI, int x, int y)// 移动光标到x，y处，注意，这里是相对整个屏幕的，而不是游戏区域的
{
	SetPos(pUI->MarginLeft + (1 + x) * pUI->BlockWidth,
		pUI->MarginTop + 1 + y);
}


static void ResetCursorPosition(const struct UI *pUI)
{
	SetPos(0, pUI->WindowHeight - 1);
}
