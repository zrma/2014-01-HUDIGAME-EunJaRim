#pragma once
#include <d3dx9math.h>

const int	MAX_UNIT_IN_CORPS = 10;
const float NOT_A_NUMBER_FLOAT = -9999.0f;
// #pragma pack(push, 1)
struct Position
{
	// bool	test = false;
	D3DXVECTOR3		m_EyePoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_LookAtPoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
};
// #pragma pack(pop)