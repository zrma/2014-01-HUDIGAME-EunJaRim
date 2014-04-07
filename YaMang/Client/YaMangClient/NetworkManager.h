#pragma once
#include "MacroSet.h"

class NetworkManager:public Singleton<NetworkManager>
{
public:
	NetworkManager();
	~NetworkManager();

	bool	Init();
	void	Destroy();
	void	ProcessPacket();
	bool	Connect( const char* serverAddr, int port );
	

private:
	SOCKET	m_Socket;
};