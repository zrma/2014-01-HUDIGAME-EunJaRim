#pragma once
#include "SharedDefine.h"
#include "EnumSet.h"

struct Tile
{
	MapTileType m_MapTileType; // R
	MapObjectType m_MapObjectType; // G
	unsigned char m_MapHeight; // B
};

class GameMapManager
{
public:
	GameMapManager();
	~GameMapManager();


	bool		Initialize();
	bool		ReadMapFile( const char* filename );

	///# STL을 통쨰로 리턴하는 구조는 좋은 구조가 아니다. -.-; OOP 개념과 정면으로 충돌하는 개념.. (상대의 내장을 들어내서 마구 조작한 다음에 집어 넣는거라고 보면 됨)
	const std::vector<PositionInfo>&	GetKingPositionList() { return m_KingPositionList; }
	const std::vector<PositionInfo>&	GetGuardPositionList() { return m_GuardPositionList; }

	///# 이거 위험할 수 있는데... 백터 STL의 해당 인덱스에 항상 원하는 데이터가 있다고 보장할 수 있는가? index로 쿼리 하지 말고 다른 방법(다른 key이용)을 찾아야 한다.
	const PositionInfo&					GetKingPositionInfo( int index ) const { return m_KingPositionList.at( index ); }
	const PositionInfo&					GetGuardPositionInfo( int index ) const { return m_GuardPositionList.at( index ); }

	const MapTileType&					GetMapTileType( int x, int z ) const { return m_BattleMap.at( x ).at( z ).m_MapTileType; }
	const MapObjectType&				GetMapObjectType( int x, int z ) const { return m_BattleMap.at( x ).at( z ).m_MapObjectType; }
	const unsigned char&				GetMapHeight( int x, int z ) const { return m_BattleMap.at( x ).at( z ).m_MapHeight; }

private:
	std::vector<std::vector<Tile>>	m_BattleMap; ///< 맵 정보는 어차피 크기가 정해지니까.. 백터로 하지 말고 고정크기 배열로.... 백터는 이런 용도로 쓰라고 있는게 아님 ㅜ (실행 시간중에 내부 원소들이 마구 바뀔 때 쓰는거..)

	std::vector<PositionInfo> m_KingPositionList;
	std::vector<PositionInfo> m_GuardPositionList;
};

