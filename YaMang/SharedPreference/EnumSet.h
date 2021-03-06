#pragma once

enum class UnitType: unsigned char
{
	UNIT_NONE = 0,

	UNIT_ARROW = 10,
	UNIT_GUARD = 20,
	UNIT_KNIGHT = 30,
	UNIT_PIKE = 40,
	UNIT_SWORD = 50,
	UNIT_KING = 60,

	UNIT_MAX = 255
};

enum class FormationType: unsigned char
{
	FORMATION_NONE = 0,

	FORMATION_DEFENSE = 10,
	FORMATION_DESTROY = 20,
	FORMATION_RUSH = 30,

	FORMATION_MAX = 255
};

enum class MapTileType: unsigned char
{
	MAP_TILE_NONE = 0,

	MAP_TILE_ROAD = 10,
	MAP_TILE_GRASS = 20,
	MAP_TILE_WATER = 30,
	MAP_TILE_WALL = 40,

	MAP_TILE_MAX = 255
};

enum class MapObjectType: unsigned char
{
	MAP_OBJECT_NONE = 0,

	MAP_OBJECT_BASE = 10,
	MAP_OBJECT_KING = 20,

	MAP_OBJECT_MAX = 255
};

enum SceneType
{
	SCENE_NONE,

	SCENE_LOAD,
	SCENE_PLAY,
	SCENE_RESULT,

	SCENE_MAX
};

enum MeshKeyType
{
	MESH_KEY_NONE,
	
	MESH_KEY_UNIT_SWORD,
	MESH_KEY_UNIT_PIKE,
	MESH_KEY_UNIT_ARROW,
	MESH_KEY_UNIT_KNIGHT,
	MESH_KEY_UNIT_GUARD,
	MESH_KEY_UNIT_KING,

	MESH_KEY_CORPS_DESTROY_MINE,
	MESH_KEY_CORPS_DEFENSE_MINE,
	MESH_KEY_CORPS_RUSH_MINE,

	MESH_KEY_CORPS_DESTROY_ENEMY,
	MESH_KEY_CORPS_DEFENSE_ENEMY,
	MESH_KEY_CORPS_RUSH_ENEMY,

	MESH_KEY_MAX
};

enum MapKeyType
{
	MAP_KEY_NONE,

	MAP_KEY_TEST,

	MAP_KEY_MAX
};

enum SpriteKeyType
{
	SPRITE_NONE,
	
	SPRITE_LOADING,
	SPRITE_RESULT_WIN,
	SPRITE_RESULT_LOSE,

	SPRITE_UI_MAP,
	SPRITE_UI_REGEN_BAR,
	SPRITE_UI_REGEN_FLAG,
	SPRITE_UI_CORPSMARK_BLUE,
	SPRITE_UI_CORPSMARK_RED,
	SPRITE_UI_CORPSFLAG_GRAY,
	SPRITE_UI_CORPSFLAG_RED,
	SPRITE_UI_CORPSFLAG_BLUE,

	SPRITE_MAX
};

enum ResourceType
{
	RESOURCE_NONE,

	RESOURCE_MESH,
	RESOURCE_MAP,
	RESOURCE_SOUNDE,
	RESOURCE_SPRITE,

	RESOURCE_MAX
};

enum ActionStatusType
{
	ACTION_STATUS_NONE,

	ACTION_STATUS_ROTATE,
	ACTION_STATUS_GOFOWARD,
	ACTION_STATUS_ATTACK,
	
	ACTION_STATUS_RUSH,
	ACTION_STATUS_KNOCKBACK,

	ACTION_STATUS_MAX
};

enum CorpsTextureType
{
	CORPS_TEXTURE_NONE,

	CORPS_TEXTURE_ARROW,
	CORPS_TEXTURE_GUARD,
	CORPS_TEXTURE_KING,
	CORPS_TEXTURE_KNIGHT,
	CORPS_TEXTURE_PIKE,
	CORPS_TEXTURE_SWORD,

	CORPS_TEXTURE_MAX
};

enum UnitStatusType
{
	UNIT_STATUS_ENEMY = 0x1,
	UNIT_STATUS_ATTACK = 0x2,
	UNIT_STATUS_SELECT = 0x4
};