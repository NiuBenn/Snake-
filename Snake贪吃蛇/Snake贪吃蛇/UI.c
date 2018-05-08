#define _CRT_SECURE_NO_WARNINGS 1
#include<stdlib.h>
#include<Windows.h>
#include<string.h>
#include<stdio.h>
#include"Snake.h"
#include"UI.h"


struct UI * UIInit(int Width, int Height)//UI��ʼ��
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
	pUI->FoodBlock = "��";
	pUI->SnakeBlock = "��";	
	pUI->WallBlock = "��";
	pUI->BlockWidth = strlen(pUI->WallBlock);

	char WindowInit[50];
	sprintf(WindowInit, "mode con cols=%d lines=%d", pUI->WindowWidth, pUI->WindowHeight);
	system(WindowInit);
	return pUI;
}

void UIShowWelcome(const struct UI *pUI)//��ʾ��Ϸ��ӭ����
{
	int i;
	const char *Messages[3] = {
		"��ӭ����̰����С��Ϸ",
		"�á�.��.��.���ֱ�����ߵ��ƶ��� F1Ϊ���٣�F2Ϊ���١�",
		"���ٽ��ܵõ����ߵķ�����"
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

void UIShowGameWindow(const struct UI *pUI, int score, int scorePerFood)// ��ʾ��Ϸ���壬����ǽ���ұߵ���Ϣ���������ߺ�ʳ�����ʾ
{
	ShowWall(pUI);
	UIShowScore(pUI, score, scorePerFood);
	ShowDesc(pUI);

	ResetCursorPosition(pUI);
}

void UIShowFoodAtXY(const struct UI *pUI, int x, int y)// ��x��y����ʾʳ�x��y�����ַ�����
{
	CoordinatePosAtXY(pUI, x, y);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);//������ɫ
	printf(pUI->FoodBlock);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);//������ɫ
	ResetCursorPosition(pUI);
}

void UIShowSnakeBlockAtXY(const struct UI *pUI, int x, int y)// ��x��y����ʾ�ߵ�һ����㣬x��y�����ַ�����
{
	CoordinatePosAtXY(pUI, x, y);
	printf(pUI->SnakeBlock);

	ResetCursorPosition(pUI);
}

void UICleanBlockAtXY(const struct UI *pUI, int x, int y)// ���x��y������ʾ�飬x��y�����ַ�����
{
	CoordinatePosAtXY(pUI, x, y);
	int i;

	for (i = 0; i < pUI->BlockWidth; i++) {
		printf(" ");
	}

	ResetCursorPosition(pUI);
}




void UIShowScore(const struct UI *pUI, int score, int scorePerFood)// ��ʾ������Ϣ
{
	int blockWidth = strlen(pUI->WallBlock);
	int left = pUI->MarginLeft + (pUI->GameWidth + 2) * blockWidth + 2;
	SetPos(left, pUI->MarginTop + 8);
	printf("�÷�: %3d��ÿ��ʳ��÷�: %3d", score, scorePerFood);

	ResetCursorPosition(pUI);
}

void UIShowMessage(const struct UI *pUI, const char *Message)// ����Ϸ���������ʾ��Ϸ�˳���Ϣ
{
	// ������� + (1(��߽������+��Ϸ����ĸ���/2) * ÿ���ַ��Ŀ��
	// - �ַ����Ŀ�� / 2
	SetPos(
		pUI->MarginLeft + (
		(1 + pUI->GameWidth / 2) * pUI->BlockWidth)
		- strlen(Message) / 2,
		pUI->MarginTop + 1 + pUI->GameHeight / 2);
	printf("%s\n", Message);

	ResetCursorPosition(pUI);
}

void UIDestory(struct UI *pUI)// ���� UI ��Դ
{
	free(pUI);
}

static void ShowWall(const struct UI *pUI)
{
	int i;
	//������ǽ
	SetPos(pUI->MarginLeft, pUI->MarginTop);
	for (i = 0; i <= pUI->GameWidth; i++)
	{
		printf("%s", pUI->WallBlock);
	}
	//������ǽ
	SetPos(pUI->MarginLeft, pUI->MarginTop+pUI->GameHeight);
	for (i = 0; i <= pUI->GameWidth; i++)
	{
		printf("%s", pUI->WallBlock);
	}
	//�����ǽ
	for (i = 0; i <= pUI->GameHeight; i++)
	{
		SetPos(pUI->MarginLeft, pUI->MarginTop+i);
		printf("%s", pUI->WallBlock);
	}
	//���ұ�ǽ
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
		"���ܴ�ǽ������ҧ���Լ���",
		"�� �� �� �� �� �ֱ�����ߵ��ƶ���",
		"F1 Ϊ���٣�F2 Ϊ���١�",
		"ESC �˳���Ϸ�� SPACE ��ͣ��Ϸ��",
		"��Ȩ @�����2012"
	};
	int i;

	for (i = 0; i < sizeof(Messages) / sizeof(char *); i++) {
		SetPos(left, pUI->MarginTop + 10 + i);
		printf("%s\n", Messages[i]);
	}
}

static void SetPos(int x, int y)// �ƶ���굽x��y����ע�⣬���������������Ļ�ģ���������Ϸ�����
{
	COORD point = { x, y };
	HANDLE HOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(HOutput, point);
}

static void CoordinatePosAtXY(const struct UI *pUI, int x, int y)// �ƶ���굽x��y����ע�⣬���������������Ļ�ģ���������Ϸ�����
{
	SetPos(pUI->MarginLeft + (1 + x) * pUI->BlockWidth,
		pUI->MarginTop + 1 + y);
}


static void ResetCursorPosition(const struct UI *pUI)
{
	SetPos(0, pUI->WindowHeight - 1);
}
