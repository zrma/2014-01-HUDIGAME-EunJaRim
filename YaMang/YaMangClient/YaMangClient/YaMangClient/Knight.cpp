#include "stdafx.h"
#include "Knight.h"


Knight::Knight( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	m_MeshKey = MESH_KEY_UNIT_KNIGHT;
	// 소녀

	m_EyePoint = { 0.0f, -1.5f, 0.0f };
	m_LookAtPoint = { 0.0f, -1.5f, 1.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 0.26f, 0.26f, 0.26f };

	SetStartPosition();
}


Knight::~Knight()
{
}
