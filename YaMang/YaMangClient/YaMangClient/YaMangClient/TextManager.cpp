#include "stdafx.h"
#include "TextManager.h"
#include "TextObject.h"

TextManager::TextManager()
{
	for ( auto& it : m_TextList )
	{
		TextObject* textOBJ = new TextObject( L"", -9999.0f, -9999.0f, 255, 255, 255, 0, 0 );
		it = textOBJ;
	}
}


TextManager::~TextManager()
{
	for ( auto& it : m_TextList )
	{
		TextObject* toBeDelete = it;
		SafeDelete( toBeDelete );
	}
}

void TextManager::RegistText( TextType key, LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom )
{
	TextObject* textOBJ = m_TextList[key];
	textOBJ->SetText( text );
	textOBJ->SetLeft( left );
	textOBJ->SetTop( top );
	textOBJ->SetColorR( RGB_R );
	textOBJ->SetColorG( RGB_G );
	textOBJ->SetColorB( RGB_B );
	textOBJ->SetRight( right );
	textOBJ->SetBottom( bottom );

}

void TextManager::DeleteText( TextType key )
{
	TextObject* textOBJ = m_TextList[key];
	textOBJ->SetText( L"" );
	textOBJ->SetLeft( -9999.0f );
	textOBJ->SetTop( -9999.0f );
}

void TextManager::DrawTexts() const
{
	for ( auto& it : m_TextList )
	{
		TextObject* textOBJ = it;
		textOBJ->DrawText();
	}
}
