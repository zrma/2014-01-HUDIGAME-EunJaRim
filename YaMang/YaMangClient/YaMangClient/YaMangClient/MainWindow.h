#pragma once

#include "MacroSet.h"

static const WCHAR WINDOW_NAME[] = L"YaMang";
const int WM_SOCKET = 104;

class GameManager;

class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		BaseWindow * pThis = NULL;

		if ( uMsg == WM_NCCREATE )
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (BaseWindow*)pCreate->lpCreateParams;
			SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR)pThis );

			pThis->m_HandleOfWindow = hwnd;
		}
		else
		{
			pThis = (BaseWindow*)GetWindowLongPtr( hwnd, GWLP_USERDATA );
		}

		if ( pThis )
		{
			return pThis->HandleMessage( uMsg, wParam, lParam );
		}
		else
		{
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
	}

	BaseWindow(): m_HandleOfWindow( nullptr ), m_HandleOfAccelTable( nullptr ){}

	BOOL Create( PCWSTR lpWindowName,
				 DWORD dwStyle, 
				 LPCWSTR lpTableName,
				 DWORD dwExStyle = 0,
				 int x = CW_USEDEFAULT,
				 int y = CW_USEDEFAULT,
				 int nWidth = BaseWindow::WINDOW_WIDTH,
				 int nHeight = BaseWindow::WINDOW_HEIGHT,
				 HWND hWndParent = 0,
				 HMENU hMenu = 0
				 )
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = BaseWindow::WindowProc;
		wc.hInstance = GetModuleHandle( NULL );
		wc.lpszClassName = ClassName();

		wc.hCursor = LoadCursor( NULL, IDC_ARROW );

		RegisterClass( &wc );

		m_HandleOfWindow = CreateWindowEx( dwExStyle, ClassName(), lpWindowName, dwStyle,
								 x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle( NULL ), this );
		
		m_HandleOfAccelTable = LoadAccelerators( wc.hInstance, lpTableName );

		return ( m_HandleOfWindow ? TRUE : FALSE );
	}

	static const int WINDOW_WIDTH = 640; // 1280;
	static const int WINDOW_HEIGHT = 360; // 720;

protected:
	virtual PCWSTR	ClassName() const = 0;
	virtual LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	HWND	m_HandleOfWindow;
	HACCEL	m_HandleOfAccelTable;
};

class MainWindow: public BaseWindow, public Singleton<MainWindow>
{
public:
	MainWindow();
	virtual ~MainWindow();

	PCWSTR	ClassName() const { return WINDOW_NAME; }
	LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	int		RunGame();

	HWND	Window() const { return m_HandleOfWindow; }
	BOOL	Display( int nCmdShow ) const { return ShowWindow( Window(), nCmdShow ); }
};

