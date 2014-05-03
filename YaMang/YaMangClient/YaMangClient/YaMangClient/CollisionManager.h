#pragma once
#include "MacroSet.h"

class CollisionManager:public Singleton<CollisionManager>
{
public:
	CollisionManager();
	~CollisionManager();
};

