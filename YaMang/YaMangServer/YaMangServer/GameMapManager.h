#pragma once
#include "SharedDefine.h"

struct Tile
{
	unsigned char R; // 맵 데이터의 R값 아직 어떤 속성값을 넣을지 정해지지 않음 맵툴쪽과 합의 필요
	unsigned char G;
	unsigned char B; // 아마도 height값이 올 예정
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
	const PositionInfo&					GetGuardPositionInfo( int index ) const { return m_GuardPositionList.at( index ); }

private:
	std::vector<std::vector<Tile>>	m_BattleMap;

	std::vector<PositionInfo> m_KingPositionList;
	std::vector<PositionInfo> m_GuardPositionList;
};

