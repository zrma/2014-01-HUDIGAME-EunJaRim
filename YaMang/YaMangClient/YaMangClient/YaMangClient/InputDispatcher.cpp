/**
*@file		InputDispatcher.cpp
*@brief		키와 마우스 이벤트를 지속성 있게 관리해줍니다.
*@details	MainWindow의 객체의 윈도우 메시지 핸들링에서 키와 마우스 관련 메시지를 받아 이벤트 분석합니다.
*			키 입력이 초기 한 번 들어오면 대략 1초의 딜레이 후 연속 발생하는데,
			해당 부분을 해결하기 위해 처음 한 번 눌렸을 때와 손을 뗄 때의 사이에 눌리고 있음을
			가상으로 만들어서 지속적으로 핸들링 함으로써 부드러운 키 입력을 구현합니다.
*/

#include "stdafx.h"
#include "InputDispatcher.h"
#include "KeyInput.h"
#include "GameManager.h"
#include "CameraController.h"
#include "NetworkManager.h"
#include "PacketType.h"
#include "MainWindow.h"
#include "Timer.h"


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
	m_IsKeyPressed.fill( false );
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
	// Log("현재 키 이벤트 발생 개수 : %d \n", m_KeyInputList.size());

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

void InputDispatcher::ClearList()
{
	m_IsKeyPressed.fill( false );
	m_KeyInputList.clear();
}

REGISTER_KEY_HANDLER( VK_SPACE )
{
	switch ( inputKey.GetKeyStatus() )
	{
		case KeyStatus::KEY_DOWN:
		{
			CameraController::GetInstance()->Init();
			// test code 
			// 서버 상황 동기화를 위한 테스트 코드
			// 원래는 방만들고 방시작하고 게임진행하면 필요없음 

			SyncAllRequest syncRequestData;
			syncRequestData.m_PlayerId = NetworkManager::GetInstance( )->GetMyPlayerID();
			syncRequestData.m_RoomNumber = 0; // 로비 번호... 테스트용

			NetworkManager::GetInstance()->SendPacket( &syncRequestData );
		}
			break;
		case KeyStatus::KEY_PRESSED:
			// Log( "** 키 누르고 있다! ** \n" );
			break;
		case KeyStatus::KEY_UP:
		{
		}
			break;
		default:
			break;
	}
}

REGISTER_KEY_HANDLER( VK_ESCAPE )
{
	GameManager::GetInstance()->Stop();
}

REGISTER_KEY_HANDLER( VK_W )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 5.0f;
	CameraController::GetInstance()->MoveForward( delta );

//	Log( "Time : %f \n", delta );
}

REGISTER_KEY_HANDLER( VK_S )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -5.0f;
	CameraController::GetInstance()->MoveForward( delta );
}

REGISTER_KEY_HANDLER( VK_A )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -5.0f;
	CameraController::GetInstance()->MoveSide( delta );
}

REGISTER_KEY_HANDLER( VK_D )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 5.0f;
	CameraController::GetInstance()->MoveSide( delta );
}

REGISTER_KEY_HANDLER( VK_Q )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -0.5f;
	CameraController::GetInstance()->RotateSide( delta );
}

REGISTER_KEY_HANDLER( VK_E )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 0.5f;
	CameraController::GetInstance()->RotateSide( delta );
}

REGISTER_KEY_HANDLER( VK_R )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -0.5f;
	CameraController::GetInstance()->RotateUp( delta );
}

REGISTER_KEY_HANDLER( VK_F )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 0.5f;
	CameraController::GetInstance()->RotateUp( delta );
}

REGISTER_KEY_HANDLER( VK_C )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 5.0f;
	CameraController::GetInstance()->MoveElevate( delta );
}

REGISTER_KEY_HANDLER( VK_V )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -5.0f;
	CameraController::GetInstance()->MoveElevate( delta );
}

// Cheat Key
REGISTER_KEY_HANDLER( VK_U )
{
	// generate arrow
	if ( KeyStatus::KEY_UP == inputKey.GetKeyStatus( ) )
	{
		GenerateCorpsRequest generateCorps;
		generateCorps.m_NowX = 40.0f;
		generateCorps.m_NowZ = 40.0f;
		generateCorps.m_LookX = 0.0f;
		generateCorps.m_LookZ = 0.0f;
		generateCorps.m_PlayerId = NetworkManager::GetInstance()->GetMyPlayerID();
		generateCorps.m_UnitType = UnitType::UNIT_ARROW;

		NetworkManager::GetInstance()->SendPacket( &generateCorps );
	}

}
REGISTER_KEY_HANDLER( VK_I )
{
	// generate knight
	if ( KeyStatus::KEY_UP == inputKey.GetKeyStatus() )
	{
		GenerateCorpsRequest generateCorps;
		generateCorps.m_NowX = 40.0f;
		generateCorps.m_NowZ = 40.0f;
		generateCorps.m_LookX = 0.0f;
		generateCorps.m_LookZ = 0.0f;
		generateCorps.m_PlayerId = NetworkManager::GetInstance()->GetMyPlayerID();
		generateCorps.m_UnitType = UnitType::UNIT_KNIGHT;

		NetworkManager::GetInstance()->SendPacket( &generateCorps );
	}
}
REGISTER_KEY_HANDLER( VK_O )
{
	// generate pike
	if ( KeyStatus::KEY_UP == inputKey.GetKeyStatus() )
	{
		GenerateCorpsRequest generateCorps;
		generateCorps.m_NowX = 40.0f;
		generateCorps.m_NowZ = 40.0f;
		generateCorps.m_LookX = 0.0f;
		generateCorps.m_LookZ = 0.0f;
		generateCorps.m_PlayerId = NetworkManager::GetInstance()->GetMyPlayerID();
		generateCorps.m_UnitType = UnitType::UNIT_PIKE;

		NetworkManager::GetInstance()->SendPacket( &generateCorps );
	}
}
REGISTER_KEY_HANDLER( VK_P )
{
	// generate sword
	if ( KeyStatus::KEY_UP == inputKey.GetKeyStatus() )
	{
		GenerateCorpsRequest generateCorps;
		generateCorps.m_NowX = 40.0f;
		generateCorps.m_NowZ = 40.0f;
		generateCorps.m_LookX = 0.0f;
		generateCorps.m_LookZ = 0.0f;
		generateCorps.m_PlayerId = NetworkManager::GetInstance()->GetMyPlayerID();
		generateCorps.m_UnitType = UnitType::UNIT_SWORD;

		NetworkManager::GetInstance()->SendPacket( &generateCorps );
	}
}

REGISTER_KEY_HANDLER( VK_L )
{
	// generate Bot sword
	if ( KeyStatus::KEY_UP == inputKey.GetKeyStatus() )
	{
		GenerateCorpsRequest generateCorps;
		generateCorps.m_NowX = -20.0f;
		generateCorps.m_NowZ = -20.0f;
		generateCorps.m_LookX = 0.0f;
		generateCorps.m_LookZ = 0.0f;
		generateCorps.m_PlayerId = 0; // BOT ID
		generateCorps.m_UnitType = UnitType::UNIT_SWORD;

		NetworkManager::GetInstance()->SendPacket( &generateCorps );
	}
}