#pragma once

#include "MacroSet.h"

extern const wchar_t* FONT_TYPE;

struct MESHOBJECT;
struct CUSTOMVERTEX;
class QuadTree;
class Frustum;

class Renderer: public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	bool	Init();
	void	Destroy();
	void	RenderBegin();
	void	RenderEnd();

	LPDIRECT3DDEVICE9	GetDevice() { return m_D3dDevice; }

	void	RenderMesh( MESHOBJECT* mesh );
	void	RenderMap( QuadTree* quadTree, Frustum* frustum, CUSTOMVERTEX* heightMap, float ratioOfLOD );
	void	RenderBillboard( int id, char flag );

	void	SetShader( bool mode );

	CUSTOMVERTEX*	GetHeightMap() const;
	void			GetHeightMapSize( DWORD* width, DWORD* height ) const;

	void	ToggleHeightMapWire() { m_IsHeightMapWire = !m_IsHeightMapWire; }

	void	ResizeWindow( LONG width, LONG height );

	void	SetViewMatrix( D3DXMATRIXA16& matrix ) const;
	void	SetWorldMatrix( D3DXMATRIXA16& matrix ) const;
	
	D3DXVECTOR3	GetPickedPointOfCenter() const;
	
	void	WriteText( LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 ) const;

private:
	bool	CreateDevice( HWND hWnd, long width, long height );
	void	DestroyDevice();

	bool	CreateFont();
	void	DestroyFont();

	bool	PreRender();
	void	PostRender();

	bool	m_Result = false;
	bool	m_IsReady = false;

	bool	m_IsHeightMapWire = false;
	
	UINT	m_PrevTime = 0;
	UINT	m_Frame = 0;
	UINT	m_FPS = 0;
	UINT	m_DPCall = 0;
	UINT	m_HeightMapVertis = 0;

	LPDIRECT3D9			m_D3D = nullptr;
	LPDIRECT3DDEVICE9	m_D3dDevice = nullptr;

	float	m_StartWidth = 0.0f;
	float	m_StartHeight = 0.0f;

	ID3DXFont*			m_Font = nullptr;
	ID3DXSprite*		m_Sprite = nullptr;

	UINT	m_FontHeight = 30;
	UINT	m_FontWidth = 0;
	UINT	m_FontMipLevel = 1;

	bool	m_IsEffectReady = false;
};