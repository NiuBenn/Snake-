#ifndef __SNAKE_H__
#define __SNAKE_H__


struct Point {
	int x;
	int y;
};// 游戏区域坐标（对应到 UI 显示的时候就是字符个数）


struct Node {
	struct Point point;
	struct Node *pNext;
};// 链表结点


enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};// 蛇朝向


struct Snake {
	enum Direction direction;
	struct Node *pBody;
};// 蛇结构体


struct Game {
	int Width;	// 游戏区域宽度（不包括墙，对应到 UI 部分就是字符个数）
	int Height;	// 游戏区域高度（不包括墙，对应到 UI 部分就是字符个数）
	int Score;	// 当前得分
	int ScorePerFood;	// 每个食物得分

	struct Snake snake;	// 蛇
	struct Point FoodPoint;	// 当前食物坐标
};// 游戏结构体

#endif
