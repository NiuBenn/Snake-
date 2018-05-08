#ifndef __UI_H__
#define __UI_H__
#include"Snake.h"
struct UI {
	
	int MarginTop;//上边界边缘空白宽度(字符宽度)
	int MarginLeft;//左边界边缘空白宽度(字符宽度)

	
	int GameWidth;//游戏区域宽度(字符个数)
	int GameHeight;//游戏区域高度(字符个数)

	
	int WindowWidth;//窗口宽度(字符)
	int WindowHeight;//窗口高度(字符)

	char *SnakeBlock;	// 蛇的显示块
	char *WallBlock;	// 墙的显示块
	char *FoodBlock;	// 食物的显示块
	int BlockWidth;		// 每个块的宽度，注意，上面几个块的宽度要相等，否则就对不齐了
};


struct UI * UIInit(int Width, int Height);//UI初始化

void UIShowWelcome(const struct UI *pUI);//显示游戏欢迎界面

void UIShowGameWindow(const struct UI *pUI, int Score, int ScorePerFood);// 显示游戏整体，包括墙、右边的信息，不包括蛇和食物的显示


void UIShowFoodAtXY(const struct UI *pUI, int x, int y);// 在x，y处显示食物，x，y都是字符个数


void UIShowSnakeBlockAtXY(const struct UI *pUI, int x, int y);// 在x，y处显示蛇的一个结点，x，y都是字符个数


void UICleanBlockAtXY(const struct UI *pUI, int x, int y);// 清空x，y处的显示块，x，y都是字符个数


void UIShowScore(const struct UI *pUI, int score, int scorePerFood);// 显示分数信息


void UIShowMessage(const struct UI *pUI, const char *message);// 在游戏区域居中显示游戏退出消息


void UIDestory(struct UI *pUI);// 销毁 UI 资源

static void SetPos(int x, int y);// 移动光标到x，y处，注意，这里是相对整个屏幕的，而不是游戏区域的



static void ShowWall(const struct UI *pUI);// 显示墙


static void ShowDesc(const struct UI *pUI);// 显示右侧信息


static void CoordinatePosAtXY(const struct UI *pUI, int x, int y);// 将游戏盘的x，y坐标转换为相对整个屏幕的x，y，也就是把字符个数转成最后的字符宽度



static void ResetCursorPosition(const struct UI *pUI);// 重置光标到屏幕下方，主要是为了显示的美观


#endif