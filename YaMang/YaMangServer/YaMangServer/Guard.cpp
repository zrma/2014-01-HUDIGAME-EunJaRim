#include "stdafx.h"
#include "Guard.h"


Guard::~Guard()
{
}

void Guard::GenerateCorps( int corpsID, Position position )
{
	m_CorpsId = corpsID;
	m_Position = position;

	m_HP = 100;
	m_UnitNum = 10;
}
