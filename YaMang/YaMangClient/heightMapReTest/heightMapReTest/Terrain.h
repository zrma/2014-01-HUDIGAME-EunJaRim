#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <MMSystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include <d3dx9.h>


struct VERTEX
{
	D3DXVECTOR3 Vec;
	DWORD       Color;
	float       tu, tv;
};

struct INDEX
{
	WORD _0, _1, _2;
};


#define     VERTEX_FVF ( D3DFVF_XYZ | D3DFVF_DIFFUSE  | D3DFVF_TEX1 )


class Terrain
{
public:

	Terrain();
	~Terrain();


	int CreateVertexBuffer( IDirect3DDevice9    *m_pkDev, char *szTexFile );
	int CreateIndexBuffer();

	void Render();
	void Shutdown();


private:

	IDirect3DDevice9            *m_pkDevice;
	IDirect3DVertexBuffer9      *m_pkVB;
	IDirect3DIndexBuffer9       *m_pkIndexVB;
	IDirect3DTexture9           *m_pkTexture;

	INDEX                       *pIndice;
	VERTEX                      *pVertex;

	int                         iWidth;
	int                         iHeight;



};