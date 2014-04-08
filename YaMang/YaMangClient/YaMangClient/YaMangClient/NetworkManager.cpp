#include "stdafx.h"
#include "NetworkManager.h"

#include "../../../PacketType.h"
#include "MainWindow.h"

#pragma comment(lib,"ws2_32.lib")

const int BUFSIZE = 1024 * 10;

NetworkManager::NetworkManager()
:m_Socket( NULL ), m_RecvBuffer( BUFSIZE ), m_SendBuffer(BUFSIZE)
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
	// MessageBox( MainWindow::GetInstance()->Window(), L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK );
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

void NetworkManager::ProcessPacket()
{
	// 하단의 것들 각각 이벤트 핸들링 해 줘야 됨

	/*
	while ( true )
	{
		PacketHeader header;
		
		if ( false == g_RecvBuffer.Peek( (char*)&header, sizeof( PacketHeader ) ) )
			break;

		if ( header.mSize > static_cast<short> ( g_RecvBuffer.GetCurrentSize() ) )
			break;

		switch ( header.mType )
		{
			case PKT_SC_LOGIN:
			{
				LoginResult recvData;
				if ( g_RecvBuffer.Read( (char*)&recvData, header.mSize ) )
				{
					// 패킷처리
					if ( recvData.mPlayerId == -1 )
					{
						/// 여기 걸리면 로그인 실패다.
						ExitProcess( -1 );
					}


					g_MyClientId = recvData.mPlayerId;
					g_LoginComplete = true;

					char buff[128] = { 0, };
					sprintf_s( buff, "LOGIN SUCCESS ClientId[%d] Name[%s] POS(%.4f, %.4f, %.4f) \n", g_MyClientId, recvData.mName, recvData.mPosX, recvData.mPosY, recvData.mPosZ );

					static int ypos = 33;
					HDC hdc = GetDC( hWnd );
					TextOutA( hdc, 10, 33, buff, strlen( buff ) );
					ReleaseDC( hWnd, hdc );

					/// 채팅 방송 패킷 보내는 타이머 돌리자.. 
					SetTimer( hWnd, 337, 3000, NULL );

				}
				else
				{
					assert( false );
				}
			}
				break;

			case PKT_SC_CHAT:
			{
				ChatBroadcastResult recvData;
				if ( g_RecvBuffer.Read( (char*)&recvData, header.mSize ) )
				{
					/// 여기 걸리면 로그인 안된놈이 보낸거다
					assert( recvData.mPlayerId != -1 );

					char buff[MAX_CHAT_LEN] = { 0, };
					sprintf_s( buff, "CHAT from Player[%s]: %s \n", recvData.mName, recvData.mChat );

					static int y2pos = 60;
					HDC hdc = GetDC( hWnd );
					TextOutA( hdc, 10, y2pos, buff, strlen( buff ) );
					ReleaseDC( hWnd, hdc );
					y2pos += 15;
					if ( y2pos > 600 )
						y2pos = 60;

				}
				else
				{
					assert( false );
				}
			}
				break;
			default:
				assert( false );
		}

	}
	*/
}
