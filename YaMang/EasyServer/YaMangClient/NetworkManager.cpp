#include "stdafx.h"
#include "NetworkManager.h"

#include "..\..\PacketType.h"

#pragma comment(lib,"ws2_32.lib")

NetworkManager::NetworkManager()
{
}


NetworkManager::~NetworkManager()
{
}

bool NetworkManager::Init()
{
	WSADATA WsaDat;

	int nResult = WSAStartup( MAKEWORD( 2, 2 ), &WsaDat );
	if ( nResult != 0 )
		return false;

	m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_Socket == INVALID_SOCKET )
		return false;

	return true;
}

void NetworkManager::Destroy()
{

}
