#include <d3dx9.h>


//vertex, index 정의
struct CUSTOMVERTEX
{
	D3DXVECTOR3 vertexPoint;
	DWORD Diffuse;
	D3DXVECTOR2 vertexTexturePoint;
};

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct MYINDEX
{
	UINT _0, _1, _2;
};


//////////////////////////////////////////////////////////////////////////
//Global val 선언
//////////////////////////////////////////////////////////////////////////
//부모 Process 공용 자원
extern LPDIRECT3D9 g_D3D;
extern LPDIRECT3DDEVICE9 g_D3dDevice;

//////////////////////////////////////////////////////////////////////////
//Height Map 생성용 공용 자원
//////////////////////////////////////////////////////////////////////////
extern LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer;
extern LPDIRECT3DINDEXBUFFER9 g_IdxBuffer;

extern LPDIRECT3DTEXTURE9 g_TexHeight;
extern LPDIRECT3DTEXTURE9 g_TexDiffuse;

extern DWORD g_XHeight;
extern DWORD g_ZHeight;

//////////////////////////////////////////////////////////////////////////
//텍스트 출력에 필요한 자원
//////////////////////////////////////////////////////////////////////////
extern ID3DXFont*	g_Font;		// 글자를 그릴 폰트 스타일 객체
extern ID3DXSprite*	g_Sprite;		// 폰트를 그릴 스프라이트 객체 

//////////////////////////////////////////////////////////////////////////
///D3D cursor Set에 필요한 자원
//////////////////////////////////////////////////////////////////////////
//IDirect3DSurface9* g_surfcursor = nullptr; //테스트 중
//IDirect3DTexture9* g_cursortex = nullptr; //테스트 중

extern LPDIRECT3DTEXTURE9	g_cursorTex;
extern LPD3DXSPRITE			g_cursorSprite;
extern D3DXVECTOR3			g_cursorPos;


//////////////////////////////////////////////////////////////////////////
/// tool Camera 관련 벡터
//////////////////////////////////////////////////////////////////////////
extern D3DXVECTOR3		g_EyePoint;
extern D3DXVECTOR3		g_LookAtPoint;
extern D3DXVECTOR3		g_UpVector;