#pragma once
#include "Scene.h"

class SceneLoad : public Scene
{
public:
	SceneLoad();
	virtual ~SceneLoad();

	void		Create();
	void		Destroy();
	void		Update();
	void		Render() const;

};