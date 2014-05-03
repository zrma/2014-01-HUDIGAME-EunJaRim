#include "stdafx.h"
#include "CollisionList.h"
#include "Collision.h"

CollisionList::CollisionList()
{
}


CollisionList::~CollisionList()
{
}

void CollisionList::AddCollision( Collision* collision )
{
	if ( collision )
	{
		m_CollisionList.push_back( collision );
	}
}

void CollisionList::CheckCollision( Collision* collision ) const
{
	for ( auto iter : m_CollisionList )
	{
		if ( collision == iter )
		{
			continue;
		}

		if ( collision->IsCollideWith( *iter ) )
		{
			collision->SetCollide();
			break;
		}
	}
}
