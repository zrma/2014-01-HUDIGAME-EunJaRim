#pragma once
#include "Action.h"
class Attack:
	public Action
{
public:
	Attack();
	~Attack(); ///# 부모 클래스의 소멸자가 virtual이면, 자식도 반드시 virtual... 졸 중요!
};

