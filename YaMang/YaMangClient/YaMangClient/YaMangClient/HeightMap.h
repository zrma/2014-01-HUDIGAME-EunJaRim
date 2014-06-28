#pragma once
#include "Interface3D.h"

#define MAP_TEXTURE_COUNT 5

#ifndef D3DFVF_TERRAIN_VERTEX
#define D3DFVF_TERRAIN_VERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

struct TERRAIN_VERTEX
{
	TERRAIN_VERTEX() {}
	TERRAIN_VERTEX( float x, float y, float z, float u, float v )
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

#endif // !D3DFVF_TERRAIN_VERTEX


class HeightMap:
	public Interface3D
{
public:
	HeightMap();
	virtual ~HeightMap();

private:
	TERRAIN_VERTEX*	m_HeightMap = nullptr;
	DWORD			m_HeightMapWidth = 0;
	DWORD			m_HeightMapHeight = 0;
	LPDIRECT3DVERTEXBUFFER9	m_HeightMapVertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9	m_HeightMapIndexBuffer = nullptr;

	bool			m_HeightMapWithQuadTreeIsReady = false;

	LPDIRECT3DTEXTURE9	m_MapHeightInfo = nullptr;
	LPDIRECT3DTEXTURE9	m_MapTexture = nullptr;
	LPDIRECT3DTEXTURE9	m_MapTextureArray[MAP_TEXTURE_COUNT];

	LPDIRECT3DTEXTURE9*	m_MeshTextures = nullptr;
	INT					m_MeshTextureSize = 0;
};