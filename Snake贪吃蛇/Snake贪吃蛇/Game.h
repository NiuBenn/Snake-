#ifndef __GAME_H__
#define __GAME_H__
#include "Snake.h"

struct Game * CreateGame();// ��������ʼ����Ϸ
void StartGame(struct Game *pGame);// ��ʼ��Ϸ
void DestroyGame(struct Game *pGame);// ������Ϸ��Դ

#endif