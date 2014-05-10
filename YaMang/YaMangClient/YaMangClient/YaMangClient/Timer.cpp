#include "stdafx.h"
#include "Timer.h"


Timer::Timer(): m_PrevTime( timeGetTime() ), m_NowTime( timeGetTime() )
{
}


Timer::~Timer()
{
}

void Timer::Tick()
{
	UINT m_NowTime = timeGetTime();
	m_DeltaTime = m_NowTime - m_PrevTime;
	m_PrevTime = m_NowTime;
}
