#pragma once
#include "Interface3D.h"
#include "EnumSet.h"

class Corps;

class Unit: public Interface3D
{
public:
	Unit( Corps* owner, UINT unitId ): m_Corps( owner ), m_UnitID( unitId ) { ClearAct(); }
	virtual ~Unit();

	virtual void	Update();
	virtual void	Render();

	int			GetUnitId() { return m_UnitID; }
	void		SetStartPosition();
	
	void		FindDestination();

	typedef std::function<void( Unit&, Corps& )> ActFunc;

	void	ChangeAct( ActFunc act ) { m_Act = act; }
	void	DoAct() { m_Act( *this, *m_Corps ); }
	void	ClearAct();

	ActionStatusType	GetActionStatus() { return m_ActionStatus; }

protected:
	int			m_UnitID = -1;
	MeshKeyType	m_MeshKey = MESH_KEY_NONE;
	Corps*		m_Corps = nullptr;

	ActFunc		m_Act;
	ActionStatusType	m_ActionStatus = ACTION_STATUS_NONE;

private:
	Unit();
	Unit( const Unit& );
	Unit& operator=( const Unit& );
};