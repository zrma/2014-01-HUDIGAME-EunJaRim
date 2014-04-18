#pragma once
#include "MacroSet.h"

class Timer : public Singleton<Timer>
{
public:
	Timer();
	~Timer();
	
	void	Tick();
	UINT	GetElapsedTime() { return m_DeltaTime; }

private:
	UINT	m_PrevTime = 0;
	UINT	m_DeltaTime = 0;
};

