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

			pThis->m_hwnd = hwnd;
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

	BaseWindow(): m_hwnd( nullptr ), m_hAccelTable( nullptr ){}

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

		m_hwnd = CreateWindowEx( dwExStyle, ClassName(), lpWindowName, dwStyle,
								 x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle( NULL ), this );
		
		m_hAccelTable = LoadAccelerators( wc.hInstance, lpTableName );

		return ( m_hwnd ? TRUE : FALSE );
	}

	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

protected:
	virtual PCWSTR	ClassName() const = 0;
	virtual LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	HWND	m_hwnd;
	HACCEL	m_hAccelTable;
};

class MainWindow: public BaseWindow, public Singleton<MainWindow>
{
public:
	MainWindow();
	virtual ~MainWindow();

	PCWSTR	ClassName() const { return WINDOW_NAME; }
	LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	int		RunGame();

	HWND	Window() const { return m_hwnd; }
	BOOL	Display( int nCmdShow ) const { return ShowWindow( Window(), nCmdShow ); }

private:
	MainWindow( const MainWindow & );
	MainWindow & operator=( const MainWindow & );

	GameManager* m_GameManager;
};

