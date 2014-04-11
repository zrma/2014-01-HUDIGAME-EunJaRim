#include "stdafx.h"
#include "GameManager.h"
#include "../../yaMangDxDll/yaMangDxDll/yaMangDxDll.h"
#include "Renderer.h"
#include "NetworkManager.h"

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

	// 씬 생성 및 초기화

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 업데이트 부분
//////////////////////////////////////////////////////////////////////////
bool GameManager::Update()
{
	if ( !m_Continue )
	{
		return false;
	}
	// 업데이트

	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 렌더링 부분
//////////////////////////////////////////////////////////////////////////
bool GameManager::Render()
{
	if ( !m_Continue )
	{
		return false;
	}

	// 렌더 비긴
	Renderer::GetInstance()->RenderBegin();

	if ( m_Mesh )
	{
		Renderer::GetInstance()->Render( m_Mesh );
	}

	// 렌더 엔드
	Renderer::GetInstance()->RenderEnd();

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
}
