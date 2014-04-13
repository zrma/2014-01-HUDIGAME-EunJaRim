#include "stdafx.h"
#include "InputDispatcher.h"
#include "KeyInput.h"
#include "GameManager.h"
#include "CameraController.h"

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
	ZeroMemory( &m_IsKeyPressed, m_IsKeyPressed.size() );
}


InputDispatcher::~InputDispatcher()
{
}

void InputDispatcher::EventKeyInput( KeyInput key )
{
	switch ( key.GetKeyStatus() )
	{
		case KeyStatus::KEY_DOWN:
			m_KeyInputList.push_back( key );

		case KeyStatus::KEY_PRESSED:
		{
			m_IsKeyPressed[key.GetKeyValue()] = true;
		}
			break;
		case KeyStatus::KEY_UP:
		{
			m_IsKeyPressed[key.GetKeyValue()] = false;
		}
			break;
		default:
		{
			assert( false );
		}
	}
}

void InputDispatcher::DispatchKeyInput()
{
	auto iter = m_KeyInputList.begin();
	while ( iter != m_KeyInputList.end() )
	{
		if ( !m_IsKeyPressed[iter->GetKeyValue()] )
		{
			m_KeyInputList.erase( iter++ );
		}
		else
		{
			KeyInput keyInput = *(iter++);
			KeyHandlerTable[keyInput.GetKeyValue()]( &keyInput );
		}
	}
}

REGISTER_KEY_HANDLER( VK_UP )
{
	myPosZ += 0.2f;
}

REGISTER_KEY_HANDLER( VK_DOWN )
{
	myPosZ -= 0.2f;
}

REGISTER_KEY_HANDLER( VK_LEFT )
{
	myPosX -= 0.2f;
}

REGISTER_KEY_HANDLER( VK_RIGHT )
{
	myPosX += 0.2f;
}

REGISTER_KEY_HANDLER( VK_SPACE )
{
	myPosX = 0;
	myPosY = 0;
	myPosZ = 0;
}

REGISTER_KEY_HANDLER( VK_ESCAPE )
{
	GameManager::GetInstance()->Stop();
}

REGISTER_KEY_HANDLER( VK_W )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_TRANSLATE_Z, -1.0f );
}

REGISTER_KEY_HANDLER( VK_S )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_TRANSLATE_Z, 1.0f );
}

REGISTER_KEY_HANDLER( VK_A )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_TRANSLATE_X, -1.0f );
}

REGISTER_KEY_HANDLER( VK_D )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_TRANSLATE_X, 1.0f );
}