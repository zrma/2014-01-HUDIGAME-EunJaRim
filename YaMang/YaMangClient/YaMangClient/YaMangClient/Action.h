#pragma once
class Action
{
public:
	Action();
	virtual ~Action();

	virtual void	OnBegin() = 0;
	virtual void	OnTick() = 0;
	virtual void	OnEnd() = 0;

	void	Stop() { m_IsPossible = false; }

private:
	bool	m_IsPossible = false;
};

