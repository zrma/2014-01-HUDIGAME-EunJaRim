// YaMangClient.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "YaMangClient.h"

#include <stdio.h>
#include <time.h>
#include <crtdbg.h>

#include "Logger.h"
#include "MainWindow.h"

#pragma comment(lib,"ws2_32.lib")

int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hInstance );
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// _CrtSetBreakAlloc( #### );
	// 메모리 릭을 체크하려면 위의 #### 칸에 릭 난 곳 { 숫자 } 표기 된 숫자를 넣어주면 됩니다.

#ifdef _PRINT_CONSOLE
	Logger::GetInstance();
#endif

	// WS_POPUPWINDOW
	if ( false == MainWindow::GetInstance()->Create( L"YaMang", WS_OVERLAPPEDWINDOW , MAKEINTRESOURCE( IDC_YAMANGCLIENT ) ) )
	{
		return 0;
	}

	MainWindow::GetInstance()->Display( nCmdShow );
	
	int result = MainWindow::GetInstance()->RunGame();
	MainWindow::Release();

#ifdef _PRINT_CONSOLE
	Logger::Release();
#endif

	return result;
}


//////////////////////////////////////////////////////////////////////////
// 이 안은 MainWindow의 HandleMessage 안으로 녹여야 됨
//
// 생각을 조금 더 해야 된다!
//////////////////////////////////////////////////////////////////////////

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
/*
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( message )
	{
		
			//////////////////////////////////////////////////////////////////////////
			// 소켓 이벤트
			//////////////////////////////////////////////////////////////////////////
		case WM_SOCKET:
		{
			// lParam 이 에러인지 검출 해보기
			if ( WSAGETSELECTERROR( lParam ) )
			{
				MessageBox( hWnd, L"WSAGETSELECTERROR", L"Error", MB_OK | MB_ICONERROR );
				SendMessage( hWnd, WM_DESTROY, NULL, NULL );
				break;
			}

			// 에러 아니면 이벤트 검출해서 switch
			switch ( WSAGETSELECTEVENT( lParam ) )
			{
				case FD_CONNECT:
					// 연결이 되었다
				{
					/// NAGLE 끈다
					int opt = 1;
					::setsockopt( g_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof( int ) );

					srand( static_cast<unsigned int> ( time( NULL ) ) );
					/// 대략 1000~1100 의 ID로 로그인 해보자
					LoginRequest sendData;
					sendData.mPlayerId = 1000 + rand() % 101;

					if ( g_SendBuffer.Write( (const char*)&sendData, sendData.mSize ) )
					{
						PostMessage( hWnd, WM_SOCKET, wParam, FD_WRITE );
					}


					int nResult = WSAAsyncSelect( g_Socket, hWnd, WM_SOCKET, ( FD_CLOSE | FD_READ | FD_WRITE ) );
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

					int recvLen = recv( g_Socket, inBuf, 4096, 0 );
					// send() 함수와 반대

					// 소켓에서 읽어온 데이터를 일단 버퍼에 쓰자
					if ( !g_RecvBuffer.Write( inBuf, recvLen ) )
					{
						/// 버퍼 꽉찼다.
						assert( false );
					}

					ProcessPacket( hWnd );
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
					int size = g_SendBuffer.GetCurrentSize();
					if ( size > 0 )
					{
						char* data = new char[size];
						g_SendBuffer.Peek( data );

						int sent = send( g_Socket, data, size, 0 );

						/// 다를수 있다
						if ( sent != size )
							OutputDebugStringA( "sent != request\n" );

						// 보낸 데이터는 지우자
						g_SendBuffer.Consume( sent );

						delete[] data;
					}

				}
					break;

				case FD_CLOSE:
				{
					MessageBox( hWnd, L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK );
					closesocket( g_Socket );
					SendMessage( hWnd, WM_DESTROY, NULL, NULL );

					// 여기는 네트워크 매니저에서 Destroy 하고 매니저를 Release 하도록 한다
				}
					break;
			}
		}
			break;

		case WM_DISPLAYCHANGE:
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			// TODO: 여기에 그리기 코드를 추가합니다.
			EndPaint( hWnd, &ps );
			break;

		case WM_SIZE:
		{
			int x = (int)LOWORD( lParam );
			int y = (int)HIWORD( lParam );
			// m_scene.Resize( x, y );
			InvalidateRect( hWnd, NULL, FALSE );
		}
			return 0;

		case WM_ERASEBKGND:
			return 1;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

*/