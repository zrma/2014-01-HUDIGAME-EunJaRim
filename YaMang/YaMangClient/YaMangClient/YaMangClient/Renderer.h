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

	void	RenderMesh( MESHOBJECT* mesh ) const;
	void	RenderMap() const;
	void	ResizeWindow( LONG width, LONG height );

	void	SetViewMatrix( D3DXMATRIXA16& matrix ) const;
	void	SetWorldMatrix( D3DXMATRIXA16& matrix ) const;
	
	D3DXVECTOR3	GetPickedPointOfCenter() const;

	void	WriteText( LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 ) const;

private:
	bool	m_Result = false;
	bool	m_IsReady = false;
	
	UINT	m_PrevTime = 0;
	UINT	m_Frame = 0;
	UINT	m_FPS = 0;
};