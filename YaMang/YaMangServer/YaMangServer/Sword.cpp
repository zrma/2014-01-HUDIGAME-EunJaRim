#include "stdafx.h"
#include "Sword.h"

Sword::~Sword()
{
}

void Sword::GenerateCorps( int corpsID, Position position )
{
	m_CorpsId = corpsID;
	m_Position = position;

	m_HP = 100;
	m_UnitNum = 10;
}
