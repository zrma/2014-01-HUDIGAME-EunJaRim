#include "stdafx.h"
#include "TextManager.h"
#include "TextObject.h"

TextManager::TextManager()
{
}


TextManager::~TextManager()
{
	for ( auto it : m_TextList )
	{
		TextObject* toBeDelete = it.second;
		delete toBeDelete;
	}
	m_TextList.clear();
}

void TextManager::RegistText( LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom )
{
	auto it = m_TextList.find( text );
	if ( it == m_TextList.end() )
	{
		TextObject* textOBJ = new TextObject( text, left, top, RGB_R, RGB_G, RGB_B, right, bottom );
		m_TextList.insert( TextList::value_type( text, textOBJ ) );
	}
}

void TextManager::DeleteText( LPCWSTR text )
{
	auto it = m_TextList.find( text );
	if ( it != m_TextList.end() )
	{
		TextObject* toBeDelete = it->second;
		delete toBeDelete;
		m_TextList.erase( text );
	}
}

void TextManager::DrawTexts()
{
	for ( auto it : m_TextList )
	{
		TextObject* textOBJ = it.second;
		textOBJ->DrawText();
	}
}
