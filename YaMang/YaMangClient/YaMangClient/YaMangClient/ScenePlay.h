#pragma once
#include "Scene.h"

class Corps;
class MouseRender;

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
	void	MoveCorpsStart( int corpsID, float x, float z, float speed );
	void	MoveCorpsStop( int corpsID );

private:
	std::hash_map<int, Corps*>	m_CorpsList;

	MouseRender*	m_MouseCursor;
};

