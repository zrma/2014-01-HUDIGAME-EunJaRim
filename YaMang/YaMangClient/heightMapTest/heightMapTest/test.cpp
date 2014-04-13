#include <d3d9.h>
#include <d3dx9.h>

#define WINDOW_W  500//���� �ʺ�
#define WINDOW_H  500//���� ����
#define WINDOW_TITLE L"HeightMap-TList"//���� Ÿ��Ʋ
#define BMP_HEIGHTMAP L"map128.bmp"//���� �̸�
/**-----------------------------------------------------------------------------
*  ��������
*------------------------------------------------------------------------------
*/
HWND     g_hwnd = NULL;//�����ڵ�
LPDIRECT3D9             g_pD3D = NULL; /// D3D ����̽��� ������ D3D��ü����
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; /// �������� ���� D3D����̽�
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; /// ������ ������ ��������
LPDIRECT3DINDEXBUFFER9 g_pIB = NULL; /// �ε����� ������ �ε�������
LPDIRECT3DTEXTURE9  g_pTexHeight = NULL; /// Texture ���̸�
LPDIRECT3DTEXTURE9  g_pTexDiffuse = NULL; /// Texture �����
D3DXMATRIXA16   g_matAni;//���
DWORD     g_cxHeight = 0;//x��
DWORD     g_czHeight = 0;//z��
//xz���
/// ����� ������ ������ ����ü
struct CUSTOMVERTEX
{
	D3DXVECTOR3  p;//����..��ġ
	D3DXVECTOR3  n;//����
	D3DXVECTOR2  t;//�ؽ�ó.
};
/// ����� ���� ����ü�� ���� ������ ��Ÿ���� FVF��
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)
struct MYINDEX
{
	WORD _0, _1, _2;  /// WORD, 16��Ʈ �ε���
};
/**-----------------------------------------------------------------------------
* Direct3D �ʱ�ȭ
*------------------------------------------------------------------------------
*/
HRESULT InitD3D( HWND hWnd )
{
	/// ����̽��� �����ϱ����� D3D��ü ����
	if ( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
		return E_FAIL;
	/// ����̽��� ������ ����ü
	/// ������ ������Ʈ�� �׸����̱⶧����, �̹����� Z���۰� �ʿ��ϴ�.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	/// ����̽� ����
	if ( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice ) ) )
	{
		return E_FAIL;
	}
	/// �⺻�ø�, CCW
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	/// Z���۱���� �Ҵ�.
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	return S_OK;
}

/**-----------------------------------------------------------------------------
* �ؽ�ó �ʱ�ȭ
*------------------------------------------------------------------------------
*/
HRESULT InitTexture( )
{
	/// ���̸� �ؽ�ó
	/// D3DFMT_X8R8G8B8�� D3DPOOL_MANAGED�� �ֱ����ؼ� �� �Լ��� ����ߴ�.
	if ( FAILED( D3DXCreateTextureFromFileEx( g_pd3dDevice, BMP_HEIGHTMAP, //��ġ,������ ������  �����̸�
		D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &g_pTexHeight ) ) )
		return E_FAIL;
	/// �����
	if ( FAILED( D3DXCreateTextureFromFile( g_pd3dDevice, L"tile2.tga", &g_pTexDiffuse ) ) )
		return E_FAIL;//�о����..������ ���� �ؽ�ó..
	return S_OK;
}
/**-----------------------------------------------------------------------------
* �������۸� �����ϰ� �������� ä���ִ´�.
*------------------------------------------------------------------------------
*/
HRESULT InitVB( )
{
	D3DSURFACE_DESC  ddsd;//����.
	D3DLOCKED_RECT  d3drc;//����
	g_pTexHeight->GetLevelDesc( 0, &ddsd ); /// ����� �ؽ�ó�� ����
	g_cxHeight = ddsd.Width;    /// �ؽ�ó�� ����ũ��
	g_czHeight = ddsd.Height;    /// �ؽ�ó�� ����ũ��
	if ( FAILED( g_pd3dDevice->CreateVertexBuffer( ddsd.Width*ddsd.Height*sizeof( CUSTOMVERTEX ),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
	{
		return E_FAIL;
	}//�α���ºκ�..
	/// �ؽ�ó �޸� ��!
	g_pTexHeight->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );
	VOID* pVertices;
	/// �������� ��!
	if ( FAILED( g_pVB->Lock( 0, g_cxHeight*g_czHeight*sizeof( CUSTOMVERTEX ), (void**) &pVertices, 0 ) ) )
		return E_FAIL;//�ʺ��.���̸�ŭ��.��.���ؽ���.�����ϰ�..��.�����͸�.pVertices�� �����Ѵ�.
	CUSTOMVERTEX v;//���ؽ�
	CUSTOMVERTEX* pV = (CUSTOMVERTEX*) pVertices;//�ּҰ����� ����
	for ( DWORD z = 0; z < g_czHeight; z++ )//����
	{
		for ( DWORD x = 0; x < g_cxHeight; x++ )//�ʺ�
		{
			v.p.x = (float) x - g_cxHeight / 2.0f;  /// ������ x��ǥ(�޽ø� ������ ����)..�ʺ񿡼�.�ʺ��� ������ ����.
			v.p.z = -( (float) z - g_czHeight / 2.0f ); /// ������ z��ǥ(�޽ø� ������ ����), z���� ����;����̹Ƿ� -�� ���Ѵ�.
			v.p.y = ( (float) ( *( (LPDWORD) d3drc.pBits + x + z*( d3drc.Pitch / 4 ) ) & 0x000000ff ) ) / 10.0f; /// DWORD�̹Ƿ� pitch/4
			//��ġ
			v.n.x = v.p.x;
			v.n.y = v.p.y;
			v.n.z = v.p.z;//���
			D3DXVec3Normalize( &v.n, &v.n );//������,�����Է�...3d������ ����ȭ�� ���͸� �����ش�.
			v.t.x = (float) x / ( g_cxHeight - 1 );//�ؽ�ó..����.
			v.t.y = (float) z / ( g_czHeight - 1 );
			*pV++ = v;//����.
			//   g_pLog->Log( "[%f,%f,%f]", v.x, v.y, v.z );
		}
	}
	g_pVB->Unlock( );
	g_pTexHeight->UnlockRect( 0 );
	return S_OK;
}
/**-----------------------------------------------------------------------------
* �ε��� ���� �ʱ�ȭ
*------------------------------------------------------------------------------
*/
HRESULT InitIB( )
{
	if ( FAILED( g_pd3dDevice->CreateIndexBuffer( ( g_cxHeight - 1 )*( g_czHeight - 1 ) * 2 * sizeof( MYINDEX ), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_pIB, NULL ) ) )
	{
		return E_FAIL;
	}
	MYINDEX  i;
	MYINDEX* pI;
	if ( FAILED( g_pIB->Lock( 0, ( g_cxHeight - 1 )*( g_czHeight - 1 ) * 2 * sizeof( MYINDEX ), (void**) &pI, 0 ) ) )
		return E_FAIL;
	for ( DWORD z = 0; z < g_czHeight - 1; z++ )
	{
		for ( DWORD x = 0; x < g_cxHeight - 1; x++ )
		{
			i._0 = ( z*g_cxHeight + x );
			i._1 = ( z*g_cxHeight + x + 1 );
			i._2 = ( ( z + 1 )*g_cxHeight + x );
			*pI++ = i;
			i._0 = ( ( z + 1 )*g_cxHeight + x );
			i._1 = ( z*g_cxHeight + x + 1 );
			i._2 = ( ( z + 1 )*g_cxHeight + x + 1 );
			*pI++ = i;
		}
	}
	g_pIB->Unlock( );
	return S_OK;
}

/**-----------------------------------------------------------------------------
* �������� �ʱ�ȭ
*------------------------------------------------------------------------------
*/
HRESULT InitGeometry( )
{
	if ( FAILED( InitTexture( ) ) ) return E_FAIL;
	if ( FAILED( InitVB( ) ) ) return E_FAIL;
	if ( FAILED( InitIB( ) ) ) return E_FAIL;
	return S_OK;
}

/**-----------------------------------------------------------------------------
* ī�޶� ��� ����
*------------------------------------------------------------------------------
*/
void SetupCamera( )
{
	/// ���� ��� ����
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	/// �� ����� ����
	D3DXVECTOR3 vEyePt( 0.0f, 100.0f, -(float) g_czHeight );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	/// �������� ��� ����
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f );
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}
/**-----------------------------------------------------------------------------
* ���� ����
*------------------------------------------------------------------------------
*/
VOID SetupLights( )
{
	/// ����(material)����
	/// ������ ����̽��� �� �ϳ��� ������ �� �ִ�.
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_pd3dDevice->SetMaterial( &mtrl );
	/// ���� ����
	D3DXVECTOR3 vecDir;         /// ���⼺ ����(directional light)�� ���� ���� ����
	D3DLIGHT9 light;         /// ���� ����ü
	ZeroMemory( &light, sizeof( D3DLIGHT9 ) );   /// ����ü�� 0���� �����.
	light.Type = D3DLIGHT_DIRECTIONAL;   /// ������ ����(�� ����,���⼺ ����,����Ʈ����Ʈ)
	light.Diffuse.r = 1.0f;       /// ������ ����� ���
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 0.0f;
	vecDir = D3DXVECTOR3( 1, 1, 1 );     /// ���� ����
	vecDir = D3DXVECTOR3( cosf( GetTickCount( ) / 350.0f ), /// ���� ȸ��
						  1.0f,
						  sinf( GetTickCount( ) / 350.0f ) );
	D3DXVec3Normalize( (D3DXVECTOR3*) &light.Direction, &vecDir ); /// ������ ������ �������ͷ� �����.
	light.Range = 1000.0f;         /// ������ �ٴٸ��� �ִ� �ִ�Ÿ�
	g_pd3dDevice->SetLight( 0, &light );       /// ����̽��� 0�� ���� ��ġ
	g_pd3dDevice->LightEnable( 0, TRUE );       /// 0�� ������ �Ҵ�
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );   /// ���������� �Ҵ�
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00909090 );  /// ȯ�汤��(ambient light)�� �� ����
}
/**-----------------------------------------------------------------------------
* FPS(Frame Per Second)���
*------------------------------------------------------------------------------
*/
void LogFPS( void )
{
	static DWORD nTick = 0;
	static DWORD nFPS = 0;
	if ( GetTickCount( ) - nTick > 1000 )
	{
		nTick = GetTickCount( );
		nFPS = 0;
		return;
	}
	nFPS++;
}
/**-----------------------------------------------------------------------------
* �ִϸ��̼� ����
*------------------------------------------------------------------------------
*/
VOID Animate( )
{
	static DWORD t = 0;
	static bool flag = false;
	/// 0 ~ 2PI ����(0~360��) ���� ��ȭ��Ŵ Fixed Point��� ���
	DWORD d = GetTickCount( ) % ( (int) ( ( D3DX_PI * 2 ) * 1000 ) );
	/// Y�� ȸ�����
	D3DXMatrixRotationY( &g_matAni, d / 1000.0f );
	// D3DXMatrixIdentity( &g_matAni );
	/// ī�޶� ��ļ���
	SetupCamera( );
	SetupLights( );
	if ( d < t )
		flag = !flag;

	//flag�� true��� wire���� �Ѵ�.
	g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, flag ? D3DFILL_WIREFRAME : D3DFILL_SOLID );
	t = d;//d�� ���� �ð��� �����Կ����� �����ϰ� �� ���� t�� �����Ѵ�.
	//1�ʰ� ������ d�� ������ �ִ� ���� ���� ���� ���� ������ �ǰ�. �� �� t�� d�� ���� ū ����
	//������ �ִ� �����̹Ƿ� �̸� �̿��� flag�� �����Ѵ�. �� ���� �ٽ� ���� �������̵ȴ�.
	LogFPS( );
}

/**-----------------------------------------------------------------------------
* �ʱ�ȭ ��ü�� �Ұ�
*------------------------------------------------------------------------------
*/
VOID Cleanup( )
{
	if ( g_pTexHeight != NULL )
		g_pTexHeight->Release( );
	if ( g_pTexDiffuse != NULL )
		g_pTexDiffuse->Release( );
	if ( g_pIB != NULL )
		g_pIB->Release( );
	if ( g_pVB != NULL )
		g_pVB->Release( );
	if ( g_pd3dDevice != NULL )
		g_pd3dDevice->Release( );
	if ( g_pD3D != NULL )
		g_pD3D->Release( );
}

/**-----------------------------------------------------------------------------
* �޽� �׸���
*------------------------------------------------------------------------------
*/
void DrawMesh( D3DXMATRIXA16* pMat )
{
	g_pd3dDevice->SetTransform( D3DTS_WORLD, pMat );
	g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof( CUSTOMVERTEX ) );
	g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	g_pd3dDevice->SetIndices( g_pIB );
	g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_cxHeight*g_czHeight, 0, ( g_cxHeight - 1 )*( g_czHeight - 1 ) * 2 );
}
/**-----------------------------------------------------------------------------
* ȭ�� �׸���
*------------------------------------------------------------------------------
*/
VOID Render( )
{
	/// �ĸ���ۿ� Z���� �ʱ�ȭ
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 255, 255, 255 ), 1.0f, 0 );
	/// �ִϸ��̼� ��ļ���
	Animate( );
	/// ������ ����
	if ( SUCCEEDED( g_pd3dDevice->BeginScene( ) ) )
	{
		g_pd3dDevice->SetTexture( 0, g_pTexDiffuse );       /// 0�� �ؽ��� ���������� �ؽ��� ����(�����)
		g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR ); /// 0�� �ؽ�ó ���������� Ȯ�� ����
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );  /// 0�� �ؽ�ó : 0�� �ؽ�ó �ε��� ���
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		DrawMesh( &g_matAni );
		/// ������ ����
		g_pd3dDevice->EndScene( );
	}
	/// �ĸ���۸� ���̴� ȭ������!
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}


/**-----------------------------------------------------------------------------
* ������ ���ν���
*------------------------------------------------------------------------------
*/
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_DESTROY:
			Cleanup( );
			PostQuitMessage( 0 );
			return 0;
		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_ESCAPE:
					PostMessage( hWnd, WM_DESTROY, 0, 0L );
					break;
			}
			break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}


/**-----------------------------------------------------------------------------
* ���α׷� ������
*------------------------------------------------------------------------------
*/
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	/// ������ Ŭ���� ���
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
		L"BasicFrame", NULL };
	RegisterClassEx( &wc );
	/// ������ ����
	HWND hWnd = CreateWindow( L"BasicFrame", WINDOW_TITLE,
							  WS_OVERLAPPEDWINDOW, 100, 100, WINDOW_W, WINDOW_H,
							  GetDesktopWindow( ), NULL, wc.hInstance, NULL );
	g_hwnd = hWnd;
	/// Direct3D �ʱ�ȭ
	if ( SUCCEEDED( InitD3D( hWnd ) ) )
	{
		if ( SUCCEEDED( InitGeometry( ) ) )
		{
			/// ������ ���
			ShowWindow( hWnd, SW_SHOWDEFAULT );
			UpdateWindow( hWnd );
			/// �޽��� ����
			MSG msg;
			ZeroMemory( &msg, sizeof( msg ) );
			while ( msg.message != WM_QUIT )
			{
				/// �޽���ť�� �޽����� ������ �޽��� ó��
				if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
				else
					/// ó���� �޽����� ������ Render()�Լ� ȣ��
					Render( );
			}
		}
	}
	/// ��ϵ� Ŭ���� �Ұ�
	UnregisterClass( L"D3D Tutorial", wc.hInstance );
	return 0;
}