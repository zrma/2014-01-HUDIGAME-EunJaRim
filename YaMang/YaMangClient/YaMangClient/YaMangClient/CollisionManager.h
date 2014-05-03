#pragma once
#include "MacroSet.h"

class Collision;

class CollisionManager:public Singleton<CollisionManager>
{
public:
	CollisionManager();
	~CollisionManager();

	void	AddCollision( Collision* collision );
	void	CheckCollision( Collision* collision ) const;

private:
	std::list<Collision*>	m_CollisionList;
};

