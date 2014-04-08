#include "stdafx.h"
#include "../yaMangDxDll/yaMangDxDll/MeshObject.h"
#include "Renderer.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	return true;
}

void Renderer::Destroy()
{

}

bool Renderer::RenderBegin()
{
	return true;
}

bool Renderer::RenderEnd()
{
	return true;
}

bool Renderer::Render( MESHOBJECT* mesh )
{
	return true;
}

bool Renderer::CreateMesh( const LPCTSTR& fileName, MESHOBJECT* mesh )
{
	return true;
}

void Renderer::DeleteMesh( MESHOBJECT* mesh )
{

}
