#include "stdafx.h"
#include "GameManager.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "NetworkManager.h"
#include "DummyRender.h"
#include "InputDispatcher.h"
#include "CameraController.h"
#include "Timer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "MouseManager.h"
#include "TextManager.h"


GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

//////////////////////////////////////////////////////////////////////////
// 게임의 자원 초기화는 이곳에서
//////////////////////////////////////////////////////////////////////////
bool GameManager::Init()
{
	//////////////////////////////////////////////////////////////////////////
	// 하단의 코드들은 방어코드가 필요합니다.
	//////////////////////////////////////////////////////////////////////////

	NetworkManager::GetInstance()->Init();
	NetworkManager::GetInstance()->Connect();

	// 렌더러 생성 및 초기화
	Renderer::GetInstance()->Init();
	
	// 리소스 로딩 및 초기화
	ResourceManager::GetInstance()->Init();
	
	// 씬 생성 및 초기화
	SceneManager::GetInstance()->Init();
	SceneManager::GetInstance()->ChangeScene( SCENE_PLAY );

	// 카메라 초기화
	CameraController::GetInstance()->Init();

	// 커서 초기화
	MouseManager::GetInstance()->CreateCursor( L"cursor2.png", 200, 200);
	MouseManager::GetInstance()->RemoveWndCursor();
	// 텍스트 매니저
	// 타이머

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 업데이트 부분
//////////////////////////////////////////////////////////////////////////
bool GameManager::Process()
{
	if ( !m_Continue )
	{
		return false;
	}
	// 업데이트

	Timer::GetInstance()->Tick();
	InputDispatcher::GetInstance()->DispatchKeyInput();
	NetworkManager::GetInstance()->ProcessPacket();
	
	SceneManager::GetInstance()->Update();

	// 더미 렌더
	DummyRender dRender;

	Renderer::GetInstance()->RenderMap();
	SceneManager::GetInstance()->Render();

	UINT deltaTime = Timer::GetInstance()->GetElapsedTime();
	wchar_t ws[100] = { 0, };
	wsprintf( ws, L"Elapsed : %d", deltaTime );
	//Renderer::GetInstance()->WriteText( ws, 20, 20 );
	TextManager::GetInstance()->RegistText( 100, ws, 20, 20 ); // 애 었다 치워야 될것같은데... 것보다 key값을 잘못잡았나. 역시 배열인가...
	TextManager::GetInstance()->DrawTexts();

	MouseManager::GetInstance()->RenderCursor();

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 자원 해제는 이곳에서 합니다.
//////////////////////////////////////////////////////////////////////////
void GameManager::Destroy()
{
	// 텍스트 매니저 해제
	TextManager::Release();

	MouseManager::GetInstance()->CleanupCursor();
	MouseManager::Release();

	// 씬 해제
	SceneManager::GetInstance()->Destroy();
	SceneManager::Release();

	// 리소스 해제
	ResourceManager::GetInstance()->Destroy();
	ResourceManager::Release();

	// 렌더러 해제
	Renderer::GetInstance()->Destroy();
	Renderer::Release();

	NetworkManager::GetInstance()->Destroy();
	NetworkManager::Release();

	CameraController::Release();
	InputDispatcher::Release();

	Timer::Release();
}
