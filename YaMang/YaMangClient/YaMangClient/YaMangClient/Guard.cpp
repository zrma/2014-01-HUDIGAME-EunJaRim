#include "stdafx.h"
#include "Guard.h"


Guard::Guard( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	m_MeshKey = MESH_KEY_UNIT_GUARD;
	// 배트맨

	m_EyePoint = { -4.0f, -1.2f, 20.0f };
	m_LookAtPoint = { -3.5f, -1.2f, 19.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 2.0f, 2.0f, 2.0f };
}


Guard::~Guard()
{
}
