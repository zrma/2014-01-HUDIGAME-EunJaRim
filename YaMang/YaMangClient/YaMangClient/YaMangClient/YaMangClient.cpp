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
	// _CrtSetBreakAlloc( 193 );

#ifdef _PRINT_CONSOLE
	Logger::GetInstance();
#endif

	if ( false == MainWindow::GetInstance()->Create( L"YaMang", WS_POPUPWINDOW, MAKEINTRESOURCE( IDC_YAMANGCLIENT ) ) )
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


		case WM_CREATE:
		{
			// Create a push button
			CreateWindow( L"BUTTON", L"CONNECT", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						  10, 10, 175, 23, hWnd, (HMENU)IDC_SEND_BUTTON, GetModuleHandle( NULL ), NULL );

			// 소켓 관련 초기화 작업
			//////////////////////////////////////////////////////////////////////////
			// 네트워크 매니저한테 가야 됨
			if ( false == Initialize() )
			{
				SendMessage( hWnd, WM_DESTROY, NULL, NULL );
				break;
			}

			//////////////////////////////////////////////////////////////////////////
			// http://blog.naver.com/alsduddl525/140123329159
			// http://blog.naver.com/merds/150000138666
			// http://blog.naver.com/popssong/70133058993 참조
			//
			// g_Socket 에 오는 것들은 WM_SOCKET 이벤트로 처리하겠음
			//////////////////////////////////////////////////////////////////////////
			int	nResult = WSAAsyncSelect( g_Socket, hWnd, WM_SOCKET, ( FD_CLOSE | FD_CONNECT ) );
			if ( nResult )
			{
				MessageBox( hWnd, L"WSAAsyncSelect failed", L"Critical Error", MB_ICONERROR );
				SendMessage( hWnd, WM_DESTROY, NULL, NULL );
				break;
			}

		}
			break;

		case WM_TIMER:
		{
			/// 주기적으로 채팅 날려보자.

			ChatBroadcastRequest sendData;

			sendData.mPlayerId = g_MyClientId;

			/// 랜덤 문자열을 채팅으로 날리기
			char* buff = sendData.mChat;
			for ( int i = 0; i < 300; ++i )
			{
				sendData.mChat[i] = (char)( 65 + ( rand() % 26 ) );
			}
			sendData.mChat[300] = '\0';

			if ( g_SendBuffer.Write( (const char*)&sendData, sendData.mSize ) )
				// sendData.mSize = sizeof(ChatBroadcastRequest);
			{
				PostMessage( hWnd, WM_SOCKET, wParam, FD_WRITE );
				//////////////////////////////////////////////////////////////////////////
				// http://blog.naver.com/gkqxhq324456/110177315036 참조
				//
				// 채팅을 날리려고 버퍼에 데이터도 넣어 두었으니, WM_SOCKET 이벤트를 발생시키자
				//////////////////////////////////////////////////////////////////////////
			}


		}
			break;

		case WM_COMMAND:
		{
			wmId = LOWORD( wParam );
			wmEvent = HIWORD( wParam );
			// 메뉴 선택을 구문 분석합니다.

			switch ( wmId )
			{
				case IDC_SEND_BUTTON:
				{
					if ( !g_LoginComplete && !Connect( szServer, nPort ) )
					{
						SendMessage( hWnd, WM_DESTROY, NULL, NULL );
						break;
					}

				}
					break;
				default:
					return DefWindowProc( hWnd, message, wParam, lParam );
			}
		}
			break;

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