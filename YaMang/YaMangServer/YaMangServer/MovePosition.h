#pragma once
#include "Action.h"
class MovePosition:
	public Action
{
public:
	MovePosition();
	~MovePosition();

private:
	void OnBegin(); ///# 왜 갑자기 private로 바꾸고...  virtual은 앞에 안붙여줌?
	void OnTick();
	void OnEnd();
};

