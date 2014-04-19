#pragma once

enum UnitType
{
	UNIT_NONE = 0,

	UNIT_ARROW = 10,
	UNIT_GUARD = 20,
	UNIT_KNIGHT = 30,
	UNIT_PIKE = 40,
	UNIT_SWORD = 50,

	UNIT_MAX = 1024
};

enum SceneType
{
	SCENE_NONE,

	SCENE_LOAD,
	SCENE_PLAY,
	SCENE_RESULT,

	SCENE_MAX
};

enum WidgetType
{
	WIDGET_NONE,
	//////////////////////////////////////////////////////////////////////////
	WIDGET_MESH_NONE,

	WIDGET_MESH_UNIT_SWORD,
	WIDGET_MESH_UNIT_PIKE,
	WIDGET_MESH_UNIT_ARROW,
	WIDGET_MESH_UNIT_KNIGHT,

	WIDGET_MESH_MAX,
	//////////////////////////////////////////////////////////////////////////
	WIDGET_MAP_NONE,

	WIDGET_MAP_HEIGHT,
	WIDGET_MAP_TEXTURE,

	WIDGET_MAP_MAX,
	//////////////////////////////////////////////////////////////////////////
	WIDGET_MAX
};

enum ResourceType
{
	RESOURCE_NONE,

	RESOURCE_WIDGET_MESH,
	RESOURCE_WIDGET_BITMAP,
	RESOURCE_SOUNDE,

	RESOURCE_MAX
};