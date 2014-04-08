#include <d3d9.h>
#include <d3dx9.h>

#define WINDOW_W 500
#define WINDOW_H 500
#define WINDOW_TITLE L"HightMap-TList"
#define BMP_HEIGHTMAP L"heightmap.bmp"

HWND g_hwnd = NULL;

LPDIRECT3D9 g_pD3D = nullptr;
LPDIRECT3DDEVICE9 g_pD3DDevice = nullptr;

LPDIRECT3DVERTEXBUFFER9 g_pVB = nullptr;
LPDIRECT3DINDEXBUFFER9 g_pIB = nullptr;

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
	WORD _0, _1, _2;
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

	if ( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice ) ) )
	{
		return E_FAIL;
	}

	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	g_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	return S_OK;
}


HRESULT InitTexture()
{
	if ( FAILED( D3DXCreateTextureFromFileEx( g_pD3DDevice, BMP_HEIGHTMAP, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_X8B8G8R8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_pTexHeight) ) )
	{
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFile(g_pD3DDevice,L"gom.tga", &g_pTexDiffuse)))
	{
		return E_FAIL;
	}
}


HRESULT InitVB()
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT d3drc;

	g_pTexHeight->GetLevelDesc( 0, &ddsd );
	g_cxHeight = ddsd.Width;
	g_czHeight = ddsd.Height;

	if (FAILED(g_pD3DDevice->CreateVertexBuffer(ddsd.Width*ddsd.Height*sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	g_pTexHeight->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );

	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, g_cxHeight*g_czHeight*sizeof(CUSTOMVERTEX),(void**)&pVertices, 0)))
	{
		return E_FAIL;
	}

	CUSTOMVERTEX v;
	CUSTOMVERTEX* pV = (CUSTOMVERTEX*) pVertices;

	for ( DWORD z = 0; z < g_czHeight; ++z )
	{
		for ( DWORD x = 0; x < g_cxHeight; ++x )
		{
			v.p.x = (float) x - g_cxHeight / 2.0f;
			v.p.z = -( (float) z - g_czHeight / 2.0f );
			v.p.y = ( (float) ( *( (LPDWORD) d3drc.pBits + x + z*( d3drc.Pitch / 4 ) ) & 0x000000ff ) ) / 10.f;

			//normal 값이고
			//0,0,0 기준으로 각 지점의 normal 값을 계산
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;

			//단위 벡터로 만드는 것
			//정규화 벡터로 변경하는 연산
			D3DXVec3Normalize( &v.n, &v.n );

			v.t.x = (float) x / ( g_cxHeight - 1 );
			v.t.y = (float) z / ( g_czHeight - 1 );
			*pV++ = v;
		}
	}
	g_pVB->Unlock();

	g_pTexHeight->UnlockRect( 0 );
}

HRESULT InitIB()
{
	if ( FAILED( g_pD3DDevice->CreateIndexBuffer( ( g_cxHeight - 1 )*( g_czHeight - 1 ) * 2 * sizeof( MYINDEX ), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL ) ) )
	{
		return E_FAIL;
	}

	MYINDEX i;
	MYINDEX* pI;

	if (FAILED(g_pIB->Lock(0, (g_cxHeight-1)*(g_czHeight-1)*2*sizeof(MYINDEX), (void**)&pI,0) ))
	{
		return E_FAIL;
	}

	for ( DWORD z = 0; z < g_czHeight - 1; ++z )
	{
		for ( DWORD x = 0; x < g_cxHeight - 1; ++x )
		{
			i._0 = ( z*g_cxHeight + x );
			i._1 = ( z*g_cxHeight + x + 1 );
			i._2 = ( ( z + 1 )*g_cxHeight + x );
			*pI++ = i;
			i._0 = ( ( z + 1 )*g_cxHeight + x );
			i._1 = ( z*g_cxHeight + x + 1 );
			i._2 = ( ( z + 1 ) *g_cxHeight + x + 1 );
			*pI++ = i;
		}
	}
	g_pIB->Unlock();

	return S_OK;
}

HRESULT InitGeometry()
{
	if (FAILED(InitTexture()) )
	{
		return E_FAIL;
	}

	if ( FAILED( InitVB() ) )
	{
		return E_FAIL;
	}

	if (FAILED(InitIB()) )
	{
		return E_FAIL;
	}

	return S_OK;
}


void SetupCamera()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3 vEyePt( 0.f, 100.f, -(float) g_czHeight );
	D3DXVECTOR3 vLookatPt( 0.f, 0.f, 0.f );
	D3DXVECTOR3 vUpVec( 0.f, 1.f, 0.f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.f, 1.f, 1000.f );
	g_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


void SetupLights()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );

	
}