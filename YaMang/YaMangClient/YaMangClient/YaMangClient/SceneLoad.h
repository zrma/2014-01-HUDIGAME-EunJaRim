#pragma once
#include "Scene.h"

class SceneLoad : public Scene
{
public:
	SceneLoad();
	virtual ~SceneLoad();

	virtual void	Create();
	virtual void	Destroy();
	virtual void	Update();
	virtual void	Render() const;

};