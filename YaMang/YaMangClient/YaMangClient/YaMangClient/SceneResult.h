#pragma once
#include "Scene.h"
class SceneResult:
	public Scene
{
public:
	SceneResult();
	virtual ~SceneResult( );

	virtual void	Create();
	virtual void	Destroy();
	virtual void	Update();
	virtual void	Render() const;
};

