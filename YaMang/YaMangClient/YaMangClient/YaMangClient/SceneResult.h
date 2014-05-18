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

private:
	D3DXVECTOR3 m_Position = { 0, 0, 0 };
};

