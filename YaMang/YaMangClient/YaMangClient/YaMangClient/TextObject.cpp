#include "stdafx.h"
#include "TextObject.h"
#include "Renderer.h"

TextObject::TextObject( LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom )
{
	m_Text = text; // copy로?
	m_Left = left;
	m_Top = top;
	m_RGB_R = RGB_R;
	m_RGB_G = RGB_G;
	m_RGB_B = RGB_B;
	m_Right = right;
	m_Bottom = bottom;
}


TextObject::~TextObject()
{
}

void TextObject::DrawText()
{
	Renderer::GetInstance( )->WriteText( m_Text, m_Left, m_Top, m_RGB_R, m_RGB_G, m_RGB_B, m_Right, m_Bottom );
}
