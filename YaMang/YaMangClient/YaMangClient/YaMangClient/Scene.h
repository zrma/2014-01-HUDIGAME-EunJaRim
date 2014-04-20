#pragma once

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual void	Create() = 0;
	virtual void	Destroy() = 0;

	virtual void	Update() = 0;
	virtual void	Render() = 0;
};

