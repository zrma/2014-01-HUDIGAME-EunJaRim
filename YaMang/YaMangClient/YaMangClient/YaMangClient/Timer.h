#pragma once
#include "MacroSet.h"

class Timer : public Singleton<Timer>
{
public:
	Timer();
	~Timer();


	UINT	GetNowTime();
	UINT	GetElapsedTime() { return m_GameStartTime - GetNowTime(); }

private:
	UINT	m_GameStartTime = 0;

};

