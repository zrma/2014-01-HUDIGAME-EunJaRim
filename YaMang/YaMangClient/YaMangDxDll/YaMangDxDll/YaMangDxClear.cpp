#include "stdafx.h"
#include "yaMangDxDll.h"
#include "Logger.h"
#include "GlobalVar.h"


/**
 *@breif MeshObject(유닛 등)에 대한 메모리 소거 함수
 *
 */
YAMANGDXDLL_API void MeshObjectCleanUp( MESHOBJECT* inputVal )
{
	if ( NULL != inputVal->MeshMarterials )
	{
		delete[] inputVal->MeshMarterials;
	}
	if ( inputVal->MeshTexture )
	{
		for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
		{
			if ( inputVal->MeshTexture[i] )
			{
				inputVal->MeshTexture[i]->Release();
			}
		}
		delete[] inputVal->MeshTexture;
	}
	if ( NULL != inputVal->importedMesh )
	{
		( inputVal->importedMesh )->Release();
	}
}


YAMANGDXDLL_API void HeightMapCleanup()
{
	if ( g_TexHeight != NULL )
	{
		g_TexHeight->Release();
	}

	if ( g_Tex0 != NULL )
	{
		g_Tex0->Release();
	}
}


YAMANGDXDLL_API void CursorCleanUp()
{
	if ( g_CursorSprite )
	{
		g_CursorSprite->Release();
	}

	if ( g_CursorTex )
	{
		for ( int i = 0; i < g_CursorMaxSize; ++i )
		{
			if ( g_CursorTex[i] )
			{
				g_CursorTex[i]->Release();
			}
		}
		delete[] g_CursorTex;
		g_CursorTex = nullptr;
	}
}


YAMANGDXDLL_API void SkyBoxCleanUp()
{
	if ( g_SkyBoxMesh != NULL )
	{
		g_SkyBoxMesh->Release();
	}

	for ( int i = 0; i < 6; ++i )
	{
		if ( g_SkyBoxTextures[i] != NULL )
		{
			g_SkyBoxTextures[i]->Release();
		}
	}
}


YAMANGDXDLL_API void D3DCleanUp()
{
	// 텍스트 소멸자 합침
	if ( g_Font != NULL )
	{
		g_Font->Release();
	}

	if ( g_Sprite != NULL )
	{
		g_Sprite->Release();
	}

	if ( g_Mesh != NULL )
	{
		g_Mesh->Release();
	}

	if ( NULL != g_D3dDevice )
	{
		g_D3dDevice->Release();
	}

	if ( NULL != g_D3D )
	{
		g_D3D->Release();
	}

#ifdef _PRINT_CONSOLE
	Logger::Release();
#endif
}