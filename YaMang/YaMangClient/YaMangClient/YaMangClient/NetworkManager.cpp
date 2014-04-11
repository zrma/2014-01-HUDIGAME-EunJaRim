#include "stdafx.h"
#include "NetworkManager.h"

#include "../../../PacketType.h"
#include "MainWindow.h"
#include "tinyxml.h"
#include "xpath_static.h"
#include <string>
#include <time.h>

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
	// 소켓에 오는 것들은 WM_SOCKET 이벤트로 처리하겠음

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

bool NetworkManager::Connect()
{

	const char * serverAddr = "127.0.0.1";
	int port;

	// xml 로드 테스트
	TiXmlDocument document = TiXmlDocument( "clientConfig.xml" );

	if ( document.LoadFile() )
	{
		std::string ipLoad = TinyXPath::S_xpath_string( document.RootElement(), "/client/ip/text()" ).c_str();
		std::string portLoad = TinyXPath::S_xpath_string( document.RootElement( ), "/client/port/text()" ).c_str( );
		
		serverAddr = ipLoad.c_str();
		port = std::stoi( portLoad );
		Log( "Loaded ip :%s Port Number :%s \n", serverAddr, portLoad.c_str() );
	}
	else
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"ClientConfig.xml Load Fail", L"Connection Load Fail!", MB_ICONINFORMATION | MB_OK );
	}

	struct hostent* host;

	if ( NULL == ( host = gethostbyname( serverAddr ) ) )
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

	PacketHeader header;

	if ( false == m_RecvBuffer.Peek( (char*)&header, sizeof( PacketHeader ) ) )
	{
		return;
	}


	if ( header.mSize > static_cast<short> ( m_RecvBuffer.GetCurrentSize() ) )
	{
		return;
	}

	switch ( header.mType )
	{
		case PKT_SC_LOGIN:
		{
			LoginResult recvData;
			if ( m_RecvBuffer.Read( (char*)&recvData, header.mSize ) )
			{
				// 패킷처리
				if ( recvData.mPlayerId == -1 )
				{
					/// 여기 걸리면 로그인 실패다.
					ExitProcess( -1 );
				}

// 				printf_s( "player[%d] \n", recvData.mPlayerId );
// 				g_MyClientId = recvData.mPlayerId;
// 				g_LoginComplete = true;
// 
// 				char buff[128] = { 0, };
// 				sprintf_s( buff, "LOGIN SUCCESS ClientId[%d] Name[%s] POS(%.4f, %.4f, %.4f) \n", g_MyClientId, recvData.mName, recvData.mPosX, recvData.mPosY, recvData.mPosZ );
// 
// 				static int ypos = 33;
// 				HDC hdc = GetDC( hWnd );
// 				TextOutA( hdc, 10, 33, buff, strlen( buff ) );
// 				ReleaseDC( hWnd, hdc );
// 
// 				/// 채팅 방송 패킷 보내는 타이머 돌리자.. 
// 				SetTimer( hWnd, 337, 3000, NULL );

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
 			if ( m_RecvBuffer.Read( (char*)&recvData, header.mSize ) )
 			{
 				/// 여기 걸리면 로그인 안된놈이 보낸거다
 				assert( recvData.mPlayerId != -1 );
 
 				char buff[MAX_CHAT_LEN] = { 0, };
 				sprintf_s( buff, "CHAT from Player[%s]: %s \n", recvData.mName, recvData.mChat );
 
// 				static int y2pos = 60;
// 				HDC hdc = GetDC( hWnd );
// 				TextOutA( hdc, 10, y2pos, buff, strlen( buff ) );
// 				ReleaseDC( hWnd, hdc );
// 				y2pos += 15;
// 				if ( y2pos > 600 )
// 					y2pos = 60;
// 
				Log( "%s \n", buff );
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

bool NetworkManager::Connected( WPARAM wParam, LPARAM lParam )
{
	// lParam 이 에러인지 검출 해보기
	if ( WSAGETSELECTERROR( lParam ) )
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"WSAGETSELECTERROR", L"Error", MB_OK | MB_ICONERROR );
		SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );
		return false;
	}

	// 에러 아니면 이벤트 검출해서 switch
	switch ( WSAGETSELECTEVENT( lParam ) )
	{
		case FD_CONNECT:
			// 연결이 되었다
		{
			/// NAGLE 끈다
			int opt = 1;
			setsockopt( m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof( int ) );

			srand( static_cast<unsigned int> ( time( NULL ) ) );
			/// 대략 1000~1100 의 ID로 로그인 해보자
			LoginRequest sendData;
			sendData.mPlayerId = 1000 + rand() % 101;

			if ( m_SendBuffer.Write( (const char*)&sendData, sendData.mSize ) )
			{
				PostMessage( MainWindow::GetInstance()->Window(), WM_SOCKET, wParam, FD_WRITE );
			}


			int nResult = WSAAsyncSelect( m_Socket, MainWindow::GetInstance()->Window(), WM_SOCKET, ( FD_CLOSE | FD_READ | FD_WRITE ) );
			if ( nResult )
			{
				assert( false );
				break;
			}
		}
			break;

		case FD_READ:
		{
			char inBuf[4096] = { 0, };

			int recvLen = recv( m_Socket, inBuf, 4096, 0 );
			// send() 함수와 반대

			// 소켓에서 읽어온 데이터를 일단 버퍼에 쓰자
			if ( !m_RecvBuffer.Write( inBuf, recvLen ) )
			{
				/// 버퍼 꽉찼다.
				assert( false );
			}

			ProcessPacket();
			//////////////////////////////////////////////////////////////////////////
			// 패킷 핸들링!
			//////////////////////////////////////////////////////////////////////////
		}
			break;
			//////////////////////////////////////////////////////////////////////////
			// 데이터를 받으면 -> 버퍼에 쓴 후에, 핸들링 하는 쪽에서 버퍼 데이터 뽑아서 처리
			//
			// 데이터를 보낼때 -> 버퍼에 쓴 후에, FD_WRITE 쪽에서 버퍼 데이터 뽑아서 send()
			//////////////////////////////////////////////////////////////////////////

		case FD_WRITE:
		{
			/// 실제로 버퍼에 있는것들 꺼내서 보내기
			int size = m_SendBuffer.GetCurrentSize();
			if ( size > 0 )
			{
				char* data = new char[size];
				m_SendBuffer.Peek( data );

				int sent = send( m_Socket, data, size, 0 );

				/// 다를수 있다
				if ( sent != size )
				{
					OutputDebugStringA( "sent != request\n" );
				}

				// 보낸 데이터는 지우자
				m_SendBuffer.Consume( sent );

				delete[] data;
			}

		}
			break;

		case FD_CLOSE:
		{
			MessageBox( MainWindow::GetInstance()->Window(), L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK );
			closesocket( m_Socket );
			SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );

			// 여기는 네트워크 매니저에서 Destroy 하고 매니저를 Release 하도록 한다
		}
			break;
	}

	return true;
}