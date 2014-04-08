//------------------------------------------------------------------------------------------------------------------------
// winmm.lib    dxguid.lib  d3dx9.lib   d3d9.lib
//------------------------------------------------------------------------------------------------------------------------

// #pragma comment( lib, "winmm" )
// #pragma comment( lib, "dxguid" )
// #pragma comment( lib, "d3dx9" )
// #pragma comment( lib, "d3d9" )


#include "Terrain.h"

//#define               _FULLMODE
#ifdef                  _FULLMODE
bool                    gFullScreen = true;
#else
bool                    gFullScreen = false;
#endif


#define     WIDTH_RES           1024
#define     HEIGHT_RES          768

#define     Random(num)         (int)(((long)rand() * (num)) / (RAND_MAX+1))
#define     Randmize()          srand((unsigned)time(NULL))
#define     RandomA( s, e )     ( (s) + (Random( (e) - (s + 1)) )

HWND                    hWnd = NULL;
LPDIRECT3D9             g_pD3D = NULL;
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL;

bool                    gGameOver = NULL;
TCHAR                   *pProjectName = _T( "3DSample" );
BYTE                    m_bKey[256] = { 0, };
WORD                    MouseX, MouseY;


Terrain                 Map;

HRESULT InitD3D( HWND hWnd )
{
	D3DDISPLAYMODE d3ddm;

	if ( !( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return E_FAIL;
	}

	if ( FAILED( g_pD3D->GetAdapterDisplayMode(
		D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

#ifdef          _FULLMODE
	// Full Screen
	d3dpp.BackBufferWidth = WIDTH_RES;
	d3dpp.BackBufferHeight = HEIGHT_RES;
	d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = !gFullScreen;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

#else
	//  Windows Mode
	d3dpp.BackBufferWidth = WIDTH_RES;
	d3dpp.BackBufferHeight = HEIGHT_RES;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = !gFullScreen;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif

	if ( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, true );

	g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE );

	return S_OK;


}


HRESULT InitData()
{

	//------------------------------------------------------------------------------------------------------------------------
	// Init Data
	//------------------------------------------------------------------------------------------------------------------------

	char *FileNmae = "terrain.bmp";

	Map.CreateVertexBuffer( g_pd3dDevice, FileNmae );
	Map.CreateIndexBuffer();

	return S_OK;
}

void Cleanup()
{
	if ( g_pd3dDevice != NULL ) { g_pd3dDevice->Release(); }
	if ( g_pD3D != NULL ) { g_pD3D->Release(); }
}

void FrameMove()
{
	//------------------------------------------------------------------------------------------------------------------------
	// Data Update
	//------------------------------------------------------------------------------------------------------------------------
}


void SetupMatrices()
{
	//------------------------------------------------------------------------------------------------------------------------
	// World, View, Project Setting
	//------------------------------------------------------------------------------------------------------------------------

	D3DXMATRIXA16 matWorld;


	UINT iTime = timeGetTime() % 10000;
	FLOAT fAngle = iTime * ( 2.0f * D3DX_PI ) / 10000.0f;
	fAngle = 0.f;
	D3DXMatrixRotationY( &matWorld, fAngle );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );


	D3DXVECTOR3 vEyePt( 0.0f, 60.0f, -80.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );


	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


HRESULT Render()
{

	m_bKey['Z'] ? g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME ) :
		g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );


	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0x80, 0x80, 0x80 ), 1.0f, 0 );

	g_pd3dDevice->BeginScene();

	SetupMatrices();

	//------------------------------------------------------------------------------------------------------------------------
	//
	//
	//      Draw Render
	//
	//
	//------------------------------------------------------------------------------------------------------------------------

	Map.Render();

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	return true;

}

LRESULT WINAPI WindowProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM IParam )
{

	switch ( msg )
	{
		case WM_MOUSEMOVE:
		{
							 MouseX = LOWORD( IParam );
							 MouseY = HIWORD( IParam );
		}
			break;
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_ESCAPE:
					gGameOver = true;
					break;
			}
			m_bKey[wParam] = 1;
			break;

		case WM_KEYUP:
			m_bKey[wParam] = 0;
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
	}

	return DefWindowProc( hWnd, msg, wParam, IParam );
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	WNDCLASS    wc;
	MSG         msg;


	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC) WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = pProjectName;

	RegisterClass( &wc );

	if ( !gFullScreen )                                  // WS_CAPTION  타이틀바 / WS_THICKFRAME 사이즈컨트롤 / WS_MAXIMIZEBOX 최대화 / WS_MINIMIZEBOX 최소화
	{
		//  Windows Mode
		hWnd = CreateWindowEx( 0, pProjectName, pProjectName, WS_CAPTION | WS_POPUP | WS_SYSMENU,
							   ( GetSystemMetrics( SM_CXSCREEN ) / 2 ) - ( WIDTH_RES / 2 ),
							   ( GetSystemMetrics( SM_CYSCREEN ) / 2 ) - ( HEIGHT_RES / 2 ),
							   WIDTH_RES, HEIGHT_RES, NULL, NULL, hInst, NULL );
	}
	else
	{   //  FullScreen
		hWnd = CreateWindowEx( 0, pProjectName, pProjectName, WS_POPUP, 0, 0,
							   WIDTH_RES, HEIGHT_RES, NULL, NULL, hInst, NULL );
	}

	if ( hWnd == NULL ) { return false; }
	if ( !SUCCEEDED( InitD3D( hWnd ) ) ) { return false; }
	if ( !SUCCEEDED( InitData() ) ) { return false; }

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );
	SetFocus( hWnd );
	ShowCursor( true );


	while ( !gGameOver )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( !GetMessage( &msg, NULL, 0, 0 ) )
			{
				return msg.wParam;
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		else
		{   // FrameMove();
			Render();
		}
	}
	Cleanup();
	UnregisterClass( pProjectName, wc.hInstance );
	return true;
}