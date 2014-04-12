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
	int				m_KeyValue = 0;
	KeyEventType	m_KeyEventType = KeyEventType::KEY_UNUSED;
};

