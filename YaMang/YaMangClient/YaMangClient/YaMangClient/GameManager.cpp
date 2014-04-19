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
	NetworkManager::GetInstance()->Init();
	// 방어코드 필요함

	NetworkManager::GetInstance()->Connect();

	// 렌더러 생성 및 초기화
	Renderer::GetInstance()->Init();
	// 방어코드 필요함


	// 리소스 로딩 및 초기화
	ResourceManager::GetInstance()->Init();
	
	// 씬 생성 및 초기화
	SceneManager::GetInstance()->Init();
	SceneManager::GetInstance()->ChangeScene( SCENE_PLAY );

	// 카메라 초기화
	CameraController::GetInstance()->Init();

	//텍스트 렌더러 초기화
	Renderer::GetInstance()->CreateTextRenderer();

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
	SceneManager::GetInstance()->Render();

	UINT deltaTime = Timer::GetInstance()->GetElapsedTime();
	wchar_t ws[100] = { 0, };
	wsprintf( ws, L"Elapsed : %d", deltaTime );
	Renderer::GetInstance()->WriteText( ws, 20, 20 );
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 자원 해제는 이곳에서 합니다.
//////////////////////////////////////////////////////////////////////////
void GameManager::Destroy()
{
	// 씬 해제

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

	Renderer::GetInstance()->DeleteTextRenderer();

	Timer::Release();
}
