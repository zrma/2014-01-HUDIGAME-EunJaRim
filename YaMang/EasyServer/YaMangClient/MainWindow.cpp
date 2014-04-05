#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow()
{
}


MainWindow::~MainWindow()
{
}

LRESULT MainWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		case WM_DESTROY:
			// m_GameManager.Release();
			PostQuitMessage( 0 );
			return 0;

		case WM_ERASEBKGND:
			return 1;

		default:
			return DefWindowProc( m_hwnd, uMsg, wParam, lParam );
	}
}
