#include "stdafx.h"
#include "CollisionManager.h"
#include "Collision.h"

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::AddCollision( Collision* collision )
{
	if ( collision )
	{
		m_CollisionList.push_back( collision );
	}
}

void CollisionManager::RemoveColiision( Collision* collision )
{
	if ( collision )
	{
		for ( auto iter = m_CollisionList.begin(); iter != m_CollisionList.end(); )
		{
			if ( collision == *iter )
			{
				iter = m_CollisionList.erase( iter );
			}
			else
			{
				++iter;
			}
		}
	}
}

void CollisionManager::CheckCollision( Collision* collision ) const
{
	for ( auto iter : m_CollisionList )
	{
		if ( collision == iter )
		{
			continue;
		}

		if ( collision->IsCollideWith( *iter ) )
		{
			break;
		}
	}
}
