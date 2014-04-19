#pragma once
#include "Scene.h"

class Corps;

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
	std::hash_map<int, Corps*>	m_CorpsList;
};

