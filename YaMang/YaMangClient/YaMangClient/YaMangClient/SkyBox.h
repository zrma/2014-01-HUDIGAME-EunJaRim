#pragma once
#include "Interface3D.h"

#ifndef D3DFVF_SKYBOXVERTEX
#define D3DFVF_SKYBOXVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

struct SKYBOXVERTEX
{
	SKYBOXVERTEX() {}
	SKYBOXVERTEX( float x, float y, float z, float u, float v )
	{
		m_X = x;
		m_Y = y;
		m_Z = z;
		m_U = u;
		m_V = v;
	}

	float	m_X = 0;
	float	m_Y = 0;
	float	m_Z = 0;
	float	m_U = 0;
	float	m_V = 0;
};

#endif // !D3DFVF_SKYBOXVERTEX

class SkyBox:
	public Interface3D
{
public:
	SkyBox();
	virtual ~SkyBox();

	enum SkyBoxType
	{
		// 순서 중요!
		SKY_BOX_BACK = 0,
		SKY_BOX_FRONT = 1,
		SKY_BOX_LEFT = 2,
		SKY_BOX_RIGHT = 3,
		SKY_BOX_TOP = 4,
		SKY_BOX_BOTTOM = 5
	};

	bool Init( LPCTSTR backSideFileName, LPCTSTR frontSideFileName, LPCTSTR leftSideFileName,
			   LPCTSTR rightSideFileName, LPCTSTR topSideFileName, LPCTSTR bottomSideFileName, UINT size = 1 );
	
	void Destroy();

	virtual void Update() {}
	virtual void Render() const;

private:
	LPDIRECT3DTEXTURE9	m_SkyBoxTextures[6];
	LPD3DXMESH          m_SkyBoxMesh = nullptr;
};