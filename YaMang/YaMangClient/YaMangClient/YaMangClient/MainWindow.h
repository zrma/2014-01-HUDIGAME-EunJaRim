#pragma once

#include "MacroSet.h"

static const WCHAR WINDOW_NAME[] = L"YaMang";
const int WM_SOCKET = 104;

class GameManager;

// agebreak : BaseWindow가 왜 필요한지 모르겠지만, 나중을 위해서 만들었다면. 다른 파일로 뽑아 낼것
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )	// agebreak: 왜 WinProc 과 Create가 헤더에 구현되어 있나??
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

	BaseWindow(){}

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
		
		// agebreak : 엑셀 테이블은 메뉴의 단축키때 사용하는 것이다. 게임에서는 필요없음. 
		m_HandleOfAccelTable = LoadAccelerators( wc.hInstance, lpTableName );

		return ( m_HandleOfWindow ? TRUE : FALSE );
	}

	static const int WINDOW_WIDTH = 640; // 1280;
	static const int WINDOW_HEIGHT = 360; // 720;

protected:
	virtual PCWSTR	ClassName() const = 0;
	virtual LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	HWND	m_HandleOfWindow = nullptr;	// agebreak : 2013부터는 이렇게 초기화를 해도 된다. (C++0x)
	HACCEL	m_HandleOfAccelTable = nullptr;
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

