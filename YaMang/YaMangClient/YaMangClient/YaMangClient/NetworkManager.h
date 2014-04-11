#pragma once
#include "MacroSet.h"
#include "CircularBuffer.h"

class NetworkManager:public Singleton<NetworkManager>
{
public:
	NetworkManager();
	~NetworkManager();

	bool	Init();
	void	Destroy();
	void	ProcessPacket();
	bool	Connect();
	bool	Connected( WPARAM wParam, LPARAM lParam );
	

private:
	SOCKET			m_Socket = NULL;
	CircularBuffer	m_RecvBuffer;
	CircularBuffer	m_SendBuffer;

	friend class LoginHandler;
	friend class ChatHandler;
};