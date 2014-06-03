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
class GameRoom;
class Action
{
public:
	Action();
	virtual ~Action();

	void		DoAction();

	void		SetGameRoom( GameRoom* gameRoom ) { m_GameRoom = gameRoom; }

	// 액션이 꼭 콥스만의 고유 행동이 아니면 생성자에서는 corpsID는 따로 빼야할듯 
	void		SetOwnerCorps( Corps* ownerCrops ) { m_OwnerCrops = ownerCrops; }
	Corps*		GetOwnerCorps() const { return m_OwnerCrops; }
	void		ClearOwnerCorps() { m_OwnerCrops = nullptr; }


	bool		Gozarani() const { return m_NoEgg; }
	void		LowKick() { m_NoEgg = true; }

	ULONGLONG	GetTime() const { return m_Time; }
	void		SetTime( ULONGLONG inputTime ) { m_Time = inputTime; }

	ActionStatus GetActionStatus() const { return m_ActionStatus; }

protected:
	virtual void OnBegin() = 0;
	virtual void OnTick() = 0;
	virtual void OnEnd();

	Corps*				m_OwnerCrops = nullptr;
	ULONGLONG			m_Time = 0;
	bool				m_NoEgg = false;
	ActionStatus		m_ActionStatus = ACTION_BEGIN; ///# ACTION_NONE을 디폴트로 해주는게 좋겠지.. 실제로 DoAction이 시작 되었을 때 ACTION_BEGIN로 바꿔주고... 그래야 버그 찾을때 편함.


	GameRoom*		m_GameRoom = nullptr;
};

