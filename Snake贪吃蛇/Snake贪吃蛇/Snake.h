#ifndef __SNAKE_H__
#define __SNAKE_H__


struct Point {
	int x;
	int y;
};// ��Ϸ�������꣨��Ӧ�� UI ��ʾ��ʱ������ַ�������


struct Node {
	struct Point point;
	struct Node *pNext;
};// ������


enum Direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};// �߳���


struct Snake {
	enum Direction direction;
	struct Node *pBody;
};// �߽ṹ��


struct Game {
	int Width;	// ��Ϸ�����ȣ�������ǽ����Ӧ�� UI ���־����ַ�������
	int Height;	// ��Ϸ����߶ȣ�������ǽ����Ӧ�� UI ���־����ַ�������
	int Score;	// ��ǰ�÷�
	int ScorePerFood;	// ÿ��ʳ��÷�

	struct Snake snake;	// ��
	struct Point FoodPoint;	// ��ǰʳ������
};// ��Ϸ�ṹ��

#endif
