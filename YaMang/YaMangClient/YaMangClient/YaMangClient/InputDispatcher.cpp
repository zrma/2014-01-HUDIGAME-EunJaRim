#include "stdafx.h"
#include "InputDispatcher.h"
#include "KeyInput.h"
#include "GameManager.h"
#include "CameraController.h"
#include "NetworkManager.h"
#include "PacketType.h"
#include "MainWindow.h"


typedef void( *KeyEventHandler )( KeyInput inputKey );
static KeyEventHandler KeyHandlerTable[MAX_KEY];

static void DefaultKeyHandler( KeyInput inputKey )
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
	RegisterKeyHandler( unsigned char keyType, KeyEventHandler keyHandler )
	{
		KeyHandlerTable[keyType] = keyHandler;
	}
};

#define REGISTER_KEY_HANDLER( KEY_TYPE ) \
	static void Handler_##KEY_TYPE( KeyInput inputKey ); \
	static RegisterKeyHandler _register_##KEY_TYPE(KEY_TYPE, Handler_##KEY_TYPE); \
	static void Handler_##KEY_TYPE( KeyInput inputKey )

InputDispatcher::InputDispatcher()
{
	ZeroMemory( &m_IsKeyPressed, sizeof(m_IsKeyPressed) );
}


InputDispatcher::~InputDispatcher()
{
}

void InputDispatcher::EventKeyInput( KeyInput key )
{
	switch ( key.GetKeyStatus() )
	{
		case KeyStatus::KEY_DOWN:
		{
			m_KeyInputList.push_back( key );
			m_IsKeyPressed[key.GetKeyValue()] = true;
		}
			break;
		case KeyStatus::KEY_PRESSED:
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
		if ( IsPressed(*(iter)) )
		{
			KeyHandlerTable[iter->GetKeyValue()]( *(iter) );
			( iter++ )->SetKeyStatus( KeyStatus::KEY_PRESSED );
		}
		else
		{
			( iter )->SetKeyStatus( KeyStatus::KEY_UP );
			KeyHandlerTable[iter->GetKeyValue()]( *(iter) );

			m_KeyInputList.erase(iter);
			break;
		}
	}
}


void NetworkManager::RequestChat( ChatBroadcastRequest& outPacket )
{
	outPacket.m_PlayerId = m_MyPlayerId;

	if ( m_SendBuffer.Write( (const char*)&outPacket, outPacket.m_Size ) )
	{
		PostMessage( MainWindow::GetInstance()->Window(), WM_SOCKET, NULL, FD_WRITE );
		//////////////////////////////////////////////////////////////////////////
		// http://blog.naver.com/gkqxhq324456/110177315036 참조
		//
		// 채팅을 날리려고 버퍼에 데이터도 넣어 두었으니, WM_SOCKET 이벤트를 발생시키자
		//////////////////////////////////////////////////////////////////////////
	}
}

REGISTER_KEY_HANDLER( VK_UP )
{
	switch ( inputKey.GetKeyStatus() )
	{
		case KeyStatus::KEY_DOWN:
			Log( "Up키 눌렀다! \n" );
			break;
		case KeyStatus::KEY_PRESSED:
			Log( "Up키 누르고 있다! \n" );
			break;
		case KeyStatus::KEY_UP:
			Log( "Up키 뗐다! \n" );
			break;
		default:
			break;
	}
}

REGISTER_KEY_HANDLER( VK_DOWN )
{
}

REGISTER_KEY_HANDLER( VK_LEFT )
{
}

REGISTER_KEY_HANDLER( VK_RIGHT )
{
// 	ChatBroadcastRequest reqPacket;
// 	strcpy_s( reqPacket.m_Chat, "IsMoveRightOK" );
// 
// 	NetworkManager::GetInstance()->RequestChat( reqPacket );
}

REGISTER_KEY_HANDLER( VK_SPACE )
{
	CameraController::GetInstance()->Init();
}

REGISTER_KEY_HANDLER( VK_ESCAPE )
{
	GameManager::GetInstance()->Stop();
}

REGISTER_KEY_HANDLER( VK_W )
{
	CameraController::GetInstance()->MoveForward( 0.3f );
}

REGISTER_KEY_HANDLER( VK_S )
{
	CameraController::GetInstance()->MoveForward( -0.3f );
}

REGISTER_KEY_HANDLER( VK_A )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_TRANSLATE_X, 10.0 );
}

REGISTER_KEY_HANDLER( VK_D )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_TRANSLATE_X, -10.0f );
}

REGISTER_KEY_HANDLER( VK_Q )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_ROTATE_Y, 0.05f );
}

REGISTER_KEY_HANDLER( VK_E )
{
	CameraController::GetInstance()->TransCamera( TransType::TRANS_TYPE_ROTATE_Y, -0.05f );
}

REGISTER_KEY_HANDLER( VK_R )
{
	CameraController::GetInstance()->AddAxis( 0.02f );
}

REGISTER_KEY_HANDLER( VK_F )
{
	CameraController::GetInstance()->AddAxis( -0.02f );
}

REGISTER_KEY_HANDLER( VK_C )
{
	CameraController::GetInstance()->AddHeight( -10.0f );
}

REGISTER_KEY_HANDLER( VK_V )
{
	CameraController::GetInstance()->AddHeight( 10.0f );
}