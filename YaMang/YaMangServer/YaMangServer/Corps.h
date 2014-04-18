#pragma once
#include "SharedDefine.h"
#include "EnumSet.h"

class Unit;
class Corps
{
public:
	Corps( );
	~Corps();
	
	void			GenerateCorps( UnitType type, int corpsID, Position position );

	int				GetCorpsID() { return m_CorpsId; }
	short			GetCorpsSize() { return static_cast<short>( m_UnitList.size( ) ); }
	Position		GetPosition() { return m_Position; }

private:
	std::vector<Unit*>		m_UnitList;
	int						m_CorpsId;

	Position				m_Position;
	
	// 방향은 어떻게 하지?? 지금부터 벡터로??
	// 유닛 속성과 여기 속성과 뭔가 섞여있는 느낌 다시 굴다리가 필요할듯

};

