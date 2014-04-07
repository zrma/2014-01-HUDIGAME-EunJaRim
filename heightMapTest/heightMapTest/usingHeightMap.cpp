#include <d3d9.h>
#include <d3dx9.h>

#define WINDOW_W 500
#define WINDOW_H 500

LPDIRECT3D9 g_pD3D = nullptr;

LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;

LPDIRECT3DVERTEXBUFFER9 g_VertexBuff = nullptr;
LPDIRECT3DINDEXBUFFER9 g_Indexbuff = nullptr;

LPDIRECT3DTEXTURE9 g_pTexHeight = nullptr;
LPDIRECT3DTEXTURE9 g_pTexDiffuse = nullptr;
D3DXMATRIXA16 g_matAni;

DWORD g_cxHeight = 0;
DWORD g_czHeight = 0;

struct CUSTOMVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;
};

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct MYINDEX 
{
	WORD _0, _1, _2; //이부분 이해가 잘 안되는데.. 확인 필요!
};

HRESULT InitD3D( HWND hWnd )
{
	if ( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	return S_OK;
}


HRESULT InitTexture()
{
	if ( FAILED( D3DXCreateTextureFromFileEx( g_pd3dDevice, L"test.bmp", D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_X8B8G8R8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_pTexHeight ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}


HRESULT InitVertexBuff()
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT d3drc;

	g_pTexHeight->GetLevelDesc( 0, &ddsd );
	g_cxHeight = ddsd.Width;
	g_czHeight = ddsd.Height;
	
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(ddsd.Width*ddsd.Height*sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_VertexBuff, NULL)))
	{
		return E_FAIL;
	}

	g_pTexHeight->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );
	VOID* pVertices;

	if (FAILED(g_VertexBuff->Lock(0, g_cxHeight*g_czHeight*sizeof(CUSTOMVERTEX), (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	CUSTOMVERTEX v;
	CUSTOMVERTEX* pV = (CUSTOMVERTEX*) pVertices;

	for ( DWORD z = 0; z < g_cxHeight; ++z )
	{
		for ( DWORD x = 0; x < g_czHeight; ++x )
		{
			v.p.x = (float) x - g_cxHeight / 2.f;
			v.p.z = -( (float) z - g_czHeight / 2.f );
			v.p.y = ( (float) ( *( (LPDWORD) d3drc.pBits + x + z*( d3drc.Pitch / 4 ) ) & 0x000000ff ) ) / 10.f;

			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;

			D3DXVec3Normalize( &v.n, &v.n );
			v.t.x = (float) x / ( g_cxHeight - 1 );
			v.t.y = (float) z / ( g_czHeight - 1 );

			*pV++ = v;
		}
	}
	g_VertexBuff->Unlock();

	g_pTexHeight->UnlockRect( 0 );

	return S_OK;
}

HRESULT InitIndexBuff()
{
	if ( FAILED( g_pd3dDevice->CreateIndexBuffer( ( g_cxHeight - 1 )*( g_czHeight - 1 ) * 2 * sizeof( MYINDEX ), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_Indexbuff, NULL ) ) )
	{
		return E_FAIL;
	}

	MYINDEX i;
	MYINDEX* pI;

	if (FAILED(g_Indexbuff->Lock(0, (g_cxHeight-1)*(g_czHeight-1)*2*sizeof(MYINDEX), (void**)&pI, 0)))
	{
		return E_FAIL;
	}

	for ( DWORD z = 0; z < g_czHeight - 1; ++z )
	{
		for ( DWORD x = 0; x < g_cxHeight; ++x )
		{
			i._0 = ( z*g_cxHeight + x );
			i._1 = ( z*g_cxHeight + x + 1 );
			i._2 = ( ( z + 1 )*g_cxHeight + x );
			*pI++ = i;
			i._0 = ( (z+1)*g_cxHeight + x );
			i._1 = ( z*g_cxHeight + x + 1 );
			i._2 = ( ( z + 1 )*g_cxHeight + x + 1 );
			*pI++ = i;
		}
	}
	g_Indexbuff->Unlock();

	return S_OK;
}


HRESULT InitGeometry()
{
	if ( FAILED( InitTexture() ) )
	{
		return E_FAIL;
	}
	if (FAILED(InitVertexBuff()))
	{
		return E_FAIL;
	}
	if (FAILED(InitIndexBuff()))
	{
		return E_FAIL;
	}

	return S_OK;
}

void SetupCamera()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3 vEyePt( 0.f, 100.f, -(float) g_czHeight );
	D3DXVECTOR3 vLookatPt( 0.f, 0.f, 0.f );
	D3DXVECTOR3 vUpVec( 0.f, 1.f, 0.f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.f, 1.f, 1000.f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

void SetupLight()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );

	mtrl.Diffuse.r = mtrl.Ambient.r = 1.f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.f;
	g_pd3dDevice->SetMaterial( &mtrl );

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
	light.Type = D3DLIGHT_DIRECTIONAL;

	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;

	vecDir = D3DXVECTOR3( 1, 1, 1 );
	vecDir = D3DXVECTOR3( cosf( GetTickCount() / 350.f ), 1.f, sinf( GetTickCount() / 350.f ));

	D3DXVec3Normalize( (D3DXVECTOR3*) &light.Direction, &vecDir );
	light.Range = 1000.f;

	g_pd3dDevice->SetLight( 0, &light );
	g_pd3dDevice->LightEnable( 0, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00909090 );
}

void Animate()
{

	SetupCamera();
	SetupLight();
}

void CleanUp()
{
	if ( g_pTexHeight != NULL )
	{
		g_pTexHeight->Release();
	}
	
	if ( g_pTexDiffuse != NULL )
	{
		g_pTexDiffuse->Release();
	}

	if ( g_Indexbuff != NULL )
	{
		g_Indexbuff->Release();
	}

	if ( g_VertexBuff != NULL )
	{
		g_VertexBuff->Release();
	}

	if ( g_pd3dDevice != NULL )
	{
		g_pd3dDevice->Release();
	}

	if ( g_pD3D != NULL )
	{
		g_pD3D->Release();
	}
}

void DrawMesh( )
{
	//g_pd3dDevice->SetTransform( D3DTS_WORLD, pMat );
	g_pd3dDevice->SetStreamSource( 0, g_VertexBuff, 0, sizeof( CUSTOMVERTEX ) );
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	g_pd3dDevice->SetIndices( g_Indexbuff );
	g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_cxHeight*g_czHeight, 0, ( g_cxHeight - 1 )*( g_czHeight - 1 ) * 2 );
}

void Render()
{
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 255, 255, 255 ), 1.f, 0 );


	Animate();

	if ( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
	{
		DrawMesh();

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg)
	{
		case WM_DESTROY:
			CleanUp();
			PostQuitMessage( 0 );
			return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle( NULL ), NULL, NULL, NULL, NULL, L"BASIC", NULL };

	RegisterClassEx( &wc );

	HWND hWnd = CreateWindow( L"BASIC", L"TEST", WS_OVERLAPPEDWINDOW, 100, 100, WINDOW_W, WINDOW_H, GetDesktopWindow(), NULL, wc.hInstance, NULL );

	/*g_hwnd = hWnd;*/

	if ( SUCCEEDED( InitGeometry() ) )
	{
		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );

		MSG msg;
		ZeroMemory( &msg, sizeof( msg ) );
		while ( msg.message != WM_QUIT )
		{
			if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				Render();
			}
		}
	}
	UnregisterClass( L"D3D Tutorial", wc.hInstance );
	return 0;
}