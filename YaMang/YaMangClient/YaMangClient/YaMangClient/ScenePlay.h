#pragma once
#include "Scene.h"

class Interface3D;

class ScenePlay:
	public Scene
{
public:
	ScenePlay();
	~ScenePlay();

	void	Create();
	void	Destroy();
	void	Update();
	void	Render();

private:
	std::hash_map<int, Interface3D*>	m_CorpsList;
};

