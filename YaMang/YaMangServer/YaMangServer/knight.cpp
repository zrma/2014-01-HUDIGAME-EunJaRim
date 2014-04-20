#include "stdafx.h"
#include "Knight.h"


Knight::Knight()
{
}


Knight::~Knight()
{
}

void Knight::GenerateCorps( int corpsID, Position position )
{
	m_CorpsId = corpsID;
	m_Position = position;

	m_HP = 100;
	m_UnitNum = 10;
}
