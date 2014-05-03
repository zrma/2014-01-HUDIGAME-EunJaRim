#pragma once

class Collision;

class CollisionList
{
public:
	CollisionList();
	~CollisionList();

	void	AddCollision( Collision* collision );
	void	CheckCollision( Collision* collision ) const;
	void	ClearList() { m_CollisionList.clear(); }

private:
	std::list<Collision*>	m_CollisionList;
};
