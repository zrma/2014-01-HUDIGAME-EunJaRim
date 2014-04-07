#pragma once
#include "MacroSet.h"

class NetworkManager:public Singleton<NetworkManager>
{
public:
	NetworkManager();
	~NetworkManager();

	bool	Init();
	void	Destroy();

private:
	SOCKET	m_Socket;
};