#include "stdafx.h"
#include "NetworkManager.h"

#include "..\..\PacketType.h"
#include "MainWindow.h"

#pragma comment(lib,"ws2_32.lib")

NetworkManager::NetworkManager()
:m_Socket(NULL)
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
	{
		return false;
	}

	m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_Socket == INVALID_SOCKET )
	{
		return false;
	}

	nResult = WSAAsyncSelect( m_Socket, MainWindow::GetInstance()->Window(), WM_SOCKET, ( FD_CLOSE | FD_CONNECT ) );
	if ( nResult )
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"WSAAsyncSelect failed", L"Critical Error", MB_ICONERROR );
		SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );
		return false;
	}

	return true;
}

void NetworkManager::Destroy()
{
	MessageBox( MainWindow::GetInstance()->Window(), L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK );
	closesocket( m_Socket );
	SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );
}

bool NetworkManager::Connect( const char* serverAddr, int port )
{
	struct hostent* host;

	if ( ( host = gethostbyname( serverAddr ) ) == NULL )
	{
		return false;
	}

	// Set up our socket address structure
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons( port );
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *( (unsigned long*)host->h_addr );

	if ( SOCKET_ERROR == connect( m_Socket, (LPSOCKADDR)( &SockAddr ), sizeof( SockAddr ) ) )
	{
		if ( GetLastError() != WSAEWOULDBLOCK )
			return false;
	}

	return true;
}
