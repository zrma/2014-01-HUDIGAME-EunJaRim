#pragma once
#include "EnumSet.h"
#include "Interface3D.h"

struct Position;
struct BreadBoard;
class Unit;

class Corps: public Interface3D
{
public:
	Corps( int corpsId, int playerId, Position pos );
	~Corps();

	void	Update();
	void	Render();
	void	Create(UINT num, UnitType unitType );
	void	SetVisible( bool visible );

	void			SetFormation( FormationType formation ) { m_Formation = formation; }
	D3DXVECTOR3		GetFormation( int unitId ) const;
private:
	Corps();
	Corps( const Corps& );
	Corps& operator=( const Corps& );

	std::vector<Unit*>		m_UnitList;
	int						m_CorpsId = -1;
	int						m_OwnerPlayerID = -1;

	FormationType			m_Formation = FormationType::FORMATION_NONE;
	std::array<BreadBoard*, static_cast<size_t>(FormationType::FORMATION_MAX)> m_FormationArray;	
};
