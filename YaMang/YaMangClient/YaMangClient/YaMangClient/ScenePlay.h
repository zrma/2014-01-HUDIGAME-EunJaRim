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

	void	AddCorps( int corpsID, Corps* corps );
	void	ChangeCorpsFormation( int corpsID, FormationType formationType );

private:
	std::hash_map<int, Corps*>	m_CorpsList;
};

