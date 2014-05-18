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

void UIObject::Render() const
{
	RenderUI(m_UISpriteType, m_UIPosition);
}

void UIObject::SetUIPosition( int x, int y )
{
	SetUIPosX(x);
	SetUIPosY(y);
}
