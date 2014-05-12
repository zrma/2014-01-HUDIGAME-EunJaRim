#include <d3dx9.h>

struct CUSTOMVERTEX
{
	D3DXVECTOR3	m_VertexPoint = { 0, 0, 0 };
	DWORD		m_Diffuse = 0;
	D3DXVECTOR2	m_VertexTexturePoint0 = { 0, 0 };
	D3DXVECTOR2	m_VertexTexturePoint1 = { 0, 0 };
	D3DXVECTOR2	m_VertexTexturePoint2 = { 0, 0 };
};

//마지막에는 각 texturepoint의 차원과 index를 의미
#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX3| D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE2(1) | D3DFVF_TEXCOORDSIZE2(2) )

struct SKYBOXVERTEX
{
	SKYBOXVERTEX() {}
	SKYBOXVERTEX( float x, float y, float z,
					float nx, float ny, float nz,
					float u, float v )
	{
		m_X = x;
		m_Y = y;
		m_Z = z;
		m_NormalX = nx;
		m_NormalY = ny;
		m_NormalZ = nz;
		m_U = u;
		m_V = v;
	}

	float	m_X = 0;
	float	m_Y = 0;
	float	m_Z = 0;
	float	m_NormalX = 0;
	float	m_NormalY = 0;
	float	m_NormalZ = 0;
	float	m_U = 0;
	float	m_V = 0;
};
#define D3DFVF_SKYBOXVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct MYINDEX
{
	UINT _0, _1, _2;
};


enum AreaModeType
{
	AREA_MODE_NONE,

	AREA_MODE_COLOR,
	AREA_MODE_HIGHER,
	AREA_MODE_LOWER,

	AREA_MODE_MAX
};

#define MAP_TEXTURE_COUNT 5

//////////////////////////////////////////////////////////////////////////
// Global val 선언
//////////////////////////////////////////////////////////////////////////
// 부모 Process 공용 자원
extern LPDIRECT3D9 g_D3D;
extern LPDIRECT3DDEVICE9 g_D3dDevice;

//////////////////////////////////////////////////////////////////////////
// Height Map 생성용 공용 자원
//////////////////////////////////////////////////////////////////////////
extern LPD3DXMESH g_Mesh;

//클라이언트용 textureload함수
extern LPDIRECT3DTEXTURE9 g_MapHeightTexture;
extern LPDIRECT3DTEXTURE9 g_MapTexture;

//맵툴용 Texture array
extern LPDIRECT3DTEXTURE9 g_MapTextureArray[MAP_TEXTURE_COUNT];

extern DWORD g_XHeight;
extern DWORD g_ZHeight;

//////////////////////////////////////////////////////////////////////////
// 텍스트 출력에 필요한 자원
//////////////////////////////////////////////////////////////////////////
extern ID3DXFont*	g_Font;		// 글자를 그릴 폰트 스타일 객체
extern ID3DXSprite*	g_Sprite;		// 폰트를 그릴 스프라이트 객체 

//////////////////////////////////////////////////////////////////////////
// D3D cursor Set에 필요한 자원
//////////////////////////////////////////////////////////////////////////
// extern IDirect3DSurface9* g_surfcursor; //테스트 중
// extern IDirect3DTexture9* g_cursortex; //테스트 중

extern LPDIRECT3DTEXTURE9*	g_CursorTex;
extern LPD3DXSPRITE			g_CursorSprite;
extern INT					g_CursorType;
extern INT					g_CursorMaxSize;
extern D3DXVECTOR3			g_CursorPos;

//////////////////////////////////////////////////////////////////////////
// tool Camera 관련 벡터
//////////////////////////////////////////////////////////////////////////
extern D3DXVECTOR3		g_EyePoint;
extern D3DXVECTOR3		g_LookAtPoint;
extern D3DXVECTOR3		g_UpVector;

//////////////////////////////////////////////////////////////////////////
// peeking 관련 변수
//////////////////////////////////////////////////////////////////////////
extern D3DXVECTOR3		g_RayOrigin;
extern D3DXVECTOR3		g_RayDirection;

//////////////////////////////////////////////////////////////////////////
// 화면 종횡비
//////////////////////////////////////////////////////////////////////////
extern float			g_Ratio;
extern float			g_Width;

//////////////////////////////////////////////////////////////////////////
// SKYBOX
//////////////////////////////////////////////////////////////////////////
extern LPDIRECT3DTEXTURE9	g_SkyBoxTextures[6];
extern LPD3DXMESH			g_SkyBoxMesh;