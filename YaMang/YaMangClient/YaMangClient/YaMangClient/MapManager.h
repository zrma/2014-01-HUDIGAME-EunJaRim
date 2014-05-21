#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

class MapManager: public Singleton<MapManager>
{
public:
	MapManager();
	~MapManager();

	float	GetHeightByPosition( D3DXVECTOR3 pos ) { return GetHeightByPosition( pos.x, pos.z ); }
	float	GetHeightByPosition( float x, float z );

private:
};