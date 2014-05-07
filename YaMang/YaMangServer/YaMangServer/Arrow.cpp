#include "stdafx.h"
#include "Arrow.h"

Arrow::Arrow( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler )
	: Corps( playerID, corpsID, position, actionScheduler ) ///# 부모의 생성자 호출은 이렇게 잘 보이도록..
{
	// xml로 뺄것
	m_HP = 100.0f;
	m_MoveSpeed = 3.0f;
	m_AttackRange = 12.f;
	m_AttackPower = 15.0f;
	m_Defense = 1.0f;
	m_AttackDelay = 5000;
}

Arrow::~Arrow()
{
}


