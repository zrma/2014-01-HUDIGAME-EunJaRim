#pragma once
#include "MacroSet.h"
#include "KeyInput.h"

class InputDispatcher : public Singleton<InputDispatcher>
{
public:
	InputDispatcher();
	~InputDispatcher();

	void DispatchKeyInput();

private:
	std::list<KeyInput>	m_KeyInputList;
	bool				m_IsKeyPressed[MAX_KEY];

	friend class MainWindow;
};