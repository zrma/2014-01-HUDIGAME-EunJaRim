#include "stdafx.h"
#include "ResourceManager.h"
#include "MainWindow.h"
#include "yaMangDxDll.h"

ResourceManager::ResourceManager()
{
	// agebreak : 응?? 왜 Array에 ZeroMemory를 쓰지?? 위험한 코드. fill() 함수를 이용하셈
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
	// agebreak : 아래 코드는 안전한가?? 최초로 실행될때 아래 코드에서 무슨 일이 생길까?
	SafeDelete( m_MapList[key] );
	ResourceMap* map = new ResourceMap();
	m_MapList[key] = map;

	// agebreak : 문자열 카피는 잘못된 코드. 왜 잘못되었을까?? 
	map->m_HeightMap = heightMapFileName;
	map->m_TextureMap = textureFileName;

	m_IsMapReady = CreateMap( key );
}

bool ResourceManager::CreateMap( MapKeyType key )
{
	if ( m_MapList[key] ) // agebreak : 맵에서 유무를 확인할때 이렇게 하지 말라고 했음. 
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

// agebreak : 이런 형태에서 return값이 BOOL일 필요가 없음. 생성된 Mesh를 리턴하는 형태가 더 좋음. 생성못했을 경우에는 null을 리턴. 
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

