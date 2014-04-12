#pragma once

enum class KeyEventType
{
	KEY_DOWN,
	KEY_PRESSED,
	KEY_UP,
	KEY_UNUSED,
};

class KeyInput
{
public:
	KeyInput();
	~KeyInput();

private:
	KeyEventType m_KeyEventType;
};

