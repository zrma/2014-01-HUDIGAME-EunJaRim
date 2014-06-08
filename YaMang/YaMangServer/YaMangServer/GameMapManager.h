#pragma once
#include "SharedDefine.h"
#include "EnumSet.h"

struct Tile
{
	MapTileType m_MapTileType; // R
	MapObjectType m_MapObjectType; // G
	float m_MapHeight; // B
};

class GameMapManager
{
public:
	GameMapManager();
	~GameMapManager();


	bool		Initialize();
	bool		ReadMapFile( const char* filename );

	const std::vector<PositionInfo>&	GetKingPositionList() { return m_KingPositionList; }
	const std::vector<PositionInfo>&	GetGuardPositionList() { return m_GuardPositionList; }

	const PositionInfo&					GetKingPositionInfo( int index ) const { return m_KingPositionList.at( index ); }
	const PositionInfo&					GetGuardPositionInfo( int index ) const { return m_GuardPositionList.at( index ); }

	const MapTileType&					GetMapTileType( int x, int z ) const { return m_BattleMap.at( x ).at( z ).m_MapTileType; }
	const MapObjectType&				GetMapObjectType( int x, int z ) const { return m_BattleMap.at( x ).at( z ).m_MapObjectType; }
	const unsigned char&				GetMapHeight( int x, int z ) const { return m_BattleMap.at( x ).at( z ).m_MapHeight; }

private:
	std::vector<std::vector<Tile>>	m_BattleMap;

	std::vector<PositionInfo> m_KingPositionList;
	std::vector<PositionInfo> m_GuardPositionList;
};

