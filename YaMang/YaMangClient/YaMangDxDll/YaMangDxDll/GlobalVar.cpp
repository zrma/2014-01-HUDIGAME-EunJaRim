#include "stdafx.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// 공용 변수 초기화 영역
//////////////////////////////////////////////////////////////////////////
//부모 Process 공용 자원
LPDIRECT3D9 g_D3D = nullptr;
LPDIRECT3DDEVICE9 g_D3dDevice = nullptr;

//Height Map 생성용 공용 자원
LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = nullptr;
LPDIRECT3DINDEXBUFFER9 g_IdxBuffer = nullptr;

LPDIRECT3DTEXTURE9 g_TexHeight = nullptr;
LPDIRECT3DTEXTURE9 g_TexDiffuse = nullptr;

DWORD g_XHeight = 0;
DWORD g_ZHeight = 0;

//텍스트 출력에 필요한 자원
ID3DXFont*		g_Font = nullptr;		// 글자를 그릴 폰트 스타일 객체
ID3DXSprite*	g_Sprite = nullptr;		// 폰트를 그릴 스프라이트 객체 

///D3D cursor Set에 필요한 자원
//IDirect3DSurface9* g_surfcursor = nullptr; //테스트 중
//IDirect3DTexture9* g_cursortex = nullptr; //테스트 중
LPDIRECT3DTEXTURE9 g_cursorTex = 0;
LPD3DXSPRITE g_cursorSprite = 0;
D3DXVECTOR3 g_cursorPos;

/// tool Camera 관련 벡터
D3DXVECTOR3		g_EyePoint = { 0, 0, 0 };
D3DXVECTOR3		g_LookAtPoint = { 0, 0, 1.0f };
D3DXVECTOR3		g_UpVector = { 0, 1, 0 };