#include "stdafx.h"
#include "InputDispatcher.h"
#include "KeyInput.h"
#include "GameManager.h"

extern float myPosX;
extern float myPosY;
extern float myPosZ;

typedef void( *KeyEventHandler )( const KeyInput* inputKey );
static KeyEventHandler KeyHandlerTable[MAX_KEY];

static void DefaultKeyHandler( const KeyInput* inputKey )
{
}

struct InitializeKeyHandlers
{
	InitializeKeyHandlers()
	{
		for ( int i = 0; i < MAX_KEY; ++i )
		{
			KeyHandlerTable[i] = DefaultKeyHandler;
		}
	}
} _init_key_handlers_;

struct RegisterKeyHandler
{
	RegisterKeyHandler( char keyType, KeyEventHandler keyHandler )
	{
		KeyHandlerTable[keyType] = keyHandler;
	}
};

#define REGISTER_KEY_HANDLER( KEY_TYPE ) \
	static void Handler_##KEY_TYPE( const KeyInput* inputKey ); \
	static RegisterKeyHandler _register_##KEY_TYPE(KEY_TYPE, Handler_##KEY_TYPE); \
	static void Handler_##KEY_TYPE( const KeyInput* inputKey )

InputDispatcher::InputDispatcher()
{
}


InputDispatcher::~InputDispatcher()
{
}

void InputDispatcher::DispatchKeyInput()
{
	while ( m_KeyInputList.size() > 0 )
	{
		KeyInput inputKey = *( m_KeyInputList.rbegin() );
		m_KeyInputList.pop_front();

		Log( "이번 입력은 %c입니다. \n", inputKey.GetKeyValue() );

		KeyHandlerTable[inputKey.GetKeyValue()]( &inputKey );
	}
}

REGISTER_KEY_HANDLER( VK_W )
{
	myPosZ += 0.5;
}

REGISTER_KEY_HANDLER( VK_S )
{
	myPosZ -= 0.5;
}

REGISTER_KEY_HANDLER( VK_A )
{
	myPosX -= 0.5;
}

REGISTER_KEY_HANDLER( VK_D )
{
	myPosX += 0.5;
}

REGISTER_KEY_HANDLER( VK_ESCAPE )
{
	GameManager::GetInstance()->Stop();
}