#pragma once

enum ActionStatus
{
	ACTION_NONE,
	ACTION_BEGIN,
	ACTION_TICK,
	ACTION_END,
	ACTION_MAX
};

class ClientManager;
class ActionScheduler;
class Action
{
public:
	Action();
	virtual ~Action();

	void	ActIt();

	void	SetClientManager( ClientManager* clientManager ) { m_ClientManager = clientManager; }

	// 액션이 꼭 콥스만의 고유 행동이 아니면 생성자에서는 corpsID는 따로 빼야할듯 
	void	SetOwnerCorpsID( int ownerCropsID ) { m_OwnerCropsID = ownerCropsID; }
	int		GetOwnerCorpsID() { return m_OwnerCropsID; }

	bool	Gozarani( ) { return m_NoEgg; }
	void	LowKick( ) { m_NoEgg = true; }

	int64_t GetTime( ) const { return m_Time; }
	void	SetTime( int64_t inputTime ) { m_Time = inputTime; }

	void	SetScheduler( ActionScheduler* actionScheduler ) { m_ActionScheduler = actionScheduler; }

protected:
	virtual void OnBegin() = 0;
	virtual void OnTick() = 0;
	virtual void OnEnd();

	int		m_OwnerCropsID = -1;
	int64_t	m_Time = 0;
	bool	m_NoEgg = false;
	ActionStatus m_ActionStatus = ACTION_BEGIN;

	ActionScheduler* m_ActionScheduler = nullptr;
	ClientManager* m_ClientManager = nullptr;
};

