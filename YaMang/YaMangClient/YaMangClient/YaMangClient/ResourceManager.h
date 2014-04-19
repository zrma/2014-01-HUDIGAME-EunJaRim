#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

struct MESHOBJECT;

struct Resource
{
	Resource(): m_Type( RESOURCE_NONE ) {}
	ResourceType m_Type;
};

struct ResourceWidgetMesh: public Resource
{
	ResourceWidgetMesh()
	{
		m_Type = RESOURCE_WIDGET_MESH;
	}
	MESHOBJECT*	m_MeshObject = nullptr;
};

struct ResourceWidgetBitmap: public Resource
{
	ResourceWidgetBitmap()
	{
		m_Type = RESOURCE_WIDGET_BITMAP;
	}
	
	LPCTSTR	m_HeightMap = nullptr;
	LPCTSTR m_TextureMap = nullptr;
};

class ResourceManager: public Singleton<ResourceManager>
{
public:
	ResourceManager();
	~ResourceManager();

	void Init();
	void Destroy();

private:
	std::hash_map<WidgetType, Resource*>	m_ResourceManager;
};

