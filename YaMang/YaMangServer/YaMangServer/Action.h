#pragma once

enum ActionStatus
{
	ACTION_NONE,
	ACTION_BEGIN,
	ACTION_TICK,
	ACTION_END,
	ACTION_MAX
};

class Corps;
class ClientManager;
class Action
{
public:
	Action();
	virtual ~Action();

	void		DoAction();

	void		SetClientManager( ClientManager* clientManager ) { m_ClientManager = clientManager; }

	// 액션이 꼭 콥스만의 고유 행동이 아니면 생성자에서는 corpsID는 따로 빼야할듯 
	void		SetOwnerCorps( Corps* ownerCrops ) { m_OwnerCrops = ownerCrops; }
	Corps*		GetOwnerCorps( ) { return m_OwnerCrops; }

	bool		Gozarani( ) { return m_NoEgg; }
	void		LowKick( ) { m_NoEgg = true; }

	ULONGLONG	GetTime( ) const { return m_Time; }
	void		SetTime( ULONGLONG inputTime ) { m_Time = inputTime; }

protected:
	virtual void OnBegin() = 0;
	virtual void OnTick() = 0;
	virtual void OnEnd();

	Corps*				m_OwnerCrops = nullptr;
	ULONGLONG			m_Time = 0;
	bool				m_NoEgg = false;
	ActionStatus		m_ActionStatus = ACTION_BEGIN;

	
	ClientManager*		m_ClientManager = nullptr;
};

