#include "stdafx.h"
#include "ResourceManager.h"
#include "MainWindow.h"
#include "yaMangDxDll.h"
#include "MouseManager.h"
#include "ResourceDef.h"

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
	AddMesh( MODEL_UNIT_SWORD, MESH_KEY_UNIT_SWORD );
	AddMesh( MODEL_UNIT_PIKE, MESH_KEY_UNIT_PIKE );
	AddMesh( MODEL_UNIT_ARROW, MESH_KEY_UNIT_ARROW );
	AddMesh( MODEL_UNIT_KNIGHT, MESH_KEY_UNIT_KNIGHT );
	AddMesh( MODEL_UNIT_GUARD, MESH_KEY_UNIT_GUARD );
	AddMesh( MODEL_UNIT_KING, MESH_KEY_UNIT_KING );

	AddMesh( MODEL_CORPS_DESTROY_A, MESH_KEY_CORPS_DESTROY_ENEMY );
	AddMesh( MODEL_CORPS_DEFENSE_A, MESH_KEY_CORPS_DEFENSE_ENEMY );
	AddMesh( MODEL_CORPS_RUSH_A, MESH_KEY_CORPS_RUSH_ENEMY );
	AddMesh( MODEL_CORPS_DESTROY_B, MESH_KEY_CORPS_DESTROY_MINE );
	AddMesh( MODEL_CORPS_DEFENSE_B, MESH_KEY_CORPS_DEFENSE_MINE );
	AddMesh( MODEL_CORPS_RUSH_B, MESH_KEY_CORPS_RUSH_MINE );

	InitGroundMesh( 100, 100 );
	CreateRawGround( 100, 100, 5.0f );
	m_MapSize = 500;

	AddMap( HEIGHT_MAP, HEIGHT_MAP, MAP_KEY_TEST );
	CreateMap( MAP_KEY_TEST );

	InitSkyBoxMesh( 1000 );

	SetSkyBoxTexture( SPRITE_SKYBOX_BACK, SKY_BOX_BACK );
	SetSkyBoxTexture( SPRITE_SKYBOX_FRONT, SKY_BOX_FRONT );
	SetSkyBoxTexture( SPRITE_SKYBOX_LEFT, SKY_BOX_LEFT );
	SetSkyBoxTexture( SPRITE_SKYBOX_RIGHT, SKY_BOX_RIGHT );
	SetSkyBoxTexture( SPRITE_SKYBOX_TOP, SKY_BOX_TOP );
	SetSkyBoxTexture( SPRITE_SKYBOX_BOTTOM, SKY_BOX_BOTTOM );
	m_IsMapReady = true;

	InitCursor( CURSOR_MAX, MouseManager::GetInstance()->GetMousePositionX(), MouseManager::GetInstance()->GetMousePositionY() );
	CreateCursorImage( SPRITE_CURSOR_DEFAULT, CURSOR_DEFAULT );
	CreateCursorImage( SPRITE_CURSOR_ATTACK, CURSOR_ATTACK );
	CreateCursorImage( SPRITE_CURSOR_CLICK, CURSOR_CLICK );
	CreateCursorImage( SPRITE_CURSOR_OVER_CORPS, CURSOR_OVER_CORPS );
	CreateCursorImage( SPRITE_CURSOR_UNRECHEABLE, CURSOR_UNRECHEABLE );
	CreateCursorImage( SPRITE_CURSOR_CAMERA_ROTATING, CURSOR_CAMERA_ROTATING );
	CreateCursorImage( SPRITE_CURSOR_CORPS_MOVABLE, CURSOR_CORPS_MOVABLE );
	CreateCursorImage( SPRITE_CURSOR_CORPS_MOVABLE_CLICK, CURSOR_CORPS_MOVABLE_CLICK );
	CreateCursorImage( SPRITE_CURSOR_OVER_PICKED_CORPS, CURSOR_OVER_PICKED_CORPS );

	m_ISCursorReady = true;

	CreateSprite();
}

void ResourceManager::Destroy()
{
	DeleteSprite();
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
	if ( S_OK == InitCharacterModel( MainWindow::GetInstance( )->Window( ), fileName, mesh ) )
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

void ResourceManager::CreateSprite()
{
	m_IsUISpriteReady = true;
	InitUISprite( SPRITE_MAX );

	CreateSpriteImage( SPRITE_IMAGE_LOADING, SPRITE_LOADING );
	CreateSpriteImage( SPRITE_IMAGE_RESULT_WIN, SPRITE_RESULT_WIN );
	CreateSpriteImage( SPRITE_IMAGE_RESULT_LOSE, SPRITE_RESULT_LOSE );

	//CreateUIImage
	CreateSpriteImage( SPRITE_UI_IMSIMAP, SPRITE_UI_MAP );
	CreateSpriteImage( SPRITE_UI_IMSIREGENBAR, SPRITE_UI_REGEN_BAR );
	CreateSpriteImage( SPRITE_UI_IMSIREGENFLAG, SPRITE_UI_REGEN_FLAG );
}

void ResourceManager::DeleteSprite()
{
	UICleanUp();
}

