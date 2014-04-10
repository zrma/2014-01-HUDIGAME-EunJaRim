#include "stdafx.h"
#include "MainWindow.h"
#include "GameManager.h"
#include "NetworkManager.h"
#include "Renderer.h"

MainWindow::MainWindow()
{
}


MainWindow::~MainWindow()
{
}

LRESULT MainWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// int wmId, wmEvent;

	switch ( uMsg )
	{
		case WM_TIMER:
		{
			// 타이머 이벤트가 발생하면 이곳에서 핸들링 해 봅니다.
			
			//////////////////////////////////////////////////////////////////////////
			// 9느님 원 소스에서 발췌 - 주기적으로 채팅하기
			/*
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
			*/
			//////////////////////////////////////////////////////////////////////////
		}
			return 0;

		case WM_SOCKET:
		{
			// 네트워크 매니저(싱글톤 객체)를 활용하여 패킷을 핸들링 해 줍니다.
		}
			return 0;
		case WM_DESTROY:
		{	
			PostQuitMessage( 0 );
		}
			return 0;

		/*
		case WM_MOUSEMOVE:
		{
			MouseX = LOWORD( IParam );
			MouseY = HIWORD( IParam );
		}
			break;
		*/
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_ESCAPE:
					GameManager::GetInstance()->Stop();
					return 0;
			}

		case WM_ERASEBKGND:
			return 1;

		default:
			return DefWindowProc( m_hwnd, uMsg, wParam, lParam );
	}

	return 0;
}

int MainWindow::RunGame()
{
	MSG msg = {0,};

	if ( false == GameManager::GetInstance()->Init() )
	{
		GameManager::GetInstance()->Destroy();
		GameManager::Release();
		// 사실 여기 이렇게 그냥 두었지만,
		// Init() 함수 내부에서 방어 코드 종류별로 케바케로 실패 뜬 부분 전부 잡아서 Release() 해줘야 됨.
		//
		// 뭔가 스마트 포인터의 필요성이 보이는 것 같다 =ㅅ=
		
		MessageBox( m_hwnd, L"Gama Manager Initialization Error", L"Game Manager Init Error!", MB_ICONINFORMATION | MB_OK );
		SendMessage( m_hwnd, WM_DESTROY, NULL, NULL );
		return false;
	}

	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( false == GameManager::GetInstance()->Process() )
		{
			GameManager::GetInstance()->Destroy();
			GameManager::Release();
			
			break;
		}
	}

	return (int)msg.wParam;
}
