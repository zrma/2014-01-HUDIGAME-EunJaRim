#include "stdafx.h"
#include "Pike.h"


Pike::Pike()
{
}


Pike::~Pike()
{
}

void Pike::GenerateCorps( int corpsID, Position position )
{
	m_CorpsId = corpsID;
	m_Position = position;

	m_HP = 100;
	m_UnitNum = 10;
}
