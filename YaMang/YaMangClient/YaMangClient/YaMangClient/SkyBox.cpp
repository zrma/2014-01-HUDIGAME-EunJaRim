#include "stdafx.h"
#include "SkyBox.h"
#include "Renderer.h"
#include "CameraController.h"


SkyBox::SkyBox()
{
	ZeroMemory( m_SkyBoxTextures, sizeof( m_SkyBoxTextures ) );
}


SkyBox::~SkyBox()
{
}

bool SkyBox::Init( LPCTSTR backSideFileName, LPCTSTR frontSideFileName, LPCTSTR leftSideFileName, LPCTSTR rightSideFileName, LPCTSTR topSideFileName, LPCTSTR bottomSideFileName, UINT size )
{
	for ( int i = 0; i < 6; ++i )
	{
		SafeRelease( m_SkyBoxTextures[i] );
	}

	bool result = true;

	result &= Renderer::GetInstance()->CreateTexture( backSideFileName, &m_SkyBoxTextures[0] );
	result &= Renderer::GetInstance()->CreateTexture( frontSideFileName, &m_SkyBoxTextures[0] );
	result &= Renderer::GetInstance()->CreateTexture( leftSideFileName, &m_SkyBoxTextures[0] );
	result &= Renderer::GetInstance()->CreateTexture( rightSideFileName, &m_SkyBoxTextures[0] );
	result &= Renderer::GetInstance()->CreateTexture( topSideFileName, &m_SkyBoxTextures[0] );
	result &= Renderer::GetInstance()->CreateTexture( bottomSideFileName, &m_SkyBoxTextures[0] );

	if ( !result )
	{
		MessageBox( NULL, L"Failed To Load Sky Box Texture", L"SkyBox", MB_OK );
		return false;
	}

	if ( FAILED( D3DXCreateMeshFVF( 12, 24, D3DXMESH_MANAGED, D3DFVF_SKYBOX_VERTEX, Renderer::GetInstance()->GetDevice(), &m_SkyBoxMesh ) ) )
	{
		MessageBox( NULL, L"Failed To Create Sky Box Mesh", L"SkyBox", MB_OK );
		return false;
	}

	SKYBOX_VERTEX* vertexBuffer = nullptr;
	if ( FAILED( m_SkyBoxMesh->LockVertexBuffer( 0, (void**)&vertexBuffer ) ) )
	{
		MessageBox( NULL, L"Failed To Lock Sky Box Mesh Vertex Buffer", L"SkyBox", MB_OK );
		m_SkyBoxMesh->Release();
		return false;
	}

	// 전면   
	vertexBuffer[0] = SKYBOX_VERTEX( -1.0f, -1.0f, -1.0f, 0.0f, 1.0f );
	vertexBuffer[1] = SKYBOX_VERTEX( -1.0f, 1.0f, -1.0f, 0.0f, 0.0f );
	vertexBuffer[2] = SKYBOX_VERTEX( 1.0f, 1.0f, -1.0f, 1.0f, 0.0f );
	vertexBuffer[3] = SKYBOX_VERTEX( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f );
	// 후면
	vertexBuffer[4] = SKYBOX_VERTEX( -1.0f, -1.0f, 1.0f, 1.0f, 1.0f );
	vertexBuffer[5] = SKYBOX_VERTEX( -1.0f, 1.0f, 1.0f, 1.0f, 0.0f );
	vertexBuffer[6] = SKYBOX_VERTEX( 1.0f, 1.0f, 1.0f, 0.0f, 0.0f );
	vertexBuffer[7] = SKYBOX_VERTEX( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f );
	// 좌측
	vertexBuffer[8] = SKYBOX_VERTEX( -1.0f, -1.0f, 1.0f, 0.0f, 1.0f );
	vertexBuffer[9] = SKYBOX_VERTEX( -1.0f, 1.0f, 1.0f, 0.0f, 0.0f );
	vertexBuffer[10] = SKYBOX_VERTEX( -1.0f, 1.0f, -1.0f, 1.0f, 0.0f );
	vertexBuffer[11] = SKYBOX_VERTEX( -1.0f, -1.0f, -1.0f, 1.0f, 1.0f );
	// 우측
	vertexBuffer[12] = SKYBOX_VERTEX( 1.0f, -1.0f, 1.0f, 1.0f, 1.0f );
	vertexBuffer[13] = SKYBOX_VERTEX( 1.0f, 1.0f, 1.0f, 1.0f, 0.0f );
	vertexBuffer[14] = SKYBOX_VERTEX( 1.0f, 1.0f, -1.0f, 0.0f, 0.0f );
	vertexBuffer[15] = SKYBOX_VERTEX( 1.0f, -1.0f, -1.0f, 0.0f, 1.0f );
	// 상단
	vertexBuffer[16] = SKYBOX_VERTEX( -1.0f, 1.0f, -1.0f, 0.0f, 1.0f );
	vertexBuffer[17] = SKYBOX_VERTEX( -1.0f, 1.0f, 1.0f, 0.0f, 0.0f );
	vertexBuffer[18] = SKYBOX_VERTEX( 1.0f, 1.0f, 1.0f, 1.0f, 0.0f );
	vertexBuffer[19] = SKYBOX_VERTEX( 1.0f, 1.0f, -1.0f, 1.0f, 1.0f );
	// 하단
	vertexBuffer[20] = SKYBOX_VERTEX( -1.0f, -1.0f, -1.0f, 1.0f, 0.0f );
	vertexBuffer[21] = SKYBOX_VERTEX( -1.0f, -1.0f, 1.0f, 1.0f, 1.0f );
	vertexBuffer[22] = SKYBOX_VERTEX( 1.0f, -1.0f, 1.0f, 0.0f, 1.0f );
	vertexBuffer[23] = SKYBOX_VERTEX( 1.0f, -1.0f, -1.0f, 0.0f, 0.0f );

	for ( UINT i = 0; i < 24; ++i )
	{
		vertexBuffer[i].m_X *= size;
		vertexBuffer[i].m_Y *= size;
		vertexBuffer[i].m_Y += size / 10;
		vertexBuffer[i].m_Z *= size;
	}

	m_SkyBoxMesh->UnlockVertexBuffer();
	
	WORD* indexBuffer = 0;
	if ( FAILED( m_SkyBoxMesh->LockIndexBuffer( 0, (void**)&indexBuffer ) ) )
	{
		MessageBox( NULL, L"Failed To Lock Sky Box Mesh Index Buffer", L"SkyBox", MB_OK );
		m_SkyBoxMesh->Release();
		return false;
	}

	indexBuffer[0] = 0;		indexBuffer[1] = 1;		indexBuffer[2] = 2;
	indexBuffer[3] = 0;		indexBuffer[4] = 2;		indexBuffer[5] = 3;

	indexBuffer[6] = 4;		indexBuffer[7] = 5;		indexBuffer[8] = 6;
	indexBuffer[9] = 4;		indexBuffer[10] = 6;	indexBuffer[11] = 7;

	indexBuffer[12] = 8;	indexBuffer[13] = 9;	indexBuffer[14] = 10;
	indexBuffer[15] = 8;	indexBuffer[16] = 10;	indexBuffer[17] = 11;

	indexBuffer[18] = 12;	indexBuffer[19] = 13;	indexBuffer[20] = 14;
	indexBuffer[21] = 12;	indexBuffer[22] = 14;	indexBuffer[23] = 15;

	indexBuffer[24] = 16;	indexBuffer[25] = 17;	indexBuffer[26] = 18;
	indexBuffer[27] = 16;	indexBuffer[28] = 18;	indexBuffer[29] = 19;

	indexBuffer[30] = 20;	indexBuffer[31] = 21;	indexBuffer[32] = 22;
	indexBuffer[33] = 20;	indexBuffer[34] = 22;	indexBuffer[35] = 23;

	m_SkyBoxMesh->UnlockIndexBuffer();

	DWORD* attributeBuffer = 0;
	if ( FAILED( m_SkyBoxMesh->LockAttributeBuffer( 0, &attributeBuffer ) ) )
	{
		MessageBox( NULL, L"Failed To Lock Sky Box Mesh Attribute Buffer", L"SkyBox", MB_OK );
		m_SkyBoxMesh->Release();
		return false;
	}

	// triangles 1 - 4
	for ( int a = 0; a < 2; a++ )
	{
		// subset 0
		attributeBuffer[a] = 0;
	}
	// triangles 5-8
	for ( int b = 2; b < 4; b++ )
	{
		// subset 1   
		attributeBuffer[b] = 1;
	}
	// triangles 9-12   
	for ( int c = 4; c < 6; c++ )
	{
		// subset 2   
		attributeBuffer[c] = 2;
	}
	// triangles 1-4   
	for ( int d = 6; d < 8; d++ )
	{
		// subset 0  
		attributeBuffer[d] = 3;
	}
	// triangles 5-8   
	for ( int e = 8; e < 10; e++ )
	{
		// subset 1
		attributeBuffer[e] = 4;
	}
	// triangles 9-12   
	for ( int f = 10; f < 12; f++ )
	{
		// subset 2   
		attributeBuffer[f] = 5;
	}
	m_SkyBoxMesh->UnlockAttributeBuffer();

	return true;
}

void SkyBox::Destroy()
{
	SafeRelease( m_SkyBoxMesh );

	for ( int i = 0; i < 6; ++i )
	{
		SafeRelease( m_SkyBoxTextures[i] );
	}
}

void SkyBox::Render() const
{
	LPDIRECT3DDEVICE9 d3dDevice = Renderer::GetInstance()->GetDevice();
	D3DXVECTOR3 eyePoint = CameraController::GetInstance()->GetEyePoint();

	if ( m_SkyBoxMesh && d3dDevice )
	{
		D3DXMATRIXA16 moveMatrix;
		D3DXMatrixTranslation( &moveMatrix, eyePoint.x, eyePoint.y, eyePoint.z );
		d3dDevice->SetTransform( D3DTS_WORLD, &moveMatrix );

		d3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		d3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_GAUSSIANQUAD );
		d3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_GAUSSIANQUAD );

		d3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		d3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

		for ( int i = 0; i < 6; i++ )
		{
			d3dDevice->SetTexture( 0, m_SkyBoxTextures[i] );
			m_SkyBoxMesh->DrawSubset( i );
		}

		d3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		d3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	}
}
