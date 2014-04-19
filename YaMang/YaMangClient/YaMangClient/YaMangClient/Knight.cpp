#include "stdafx.h"
#include "Knight.h"


Knight::Knight()
{
	m_MeshKey = MESH_KEY_UNIT_KNIGHT;
	// 소녀

	m_EyePoint = { 3.0f, -1.5f, 20.0f };
	m_LookAtPoint = { 3.5f, -1.5f, 21.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 0.26f, 0.26f, 0.26f };
}


Knight::~Knight()
{
}
