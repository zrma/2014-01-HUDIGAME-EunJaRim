#pragma once
#include <d3dx9math.h>

const int	MAX_UNIT_IN_CORPS = 10;

const float NOT_A_NUMBER_FLOAT = -9999.0f;

///# 네이밍이 이상한데? 두개의 벡터인데.. 그냥 포지션이라기 보다.. 
struct Position
{
	D3DXVECTOR3		m_EyePoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_LookAtPoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
};
