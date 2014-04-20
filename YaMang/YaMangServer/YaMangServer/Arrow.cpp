#include "stdafx.h"
#include "Arrow.h"


Arrow::Arrow()
{
}


Arrow::~Arrow()
{
}

void Arrow::GenerateCorps( int corpsID, Position position )
{
	m_CorpsId = corpsID;
	m_Position = position;

	m_HP = 100;
	m_UnitNum = 10;
}
