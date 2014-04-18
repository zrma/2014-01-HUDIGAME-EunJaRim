#include "stdafx.h"
#include "Timer.h"


Timer::Timer(): m_PrevTime( timeGetTime() )
{
}


Timer::~Timer()
{
}

void Timer::Tick()
{
	UINT nowTime = timeGetTime();
	m_DeltaTime = nowTime - m_PrevTime;
	m_PrevTime = nowTime;
}
