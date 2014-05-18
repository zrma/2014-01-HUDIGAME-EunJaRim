#pragma once
#include "EnumSet.h"
class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void	Create() = 0;
	virtual void	Destroy() = 0;

	virtual void	Update() = 0;
	virtual void	Render() const = 0;

	SceneType		GetSceneType() { return m_SceneType; }

private:
	SceneType		m_SceneType;
};

