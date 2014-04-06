#pragma once

#include "MacroSet.h"

class GameManager : public Singleton<GameManager>
{
public:
	GameManager();
	~GameManager();

	bool Init();
	bool Process();
	void Destory();

private:
	GameManager( const GameManager & );
	GameManager & operator=( const GameManager & );
};