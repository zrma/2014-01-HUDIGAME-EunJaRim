#pragma once

#include "MacroSet.h"

struct MESHOBJECT;

class GameManager : public Singleton<GameManager>
{
public:
	GameManager();
	~GameManager();

	bool Init();
	bool Process();
	void Destroy();

private:
	GameManager( const GameManager & );
	GameManager & operator=( const GameManager & );

	MESHOBJECT*		m_Mesh;
};