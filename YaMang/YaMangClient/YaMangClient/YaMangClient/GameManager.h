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
	void Stop() { m_Continue = false;  }

private:
	bool			m_Continue = true;	// agebreak : C++0x에 추가된 초기화

	MESHOBJECT*		m_Mesh = nullptr;
};