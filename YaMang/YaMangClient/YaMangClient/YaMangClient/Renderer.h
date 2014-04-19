#pragma once

#include "MacroSet.h"
#include <d3dx9math.h>

struct MESHOBJECT;

class Renderer: public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	bool	Init();
	void	Destroy();
	void	RenderBegin();
	void	RenderEnd();

	void	RenderMesh( MESHOBJECT* mesh );
	void	RenderMap();
	void	ResizeWindow( LONG width, LONG height );

	void	SetCamera( D3DXMATRIXA16& matrix );
	
	D3DVIEWPORT9	GetViewPort();
	D3DXMATRIXA16	GetProjMatrix();
	D3DXMATRIXA16	GetViewMatrix();

	void	CreateTextRenderer( LPCWSTR fontType = L"맑은 고딕", int fontHeight = 20, int fontWidth = 0 );
	void	WriteText( LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 );
	void	DeleteTextRenderer();

private:
	bool	m_Result = false;
	bool	m_IsReady = false;
	
	UINT	m_PrevTime = 0;
	UINT	m_Frame = 0;
	UINT	m_FPS = 0;
};