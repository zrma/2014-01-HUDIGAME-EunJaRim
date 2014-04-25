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

	int		GetCorpsID() { return m_CorpsId; }
	int		GetCorpsHP() { return m_UnitList.size(); }

	void			SetFormation( FormationType formation ) { m_Formation = formation; }
	D3DXVECTOR3		GetFormation( int unitId ) const;

	typedef std::function<void( Corps& )>	ActionFunc;

	void	ChangeAction( ActionFunc action ) { m_Action = action; }
	void	DoAction() { m_Action( *this ); }

	void			SetTargetVector( float x, float z ) { m_TargetVector.x = x; m_TargetVector.z = z; }
	D3DXVECTOR3		GetTargetVector( ) { return m_TargetVector; }
	void			SetSpeed( float speed ) { m_Speed = speed; }
	void			GoFoward();
	
private:
	Corps();
	Corps( const Corps& );
	Corps& operator=( const Corps& );

	std::vector<Unit*>		m_UnitList;
	int						m_CorpsId = -1;
	int						m_OwnerPlayerID = -1;

	float	m_Speed;

	FormationType			m_Formation = FormationType::FORMATION_NONE;
	std::array<BreadBoard*, static_cast<size_t>(FormationType::FORMATION_MAX)> m_FormationArray;

	ActionFunc	m_Action;
	D3DXVECTOR3	m_TargetVector = { NOT_A_NUMBER_FLOAT, 0.0f, NOT_A_NUMBER_FLOAT };
};
