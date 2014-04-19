#include "stdafx.h"
#include "GameManager.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "NetworkManager.h"
#include "DummyRender.h"
#include "InputDispatcher.h"
#include "CameraController.h"
#include "Timer.h"



GameManager::GameManager()
:m_Mesh( new MESHOBJECT() )
{
}


GameManager::~GameManager()
{
	SafeDelete( m_Mesh );
}

//////////////////////////////////////////////////////////////////////////
// 게임의 자원 초기화는 이곳에서
//////////////////////////////////////////////////////////////////////////
bool GameManager::Init()
{
	// 렌더러 생성 및 초기화
	Renderer::GetInstance()->Init();
	// 방어코드 필요함

	NetworkManager::GetInstance()->Init();
	// 방어코드 필요함

	NetworkManager::GetInstance()->Connect();

	// 리소스 로딩 및 초기화
	//////////////////////////////////////////////////////////////////////////
	// 테스트용 코드
	LPCTSTR fileName = L"tiger.x";
	Renderer::GetInstance()->CreateMesh( fileName, m_Mesh );

	LPCTSTR mapFileName = L"heightmap_1024_1024_korea.bmp";
	LPCTSTR textureFileName = L"heightmap_texture_1024_1024_korea.bmp";
	Renderer::GetInstance()->CreateMap( mapFileName, textureFileName );

	// 씬 생성 및 초기화

	// 카메라 초기화
	CameraController::GetInstance()->Init();

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
	
	// 더미 렌더
	DummyRender dRender;

	if ( m_Mesh )
	{
		//////////////////////////////////////////////////////////////////////////
		// 여기는 현재 하드코딩으로 렌더를 해 주지만,
		// 추후 씬 디렉터를 추가해서 씬 디렉터가 각 씬을 구성하고,
		// 그 씬 내부에 맵이나, 케릭터 등의 각 객체가 존재해서
		// 업데이트와 렌더를 씬에게 명령을 내리고 씬에서 다시 하부로 명령이 전파 되는 식으로 구현 해야 함
		//////////////////////////////////////////////////////////////////////////

		Renderer::GetInstance()->RenderMap();
		Renderer::GetInstance()->RenderMesh( m_Mesh );
	}

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
	Renderer::GetInstance()->DeleteMesh( m_Mesh );
	// 렌더러 해제
	Renderer::GetInstance()->Destroy();
	Renderer::Release();

	NetworkManager::GetInstance()->Destroy();
	NetworkManager::Release();

	CameraController::Release();
	InputDispatcher::Release();

	Timer::Release();
}
