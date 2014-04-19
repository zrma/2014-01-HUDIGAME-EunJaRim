#include "stdafx.h"
#include "ResourceManager.h"
#include "MainWindow.h"
#include "yaMangDxDll.h"

ResourceManager::ResourceManager()
{
	ZeroMemory( &m_MeshList, sizeof( m_MeshList ) );
	ZeroMemory( &m_MapList, sizeof( m_MapList ) );
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

	LPCTSTR heightMapFileName = L"heightmap_1024_1024_korea.bmp";
	LPCTSTR textureFileName = L"heightmap_texture_1024_1024_korea.bmp";
	AddMap( heightMapFileName, textureFileName, MAP_KEY_KOREA );
}

void ResourceManager::Destroy()
{
	DeleteMap();

	for ( auto& toBeDelete : m_MeshList )
	{
		if ( toBeDelete )
		{
			DeleteMesh( toBeDelete->m_MeshObject );
			SafeDelete( toBeDelete->m_MeshObject );
			delete toBeDelete;
		}
	}
	for ( auto& toBeDelete : m_MapList )
	{
		SafeDelete( toBeDelete );
	}
}

void ResourceManager::AddMap( LPCTSTR& heightMapFileName, LPCTSTR& textureFileName, MapKeyType key )
{
	SafeDelete( m_MapList[key] );
	ResourceMap* map = new ResourceMap();
	m_MapList[key] = map;

	map->m_HeightMap = heightMapFileName;
	map->m_TextureMap = textureFileName;

	m_IsMapReady = CreateMap( key );
}

bool ResourceManager::CreateMap( MapKeyType key )
{
	if ( m_MapList[key] )
	{
		return ( S_OK == HeightMapTextureImport(
			MainWindow::GetInstance()->Window(), m_MapList[key]->m_HeightMap, m_MapList[key]->m_TextureMap ) );
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

	bool result = CreateMesh( fileName, mesh->m_MeshObject );

	if ( result )
	{
		SafeDelete( m_MeshList[key] );
		m_MeshList[key] = mesh;
	}
	
	return result;
}

bool ResourceManager::CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh )
{
	if ( S_OK == InitGeometry( MainWindow::GetInstance()->Window(), fileName, mesh ) )
	{
		return true;
	}
	return false;
}

void ResourceManager::DeleteMesh( MESHOBJECT* mesh )
{
	if ( mesh )
	{
		MeshObjectCleanUp( mesh );
	}
}

