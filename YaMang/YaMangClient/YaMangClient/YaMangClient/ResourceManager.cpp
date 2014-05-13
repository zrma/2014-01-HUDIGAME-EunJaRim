#include "stdafx.h"
#include "ResourceManager.h"
#include "MainWindow.h"
#include "yaMangDxDll.h"
#include "MouseManager.h"
#include "YMClientDef.h"

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
	AddMesh( MESH_UNIT_SWORD, MESH_KEY_UNIT_SWORD );
	AddMesh( MESH_UNIT_PIKE, MESH_KEY_UNIT_PIKE );
	AddMesh( MESH_UNIT_ARROW, MESH_KEY_UNIT_ARROW );
	AddMesh( MESH_UNIT_KNIGHT, MESH_KEY_UNIT_KNIGHT );	
	AddMesh( MESH_UNIT_GUARD, MESH_KEY_UNIT_GUARD );

	InitGroundMesh( 100, 100 );
	CreateRawGround( 100, 100, 5.0f );	
	AddMap(MAP_128, MAP_128, MAP_KEY_TEST);
	CreateMap( MAP_KEY_TEST );

	InitSkyBoxMesh( 1000 );
	SetSkyBoxTexture(TEX_SKYBOX_BACK, SKY_BOX_BACK);
	SetSkyBoxTexture(TEX_SKYBOX_FRONT, SKY_BOX_FRONT);
	SetSkyBoxTexture(TEX_SKYBOX_LEFT, SKY_BOX_LEFT);
	SetSkyBoxTexture(TEX_SKYBOX_RIGHT, SKY_BOX_RIGHT);
	SetSkyBoxTexture(TEX_SKYBOX_TOP, SKY_BOX_TOP);
	SetSkyBoxTexture(TEX_SKYBOX_BOTTOM, SKY_BOX_BOTTOM);
	m_IsMapReady = true;

	InitCursor( CURSOR_MAX, MouseManager::GetInstance()->GetMousePositionX(), MouseManager::GetInstance()->GetMousePositionY() );
	CreateCursorImage(TEX_CURSOR_DEFAULT, CURSOR_DEFAULT);
	CreateCursorImage(TEX_CURSOR_ATTACK, CURSOR_ATTACK);
	CreateCursorImage(TEX_CURSOR_CLICK, CURSOR_CLICK);
	CreateCursorImage(TEX_CURSOR_OVER_CORPS, CURSOR_OVER_CORPS);
// 	fileName = L"cursor_corps_clicked.png";
// 	CreateCursorImage( fileName, CURSOR_CLICK_CORPS );
	CreateCursorImage(TEX_CURSOR_UNRECHEABLE, CURSOR_UNRECHEABLE);
	CreateCursorImage(TEX_CURSOR_ROTATION, CURSOR_CAMERA_ROTATING);
	CreateCursorImage(TEX_CURSOR_MOVABLE, CURSOR_CORPS_MOVABLE);
	CreateCursorImage(TEX_CURSOR_CLICKED, CURSOR_CORPS_MOVABLE_CLICK);
	CreateCursorImage(TEX_CURSOR_CLICKED_CORPS, CURSOR_OVER_PICKED_CORPS);

	m_ISCursorReady = true;
}

void ResourceManager::Destroy()
{
	CursorCleanUp();

	SkyBoxCleanUp();
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

void ResourceManager::AddMap( LPCTSTR heightMapFileName, LPCTSTR textureFileName, MapKeyType key )
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

bool ResourceManager::AddMesh( LPCTSTR fileName, MeshKeyType key )
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

