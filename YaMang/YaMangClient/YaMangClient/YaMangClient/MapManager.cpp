#include "stdafx.h"
#include "MapManager.h"
#include "yaMangDxDll.h"

MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}

float MapManager::GetHeightByPosition( float x, float z )
{
	//////////////////////////////////////////////////////////////////////////
	// 실제로는 하이트맵에서 불러온 x, z에 맞는 y 값을 리턴하면 됨
	// 임시로 하드 코딩 구현 해 둠
	//////////////////////////////////////////////////////////////////////////

	return 10.0f;
}
