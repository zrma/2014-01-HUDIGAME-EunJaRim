#include "stdafx.h"
#include "MouseInput.h"
#include "YaMangDxDll.h"


MouseInput::MouseInput()
{
}


MouseInput::~MouseInput()
{
}

void MouseInput::SetCursorPos(float PosX, float PosY)
{
	SetCursorPosition(PosX, PosY);
}
