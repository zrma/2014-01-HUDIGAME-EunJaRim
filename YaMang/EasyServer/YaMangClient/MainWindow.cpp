#include "stdafx.h"
#include "MainWindow.h"
#include "GameManager.h"

MainWindow::MainWindow()
: m_GameManager(GameManager::GetInstance())
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
		case WM_CREATE:
		{
			// 요거 자원 초기화로 따로 뽑아내야 할 지 고민

		}
			return 0;

		case WM_TIMER:
		{
			// 타이머 이벤트가 발생하면 이곳에서 핸들링 해 봅니다.
		}
			return 0;

		case WM_SOCKET:
		{
			// 네트워크 매니저(싱글톤 객체)를 활용하여 패킷을 핸들링 해 줍니다.
		}
			return 0;
		case WM_DESTROY:
		{
			m_GameManager->Release();
			PostQuitMessage( 0 );
		}
			return 0;

		case WM_ERASEBKGND:
			return 1;

		default:
			return DefWindowProc( m_hwnd, uMsg, wParam, lParam );
	}

	return 0;
}

int MainWindow::RunGame()
{
	m_GameManager->Init();

	MSG msg = {0,};

	while ( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if ( m_GameManager->Process() == false )
		{
			m_GameManager->Destroy();
			PostQuitMessage( 0 );
		}
	}

	return (int)msg.wParam;
}
