#pragma once

enum class KeyStatus
{
	KEY_NONE,
	KEY_DOWN,
	KEY_PRESSED,
	KEY_UP,
};

const char VK_A = 0x41;
const char VK_B = 0x42;
const char VK_C = 0x43;
const char VK_D = 0x44;
const char VK_E = 0x45;
const char VK_F = 0x46;
const char VK_G = 0x47;
const char VK_H = 0x48;
const char VK_I = 0x49;
const char VK_J = 0x4A;
const char VK_K = 0x4B;
const char VK_L = 0x4C;
const char VK_M = 0x4D;
const char VK_N = 0x4E;
const char VK_O = 0x4F;
const char VK_P = 0x50;
const char VK_Q = 0x51;
const char VK_R = 0x52;
const char VK_S = 0x53;
const char VK_T = 0x54;
const char VK_U = 0x55;
const char VK_V = 0x56;
const char VK_W = 0x57;
const char VK_X = 0x58;
const char VK_Y = 0x59;
const char VK_Z = 0x5A;

const int MAX_KEY = 256;

class KeyInput
{
public:
	KeyInput();
	~KeyInput();

	char		GetKeyValue() { return m_KeyValue; }
	void		SetKeyValue( char key ) { m_KeyValue = key; }
	KeyStatus	GetKeyStatus() { return m_KeyStatus; }
	void		SetKeyStatus( KeyStatus keyStatus ) { m_KeyStatus = keyStatus; }

private:
	char		m_KeyValue = 0;
	KeyStatus	m_KeyStatus = KeyStatus::KEY_NONE;
};
