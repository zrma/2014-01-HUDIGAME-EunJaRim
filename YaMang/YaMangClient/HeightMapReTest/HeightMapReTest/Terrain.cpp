#include "Terrain.h"



Terrain::Terrain()
{
	m_pkDevice = NULL;
	m_pkVB = NULL;
	m_pkIndexVB = NULL;
	m_pkTexture = NULL;
	iWidth = 0;
	iHeight = 0;
}

Terrain::~Terrain()
{
	Shutdown();
}



int Terrain::CreateVertexBuffer( IDirect3DDevice9  *m_pkDev, char *szTexFile )
{
	m_pkDevice = m_pkDev;




	D3DXCreateTextureFromFileExA( m_pkDevice, szTexFile, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
								  D3DX_DEFAULT, NULL, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
								  D3DX_FILTER_NONE, D3DX_FILTER_NONE, NULL, NULL, NULL, &m_pkTexture );

	iWidth = 64;
	iHeight = 64;

	m_pkDevice->CreateVertexBuffer( ( iWidth * iHeight ) * sizeof( VERTEX ),
									0, VERTEX_FVF, D3DPOOL_DEFAULT, &m_pkVB, NULL );




	D3DLOCKED_RECT d3drc;


	m_pkTexture->LockRect( 0, &d3drc, 0, D3DLOCK_DISCARD );

	m_pkVB->Lock( 0, 0, (void **) &pVertex, 0 );

	BYTE *pkBuffer = static_cast<BYTE *>( d3drc.pBits );

	for ( int z = 0; z < iHeight; ++z )
	{
		for ( int x = 0; x < iWidth; ++x )
		{
			pVertex->Vec.x = (float) x - iWidth / 2.0f;
			pVertex->Vec.z = -( (float) z - iHeight / 2.0f );

			BYTE B = pkBuffer[z*d3drc.Pitch + x * 4 + 0];
			BYTE G = pkBuffer[z*d3drc.Pitch + x * 4 + 1];
			BYTE R = pkBuffer[z*d3drc.Pitch + x * 4 + 2];
			BYTE A = pkBuffer[z*d3drc.Pitch + x * 4 + 3];

			pVertex->Vec.y = float( B + G + R + A ) * 0.02f;

			pVertex->Color = 0xFFFFFFFF;

			pVertex->tu = (float) x / iWidth;
			pVertex->tv = (float) z / iHeight;

			++pVertex;
		}
	}

	m_pkTexture->UnlockRect( 0 );
	m_pkVB->Unlock();


	return S_OK;
}


int Terrain::CreateIndexBuffer()
{

	m_pkDevice->CreateIndexBuffer( ( iHeight - 1 )*( iWidth - 1 ) * 2 * sizeof( INDEX ), 0,
								   D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pkIndexVB, NULL );


	m_pkIndexVB->Lock( 0, 0, (void**) &pIndice, 0 );

	for ( int z = 0; z < iHeight - 1; ++z )
	{
		for ( int x = 0; x < iWidth - 1; ++x )
		{
			pIndice->_0 = static_cast<WORD> ( z*iHeight + x );
			pIndice->_1 = static_cast<WORD> ( z*iHeight + x + 1 );
			pIndice->_2 = static_cast<WORD> ( ( z + 1 )*iHeight + x );
			++pIndice;

			pIndice->_0 = static_cast<WORD> ( ( z + 1 )*iHeight + x );
			pIndice->_1 = static_cast<WORD> ( z*iHeight + x + 1 );
			pIndice->_2 = static_cast<WORD> ( ( z + 1 )*iHeight + x + 1 );
			++pIndice;
		}
	}

	m_pkIndexVB->Unlock();


	return S_OK;
}



void Terrain::Render()
{
	m_pkDevice->SetTexture( 0, m_pkTexture );
	m_pkDevice->SetStreamSource( 0, m_pkVB, 0, sizeof( VERTEX ) );
	m_pkDevice->SetFVF( VERTEX_FVF );
	m_pkDevice->SetIndices( m_pkIndexVB );
	m_pkDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, ( iWidth * iHeight ), 0, ( iHeight - 1 )*( iWidth - 1 ) * 2 );

}

void Terrain::Shutdown()
{
	if ( m_pkTexture != NULL ) { m_pkTexture->Release(); }
	if ( m_pkIndexVB != NULL ) { m_pkIndexVB->Release(); }
	if ( m_pkVB != NULL ) { m_pkVB->Release(); }
}