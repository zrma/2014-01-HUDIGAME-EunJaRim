#include "stdafx.h"
#include "Sword.h"


Sword::Sword()
{
	m_MeshKey = MESH_KEY_UNIT_SWORD;
	// 호랑이

	m_EyePoint = { 0.0f, 0.3f, -20.0f };
	m_LookAtPoint = { 0.0f, 0.3f, -21.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 2.0f, 2.0f, 2.0f };
}


Sword::~Sword()
{
}
