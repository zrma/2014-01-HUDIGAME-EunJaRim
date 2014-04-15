#pragma once

#include "MacroSet.h"

static const WCHAR WINDOW_NAME[] = L"YaMang";
const int WM_SOCKET = 104;

class GameManager;

class MainWindow: public Singleton<MainWindow>
{
public:
	MainWindow();
	virtual ~MainWindow();

	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	PCWSTR	ClassName() const { return WINDOW_NAME; }
	LRESULT	HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );

	int		RunGame();

	HWND	Window() const { return m_HandleOfWindow; }
	BOOL	Display( int nCmdShow ) const { return ShowWindow( Window(), nCmdShow ); }

	BOOL	Create( PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
					int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, 
					int nWidth = MainWindow::WINDOW_WIDTH, int nHeight = MainWindow::WINDOW_HEIGHT,
					HWND hWndParent = 0, HMENU hMenu = 0 );
private:
	static const int WINDOW_WIDTH = 640; // 1280;
	static const int WINDOW_HEIGHT = 360; // 720;

	HWND	m_HandleOfWindow = nullptr;
	HACCEL	m_HandleOfAccelTable = nullptr;
};

