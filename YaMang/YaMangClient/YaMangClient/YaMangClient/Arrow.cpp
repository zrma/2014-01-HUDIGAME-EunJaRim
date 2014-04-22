#include "stdafx.h"
#include "Arrow.h"
#include "Timer.h"

Arrow::Arrow( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	m_MeshKey = MESH_KEY_UNIT_ARROW;
	// 토로

	m_EyePoint = { -8.0f, -1.2f, 20.0f };
	m_LookAtPoint = { -7.0f, -1.2f, 19.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };
}

Arrow::~Arrow()
{
}