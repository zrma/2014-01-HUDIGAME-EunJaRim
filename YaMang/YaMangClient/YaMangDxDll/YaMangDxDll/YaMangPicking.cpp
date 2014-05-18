#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// Picking Ray를 위한 부분
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API void CalcPickingRay( int mouseX, int mouseY )
{
	D3DVIEWPORT9 viewPort;
	D3DXMATRIXA16 projectionMatrix;

	float rayX = 0.f;
	float rayY = 0.f;

	g_D3dDevice->GetViewport( &viewPort );
	g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

	rayX = ( ( ( ( ( mouseX - viewPort.X ) * 2.0f / (float)viewPort.Width ) - 1.0f ) ) - projectionMatrix( 2, 0 ) ) / projectionMatrix( 0, 0 );
	rayY = ( ( -( ( ( mouseY - viewPort.Y ) * 2.0f / (float)viewPort.Height ) - 1.0f ) ) - projectionMatrix( 2, 1 ) ) / projectionMatrix( 1, 1 );
	// Log( "Ray x : %f, y : %f\n", rayX, rayY );

	//viewport트랜스, 프로젝션 트랜스 역행
	g_RayOrigin = { 0.f, 0.f, 0.f };
	g_RayDirection = { rayX, rayY, 1.f };
	// Log( "뷰포트, 프로젝션 역행\n" );

	//뷰잉 트랜스 역행
	D3DXMATRIXA16 viewingMatrix;
	g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
	D3DXMatrixInverse( &viewingMatrix, 0, &viewingMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &viewingMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &viewingMatrix );
	// Log( "뷰잉 좌표 역행\n" );

	//월드 좌표로 역행
	D3DXMATRIXA16 worldMatrix;
	g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
	D3DXMatrixInverse( &worldMatrix, 0, &worldMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &worldMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &worldMatrix );
	// Log( "월드 좌표 역행\n" );
	// Log( "origin: %f,%f,%f\n direction: %f, %f, %f\n", g_RayOrigin.x, g_RayOrigin.y, g_RayOrigin.z, g_RayDirection.x, g_RayDirection.y, g_RayDirection.z );

}

YAMANGDXDLL_API HRESULT TransPickedTriangle( float* pickedX, float* pickedZ )
{
	if ( !( pickedX && pickedZ ) )
	{
		return S_FALSE;
	}

	LPDIRECT3DVERTEXBUFFER9 presentVertexBuffer;
	g_Mesh->GetVertexBuffer( &presentVertexBuffer );

	CUSTOMVERTEX* VerticesStartPoint;
	presentVertexBuffer->Lock( 0, 0, (void**)&VerticesStartPoint, 0 );

	BOOL hit1 = false;
	BOOL hit2 = false;
	float dist1 = 0;
	float dist2 = 0;

	int trianglePointA = NULL, trianglePointB = NULL, trianglePointC = NULL, trianglePointD = NULL;

	
	//한번 루프에에서 4점씩 확인(ABCD)
	//4점으로 삼각형 2개를 확인할 수 있기 때문에 한 루프에서 hit가 두 종류 발생될 수 있음
	//hit가 발생하면 더 이상 탐색하지 않아도 되기 때문에 loop문 탈출
	for ( UINT z = 0; ( z < ( g_ZHeight - 1 ) ) && !( hit1 | hit2 ); ++z )
	{
		if ( z % 2 == 0 )
		{
			for ( UINT x = 0; ( x < ( g_XHeight - 1 ) ) && !( hit1 | hit2 ); ++x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x + 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].m_VertexPoint, &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x + 1 );
				hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &VerticesStartPoint[trianglePointD].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
		else
		{
			for ( UINT x = g_ZHeight - 1; ( x > 0 ) && !( hit1 | hit2 ); --x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x - 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].m_VertexPoint, &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x - 1 );
				hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].m_VertexPoint, &VerticesStartPoint[trianglePointC].m_VertexPoint, &VerticesStartPoint[trianglePointD].m_VertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
	}

	HRESULT result = S_FALSE;

	if ( ( hit1 && ( dist1 > 0 ) ) || ( hit2 && ( dist2 > 0 ) ) )
	{
		CUSTOMVERTEX* intersectedVertexBufferStart;
		g_Mesh->LockVertexBuffer( 0, (void**)&intersectedVertexBufferStart );

		CUSTOMVERTEX* pointA;
		CUSTOMVERTEX* pointB;
		CUSTOMVERTEX* pointC;

		if ( hit1 )
		{
			pointA = &VerticesStartPoint[trianglePointA];
			pointB = &VerticesStartPoint[trianglePointB];
			pointC = &VerticesStartPoint[trianglePointC];
		}
		else
		{
			pointA = &VerticesStartPoint[trianglePointB];
			pointB = &VerticesStartPoint[trianglePointC];
			pointC = &VerticesStartPoint[trianglePointD];
		}

		*pickedX += pointA->m_VertexPoint.x;
		*pickedZ += pointA->m_VertexPoint.z;

		g_Mesh->UnlockVertexBuffer();

		result = S_OK;
	}

	presentVertexBuffer->Unlock();
	presentVertexBuffer->Release();

	return result;
}

/*

		modeSelector = static_cast<AreaModeType>( modeSelector );

		switch ( modeSelector )
		{
			case AREA_MODE_NONE:

				break;
				// 			case AREA_MODE_COLOR:
				// 			{
				// 				pointA->m_Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				// 				pointB->m_Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				// 				pointC->m_Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				// 			}
				break;
			case AREA_MODE_HIGHER:
				pointA->m_VertexPoint.y += 3;
				pointB->m_VertexPoint.y += 3;
				pointC->m_VertexPoint.y += 3;
				break;
			case AREA_MODE_LOWER:
				pointA->m_VertexPoint.y -= 3;
				pointB->m_VertexPoint.y -= 3;
				pointC->m_VertexPoint.y -= 3;
				break;
			case AREA_MODE_PAINT_TEXTURE:
				//텍스쳐 입히기 실험
				pointA->m_VertexTexturePoint1.x = 0.f;
				pointA->m_VertexTexturePoint1.y = 0.f;
				pointB->m_VertexTexturePoint1.x = 1.f;
				pointB->m_VertexTexturePoint1.y = 1.f;
				pointC->m_VertexTexturePoint1.x = 1.f;
				pointC->m_VertexTexturePoint1.y = 1.f;
				break;
			default:
				break;
		}	
	}
}

*/