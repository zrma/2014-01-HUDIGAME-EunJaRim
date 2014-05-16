#include "stdafx.h"
#include "UIObject.h"
#include "YaMangDxDll.h"


UIObject::UIObject()
{
}


UIObject::~UIObject()
{
}

void UIObject::Update()
{

}

void UIObject::SetUIPosition( int m_UIType, int x, int y )
{
	SetUIPos(m_UIType, x, y);
}
