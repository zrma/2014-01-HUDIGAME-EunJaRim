#pragma once


#define WINDOW_WIDTH	1024
#define WINDOW_HEIGHT	768

#define MAX_STRING		128

#define BUF_SIZE		32
#define SERVER_PORT_NUM	9000


template <class DERIVED_TYPE>
class BaseWindow
{
public:
	
	static LRESULT CALLBACK WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		DERIVED_TYPE *pThis = NULL;

		// 초기 생성 시
		if ( uMsg == WM_NCCREATE )
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*) lParam;

			pThis = (DERIVED_TYPE*) pCreate->lpCreateParams;

			SetWindowLongPtr( hwnd, GWLP_USERDATA, (LONG_PTR) pThis );
			// 일단 윈도우와 관련 된 정보 저장해두기
		
			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*) GetWindowLongPtr( hwnd, GWLP_USERDATA );
			// 가져와 본다
		}
		if ( pThis )
		{
			// 창이 떠 있으면 메시지 핸들
			return pThis->HandleMessage( uMsg, wParam, lParam );
		}
		else
		{
			// 창이 안 떠 있으면 DefWindowProc()에 메시지 핸들링을 넘김
			return DefWindowProc( hwnd, uMsg, wParam, lParam );
		}
	}

	BaseWindow(): m_hwnd( NULL ) {}

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = WINDOW_WIDTH,
		int nHeight = WINDOW_HEIGHT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
		)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle( NULL );
		wc.lpszClassName = ClassName();

		wc.hCursor = LoadCursor( NULL, IDC_ARROW );
		// 요거 추가해둠

		RegisterClass( &wc );

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle( NULL ), this
			);

		
		return ( m_hwnd ? TRUE : FALSE );
	}

	HWND Window() const { return m_hwnd; }

protected:

	// 순수 가상 함수 설정 해 둠
	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;

	HWND m_hwnd;
};


class MainWindow: public BaseWindow<MainWindow>
	// 템플릿으로 구현 되어 있는 BaseWindow를 상속 받아서 MainWindow 구현
{
//	Scene   m_scene;
// 렌더러가 붙어야 될 부분임

public:
	// 아래 두 개는 순수 가상 함수였으므로 반드시 구현 해야 됨
	PCWSTR  ClassName() const { return L"P2P Demo"; }
	LRESULT HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
};