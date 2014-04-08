#pragma once

#include "MacroSet.h"

struct MESHOBJECT;

class Renderer: public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

	bool	Init();
	void	Destroy();
	bool	RenderBegin();
	bool	RenderEnd();

	bool	Render( MESHOBJECT* mesh );
	bool	CreateMesh( const LPCTSTR& fileName, MESHOBJECT* mesh );
	void	DeleteMesh( MESHOBJECT* mesh );
};

