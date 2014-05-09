#pragma once
#include "MacroSet.h"

class PlayerManager : public Singleton<PlayerManager>
{
public:
	PlayerManager();
	~PlayerManager();

	bool	IsSelectedCorps() { return ( m_SelectedCorpsList.size() != 0 ); }
	void	AddToSelectedCorps( int corpsID ) { m_SelectedCorpsList.push_back( corpsID ); }
	void	ClearSelectedCorps() { m_SelectedCorpsList.clear(); }

private:
	std::list<int>	m_SelectedCorpsList;
};

