#include "stdafx.h"
#include "ResourceManager.h"
#include "MainWindow.h"
#include "yaMangDxDll.h"

ResourceManager::ResourceManager()
{
	m_MeshArray.fill( nullptr );
	m_HeightMapArray.fill( nullptr );
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init()
{
	LPCTSTR fileName = L"UnitSword.x";
	AddMesh( fileName, MESH_KEY_UNIT_SWORD );
	fileName = L"UnitPike.x";
	AddMesh( fileName, MESH_KEY_UNIT_PIKE );
	fileName = L"UnitArrow.x";
	AddMesh( fileName, MESH_KEY_UNIT_ARROW );
	fileName = L"UnitKnight.x";
	AddMesh( fileName, MESH_KEY_UNIT_KNIGHT );
	fileName = L"UnitGuard.x";
	AddMesh( fileName, MESH_KEY_UNIT_GUARD );

// 	LPCTSTR heightMapFileName = L"heightmap_1024_1024_korea.bmp";
// 	LPCTSTR textureFileName = L"heightmap_texture_1024_1024_korea.bmp";
// 	AddMap( heightMapFileName, textureFileName, MAP_KEY_KOREA );

	InitGroundMesh( 100, 100 );
	CreateRawGround( 100, 100, 1.0f );
	m_IsMapReady = true;
}

void ResourceManager::Destroy()
{
	DeleteMap();

	for ( auto& toBeDelete : m_MeshArray )
	{
		if ( toBeDelete )
		{
			DeleteMesh( toBeDelete->m_MeshObject );
			SafeDelete( toBeDelete->m_MeshObject );
			delete toBeDelete;
		}
	}
	for ( auto& toBeDelete : m_HeightMapArray )
	{
		SafeDelete( toBeDelete );
	}
}

void ResourceManager::AddMap( LPCTSTR& heightMapFileName, LPCTSTR& textureFileName, MapKeyType key )
{
	SafeDelete( m_HeightMapArray[key] );
		
	ResourceMap* map = new ResourceMap();
	m_HeightMapArray[key] = map;

	map->m_HeightMap.append( heightMapFileName );
	map->m_TextureMap.append( textureFileName );

	m_IsMapReady = CreateMap( key );
}

bool ResourceManager::CreateMap( MapKeyType key )
{
	if ( m_HeightMapArray[key] )
	{
		return ( S_OK == HeightMapTextureImport(
			MainWindow::GetInstance()->Window(), m_HeightMapArray[key]->m_HeightMap.c_str(), m_HeightMapArray[key]->m_TextureMap.c_str() ) );
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 우선은 이렇게 해 두지만,
// 나중에 맵이 여러 종류가 될 경우
// 씬 쪽에서 필요한 맵들을 불러오고 제거할 수 있게 되어야 한다.
//////////////////////////////////////////////////////////////////////////
void ResourceManager::DeleteMap()
{
	if ( m_IsMapReady )
	{
		HeightMapCleanup();
	}
	m_IsMapReady = false;
}

bool ResourceManager::AddMesh( LPCTSTR& fileName, MeshKeyType key )
{
	ResourceMesh* mesh = new ResourceMesh();
	mesh->m_MeshObject = new MESHOBJECT();

	CreateMesh( fileName, mesh->m_MeshObject );

	if ( mesh->m_MeshObject )
	{
		SafeDelete( m_MeshArray[key] );
		m_MeshArray[key] = mesh;

		return true;
	}
	
	SafeDelete( mesh->m_MeshObject );
	SafeDelete( mesh );
	return false;
}

void ResourceManager::CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh )
{
	if ( S_OK == InitGeometry( MainWindow::GetInstance()->Window(), fileName, mesh ) )
	{
		return;
	}
	else mesh = nullptr;
}

void ResourceManager::DeleteMesh( MESHOBJECT* mesh )
{
	if ( mesh )
	{
		MeshObjectCleanUp( mesh );
	}
}

