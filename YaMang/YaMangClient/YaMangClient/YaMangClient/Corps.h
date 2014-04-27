#pragma once
#include "EnumSet.h"
#include "Interface3D.h"

struct PositionInfo;
struct BreadBoard;
class Unit;

class Corps: public Interface3D
{
public:
	Corps( int corpsId, int playerId, PositionInfo pos );
	~Corps();

	void	Update();
	void	Render();
	void	Create(UINT num, UnitType unitType );
	void	SetVisible( bool visible );

	int		GetCorpsID() { return m_CorpsId; }
	int		GetCorpsHP() { return m_UnitList.size(); }

	void			SetFormation( FormationType formation ) { m_TargetFormation = formation; }
	D3DXVECTOR3		GetFormation( int unitId ) const;

	typedef std::function<void( Corps& )>	ActionFunc;

	void			ChangeAction( ActionFunc action ) { m_Action = action; }
	void			DoAction() { m_Action( *this ); }
	void			ClearAction();

	typedef std::function<void( Unit&, Corps& )> ActFunc;

	void			SetAct( ActFunc act );

	void			SetLookAtPosition( float x, float z ) { m_LookAtPoint.x = m_EyePoint.x + x; m_LookAtPoint.y = m_EyePoint.y;  m_LookAtPoint.z = m_EyePoint.z + z; }
	D3DXVECTOR3		GetLookAtVector( ) { return (m_LookAtPoint - m_EyePoint); }
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

	FormationType			m_NowFormation = FormationType::FORMATION_NONE;
	FormationType			m_TargetFormation = FormationType::FORMATION_NONE;
	std::array<BreadBoard*, static_cast<size_t>(FormationType::FORMATION_MAX)> m_FormationArray;

	ActionFunc	m_Action;
};
