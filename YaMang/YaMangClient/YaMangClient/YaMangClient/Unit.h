#pragma once
#include "Interface3D.h"
#include "EnumSet.h"
#include "Act.h"

class Corps;

class Unit: public Interface3D
{
public:
	Unit( Corps* owner, UINT unitId ): m_Corps( owner ), m_UnitId( unitId ) { ActDefault act; m_Act = act; }
	virtual ~Unit();

	virtual void	Update();
	virtual void	Render();

	int			GetUnitId() { return m_UnitId; }
	void		SetStartPosition();

	typedef std::function<void( const Unit& )> ActFunc;

	void	ChangeAct( ActFunc act ) { m_Act = act; }
	void	DoAct() { m_Act( *this ); }

	ActionStatusType	GetActionStatus() { return m_ActionStatus; }

protected:
	int			m_UnitId = -1;
	MeshKeyType	m_MeshKey = MESH_KEY_NONE;
	Corps*		m_Corps = nullptr;

	ActFunc		m_Act;
	ActionStatusType	m_ActionStatus = ACTION_STATUS_NONE;

private:
	Unit();
	Unit( const Unit& );
	Unit& operator=( const Unit& );
};