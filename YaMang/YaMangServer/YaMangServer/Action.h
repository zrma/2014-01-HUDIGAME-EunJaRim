#pragma once
class Action
{
public:
	Action();
	virtual ~Action();

	virtual void OnBegin() = 0;
	virtual void OnTick( ) = 0;
	virtual void OnEnd( ) = 0;

	void Stop() { m_IsPossible = false; }
	int64_t GetTime( ) const { return m_Time; }
	void SetTime( int64_t inputTime ) { m_Time = inputTime; }

private:
	int64_t	m_Time = 0;
	bool	m_IsPossible = false;
};

