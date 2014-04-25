#pragma once
class Action
{
public:
	Action();
	virtual ~Action();

	virtual void OnBegin() = 0;
	virtual void OnTick( ) = 0;
	virtual void OnEnd( ) = 0;

	// 액션이 꼭 콥스만의 고유 행동이 아니면 corpsID는 따로 빼야할듯 
	void	SetOwnerCorpsID( int ownerCropsID ) { m_OwnerCropsID = ownerCropsID; }
	int		GetOwnerCorpsID() { return m_OwnerCropsID; }

	bool	Gozarani( ) { return m_NoEgg; }
	void	LowKick( ) { m_NoEgg = true; }

	int64_t GetTime( ) const { return m_Time; }
	void	SetTime( int64_t inputTime ) { m_Time = inputTime; }

private:
	int		m_OwnerCropsID = -1;
	int64_t	m_Time = 0;
	bool	m_NoEgg = true;
};

