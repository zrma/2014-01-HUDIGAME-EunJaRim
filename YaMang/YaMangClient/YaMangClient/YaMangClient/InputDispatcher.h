#pragma once
#include "MacroSet.h"
#include "KeyInput.h"

class MouseInput;

class InputDispatcher : public Singleton<InputDispatcher>
{
public:
	InputDispatcher();
	~InputDispatcher();

	void	EventKeyInput( KeyInput key );
	void	DispatchKeyInput();

	bool	IsPressed( KeyInput key ) { return IsPressed( key.GetKeyValue() ); }
	bool	IsPressed( unsigned char key ) { return m_IsKeyPressed[key]; }

	void	EventMouseMove(int XMove = 0, int YMove = 0);

private:
	std::list<KeyInput>			m_KeyInputList;
	std::array<bool, MAX_KEY>	m_IsKeyPressed;

	MouseInput*					m_MouseInput;
};