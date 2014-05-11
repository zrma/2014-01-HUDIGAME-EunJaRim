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
#include "MouseRender.h"
#include "TextManager.h"

#include "CollisionManager.h"
#include "PlayerManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

//////////////////////////////////////////////////////////////////////////
// 게임의 자원 초기화는 이곳에서
//////////////////////////////////////////////////////////////////////////
bool GameManager::Init() const
{
	//////////////////////////////////////////////////////////////////////////
	// 하단의 코드들은 방어코드가 필요합니다.
	//////////////////////////////////////////////////////////////////////////

	// 렌더러 생성 및 초기화
	Renderer::GetInstance()->Init();

	// 씬 생성 및 초기화
	SceneManager::GetInstance()->Init();
	SceneManager::GetInstance()->ChangeScene( SCENE_PLAY );

	// 네트워크 생성 및 초기화
	NetworkManager::GetInstance()->Init();
	NetworkManager::GetInstance()->Connect();
	
	// 리소스 로딩 및 초기화
	ResourceManager::GetInstance()->Init();
	
	// 카메라 초기화
	CameraController::GetInstance()->Init();

	// 텍스트 매니저
	// 타이머
	// 플레이어

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 업데이트 부분
//////////////////////////////////////////////////////////////////////////
bool GameManager::Process() const
{
	if ( !m_Continue )
	{
		return false;
	}
	// 업데이트

	Timer::GetInstance()->Tick();
	InputDispatcher::GetInstance()->DispatchKeyInput();
	NetworkManager::GetInstance()->ProcessPacket();

	CameraController::GetInstance()->Update();
	SceneManager::GetInstance()->Update();

	// 더미 렌더
	{
		DummyRender dRender;

		Renderer::GetInstance()->RenderMap();
		SceneManager::GetInstance()->Render();

		UINT deltaTime = Timer::GetInstance()->GetElapsedTime();
		wchar_t ws[100] = { 0, };
		wsprintf(ws, L"Elapsed : %d", deltaTime);
		TextManager::GetInstance( )->RegistText( TEXT_FPS, ws, 20, 20 ); // 애 었다 치워야 될것같은데... 것보다 key값을 잘못잡았나. 역시 배열인가...
		TextManager::GetInstance()->DrawTexts();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 자원 해제는 이곳에서 합니다.
//////////////////////////////////////////////////////////////////////////
void GameManager::Destroy() const
{
	// 텍스트 매니저 해제
	TextManager::Release();

	//마우스 렌더 객체 씬이 상속하도록 바꿈
// 	MouseRender::GetInstance()->CleanupCursor();
// 	MouseRender::Release();

	// 네트워크에서 씬을 참조하는 부분이 있으므로 씬을 먼저 해제하면 안 됨
	NetworkManager::GetInstance()->Destroy();
	NetworkManager::Release();

	// 씬 해제
	SceneManager::GetInstance()->Destroy();
	SceneManager::Release();

	// 충돌 체크 매니저는 씬매니저보다 나중에 제거
	CollisionManager::GetInstance()->ClearList();
	CollisionManager::Release();

	// 리소스 해제
	ResourceManager::GetInstance()->Destroy();
	ResourceManager::Release();

	// 렌더러 해제
	Renderer::GetInstance()->Destroy();
	Renderer::Release();

	CameraController::Release();
	InputDispatcher::Release();

	Timer::Release();
	PlayerManager::Release();
}
