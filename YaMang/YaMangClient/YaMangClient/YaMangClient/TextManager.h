#pragma once
#include "MacroSet.h"

class TextObject;
class TextManager:public Singleton<TextManager>
{
public:
	TextManager();
	~TextManager();

	void	RegistText( UINT key, LPCWSTR text, float left, float top, int RGB_R = 255, int RGB_G = 255, int RGB_B = 255, float right = 0, float bottom = 0 );
	void	DeleteText( UINT key );

	void	DrawTexts() const;

private:
	// enum으로 텍스트 종류들 뽑고 거기다 배열로 하는게 좋을것 같긴한데 개발중 편하게 넣고 쓰기에는 map형태가 좋을듯
	typedef std::map<UINT, TextObject*> TextList;
	TextList			m_TextList;
};

