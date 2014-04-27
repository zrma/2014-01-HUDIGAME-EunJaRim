#pragma once

enum class MouseStatus
{
	MOUSE_NONE,
	MOUSE_DOWN,
	MOUSE_PRESSED,
	MOUSE_UP,
};

class MouseInput
{
public:
	MouseInput();
	~MouseInput();

	POINT			GetMousePosition() { return m_MousePosition; }
	void			SetMousePosition(POINT Position) { m_MousePosition = Position; }
	void			SetMousePosition(int xPos, int yPos) { m_xPos = xPos; m_yPos = yPos; }
	MouseStatus		GetMouseStatus() { return m_MouseStatus; }
	void			SetMouseStatus(MouseStatus mouseStatus) { m_MouseStatus = mouseStatus; }

private:
	int m_xPos = 200;
	int m_yPos = 200;
	POINT			m_MousePosition = { m_xPos, m_yPos };
	unsigned char	m_KeyValue = 0;
	MouseStatus		m_MouseStatus = MouseStatus::MOUSE_NONE;

};

