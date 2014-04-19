#include "stdafx.h"
#include "Pike.h"


Pike::Pike()
{
	m_MeshKey = MESH_KEY_UNIT_PIKE;
	// 중국

	m_EyePoint = { -7.0f, -1.7f, -20.0f };
	m_LookAtPoint = { -8.0f, -1.7f, -21.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 0.8f, 0.8f, 0.8f };
}


Pike::~Pike()
{
}
