#include "stdafx.h"
#include "ResourceManager.h"
#include "MainWindow.h"
#include "yaMangDxDll.h"
#include "MouseManager.h"

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
	fileName = L"UnitKing.x";
	AddMesh( fileName, MESH_KEY_UNIT_KING );

	fileName = L"Corps_Destroy.x";
	AddMesh( fileName, MESH_KEY_CORPS_DESTROY );
	fileName = L"Corps_Defense.x";
	AddMesh( fileName, MESH_KEY_CORPS_DEFENSE );
	fileName = L"Corps_Rush.x";
	AddMesh( fileName, MESH_KEY_CORPS_RUSH );

	InitGroundMesh( 100, 100 );
	CreateRawGround( 100, 100, 5.0f );
	m_MapSize = 500;

	fileName = L"heightmap_128_128.bmp";
	AddMap( fileName, fileName, MAP_KEY_TEST );
	CreateMap( MAP_KEY_TEST );


	InitSkyBoxMesh( 1000 );

	fileName = L"SkyBox_Back.png";
	SetSkyBoxTexture( fileName, SKY_BOX_BACK );
	fileName = L"SkyBox_Front.png";
	SetSkyBoxTexture( fileName, SKY_BOX_FRONT );
	fileName = L"SkyBox_Left.png";
	SetSkyBoxTexture( fileName, SKY_BOX_LEFT );
	fileName = L"SkyBox_Right.png";
	SetSkyBoxTexture( fileName, SKY_BOX_RIGHT );
	fileName = L"SkyBox_Top.png";
	SetSkyBoxTexture( fileName, SKY_BOX_TOP );
	fileName = L"SkyBox_Bottom.png";
	SetSkyBoxTexture( fileName, SKY_BOX_BOTTOM );
	m_IsMapReady = true;

	InitCursor( CURSOR_MAX, MouseManager::GetInstance()->GetMousePositionX(), MouseManager::GetInstance()->GetMousePositionY() );
	fileName = L"cursor_default.png";
	CreateCursorImage( fileName, CURSOR_DEFAULT );
	fileName = L"cursor_attack.png";
	CreateCursorImage( fileName, CURSOR_ATTACK );
	fileName = L"cursor_clicked.png";
	CreateCursorImage( fileName, CURSOR_CLICK );
	fileName = L"cursor_corps_clickable.png";
	CreateCursorImage( fileName, CURSOR_OVER_CORPS );
// 	fileName = L"cursor_corps_clicked.png";
// 	CreateCursorImage( fileName, CURSOR_CLICK_CORPS );
	fileName = L"cursor_unreachable.png";
	CreateCursorImage( fileName, CURSOR_UNRECHEABLE );
	fileName = L"cursor_camera_rotating.png";
	CreateCursorImage(fileName, CURSOR_CAMERA_ROTATING);
	fileName = L"cursor_movable.png";
	CreateCursorImage(fileName, CURSOR_CORPS_MOVABLE);
	fileName = L"cursor_movable_clicked.png";
	CreateCursorImage(fileName, CURSOR_CORPS_MOVABLE_CLICK);
	fileName = L"cursor_over_clicked_corps.png";
	CreateCursorImage(fileName, CURSOR_OVER_PICKED_CORPS);

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

void ResourceManager::CreateMesh( LPCTSTR fileName, MESHOBJECT* mesh )
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

