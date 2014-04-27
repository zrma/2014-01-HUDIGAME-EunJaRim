#include "stdafx.h"
#include "YamangServer.h"
#include "PacketType.h"
#include "ClientSession.h"
#include "ClientManager.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"
#include "RoomManager.h"
#include "Arrow.h"
#include "Guard.h"
#include "knight.h"
#include "Pike.h"
#include "Sword.h"
#include "tinyxml.h"
#include "xpath_static.h"
#include "ActionScheduler.h"


ClientManager::ClientManager( int roomNumber ): m_RoomNumber( roomNumber ), m_LastGCTick( 0 ), m_LastClientWorkTick( 0 )
{

	// 액션 스케쥴러 실행
	m_ActionScheduler = new ActionScheduler( this );
}


ClientManager::~ClientManager()
{
	for ( auto& it : m_ClientList )
	{
		ClientSession* toBeDelete = it.second;
		delete toBeDelete;
	}
	for ( auto& it : m_CorpsList )
	{
		Corps* toBeDelete = it.second;
		delete toBeDelete;
	}

	for( int i = 0; i < m_Map.size( ); ++i )
	{
		m_Map.at( i ).clear();
	}
	m_Map.clear();

	delete m_ActionScheduler;
}

void ClientManager::GameStart()
{
	TiXmlDocument document = TiXmlDocument( "../../SharedPreference/ServerConfig.xml" );
	bool xmlLoadSuccess = document.LoadFile();

	if ( xmlLoadSuccess )
	{
		std::string mapFilePath;
		mapFilePath = TinyXPath::S_xpath_string( document.RootElement( ), "/server/mapFilePath/text()" ).c_str( );
		printf_s( "Map Path Loaded! :%s \n", mapFilePath.c_str( ) );
		if ( ReadMapFile( mapFilePath.c_str( ) ) )
		{
			m_IsGameStart = true;
		}
		else
		{
			m_IsGameStart = false;
		}

	}
	else
	{
		printf_s( "Map Path Load Fail! \n" );
		m_IsGameStart = false;
	}

	
	
}


// 클라 생성
// _tmain() 쪽의 클라이언트 핸들링 스레드에서 WaitForSingleObjectEx(hEvent, INFINITE, TRUE) 가
// 이벤트를 발생하기를 기다려서 이벤트 발생 신호가 오면(클라이언트 쪽에서 접속 요청하면)
// 전역 변수 g_AcceptedSocket에 담겨 있던 소켓 데이터를 인자로 넘김
ClientSession* ClientManager::CreateClient( SOCKET sock )
{
	// 이 함수는 클라이언트 핸들링 스레드에서 불려야 한다.
	assert( LThreadType == THREAD_CLIENT );

	// 클라이언트 세션을 생성
	ClientSession* client = new ClientSession( sock );

	// 클라이언트 리스트에 소켓 데이터와 세션을 저장
	// 키는 소켓, 밸류는 클라이언트 세션의 포인터인 맵 자료구조
	// typedef pair<const Key, Type> value_type;
	// value_type은 키와 밸류를 한 쌍으로 묶어주는 자료구조
	m_ClientList.insert( ClientList::value_type( sock, client ) );
	client->SetClientManager( this );

	return client;
}


void ClientManager::InputClient( ClientSession* client )
{
	m_ClientList.insert( ClientList::value_type( client->m_Socket, client ) );
	client->SetClientManager( this );
}


void ClientManager::BroadcastPacket( ClientSession* from, PacketHeader* pkt )
{
	///FYI: C++ STL iterator 스타일의 루프
	// 연산자 오버로딩 된 ++ 연산자를 이용해서 차례대로 순회
	for ( ClientList::const_iterator it = m_ClientList.begin(); it != m_ClientList.end(); ++it )
	{
		// ClientList는 맵이므로 it(iterator)는 해당 pair의 포인터이다.
		// 그러므로 it->second 는 밸류(클라이언트 세션)
		ClientSession* client = it->second;

		// 보낸 이에게는 패스
		if ( from == client )
		{
			continue;
		}

		client->SendRequest( pkt );
	}
}


bool ClientManager::DirectPacket( int pid, PacketHeader* pkt )
{
	// 뭔가 이상하지만 어떻게 바꿔볼려고 해도 안됨...
	auto it = g_PidSessionTable.find( pid );
	if ( it != g_PidSessionTable.end( ) )
	{
		ClientSession* client = it->second;
		client->SendRequest( pkt );
		return true;
	}

	return false;
	
}


// EasyServer.cpp의 클라이언트 핸들링 스레드에서 0.1초마다 콜백
//
// 0.1초마다 주기적으로 해야 할 일
//
// 1. 가비지 컬렉팅
// 2. 클라이언트 세션 별 할 일 처리
// 3. DB 작업 처리 된 것 각각 클라이언트에 맞게 적용
void ClientManager::OnPeriodWork()
{
	/// 접속이 끊긴 세션들 주기적으로 정리 (1초 정도 마다 해주자)
	DWORD currTick = GetTickCount();
	if ( currTick - m_LastGCTick >= 1000 )
	{
		CollectGarbageSessions();
		m_LastGCTick = currTick;
	}

	/// 접속된 클라이언트 세션별로 주기적으로 해줘야 하는 일 (주기는 알아서 정하면 됨 - 지금은 1초로 ㅎㅎ)
	if ( currTick - m_LastClientWorkTick >= 1000 )
	{
		ClientPeriodWork();
		m_LastClientWorkTick = currTick;
	}


	// 방을 만들어야지만 실행됨 로비에서는 불가
	#ifndef DEBUG
		if ( m_IsGameStart )
	#endif // DEBUG
	{
		// 0.01초마다 타이머 확인 
		if ( currTick - m_LastClientWorkTick >= 10 )
		{
			m_ActionScheduler->DoScheduledAction();
		}
	}


	/// 처리 완료된 DB 작업들 각각의 Client로 dispatch
	DispatchDatabaseJobResults();
}

void ClientManager::CollectGarbageSessions()
{
	std::vector<ClientSession*> disconnectedSessions;

	///FYI: C++ 11 람다를 이용한 스타일
	std::for_each( m_ClientList.begin(), m_ClientList.end(),
				   [&]( ClientList::const_reference it )
	// Call by Reference(&)를 이용해서 이터레이터 람다 사용, 아래 함수 실행
	{
		ClientSession* client = it.second;

		if ( false == client->IsConnected() && false == client->DoingOverlappedOperation() )
		{
			disconnectedSessions.push_back( client );
		}

	}
	);


	///FYI: C언어 스타일의 루프
	for ( size_t i = 0; i < disconnectedSessions.size(); ++i )
	{
		// vector 자료구조에 대해서 연산자 오버로딩 된 [] 연산자를 통해 배열처럼 사용
		ClientSession* client = disconnectedSessions[i];

		// 우선 클라이언트 리스트에서 해당 목록을 지운다
		m_ClientList.erase( client->m_Socket );
		delete client;
	}

}

// 클라이언트 세션 별로 주기적으로 할 일
void ClientManager::ClientPeriodWork()
{
	// 실행이 안된 룸막기... 근데 Tick이 없어질텐데...
	if ( !m_IsGameStart )
	{
		return;
	}

	// 쓰게 될까? 일단은 남겨둠...
	/// FYI: C++ 11 스타일의 루프
	for ( auto& it : m_ClientList )
	{
		ClientSession* client = it.second;
		client->OnTick();
	}
}

void ClientManager::DispatchDatabaseJobResults()
{
	/// 쌓여 있는 DB 작업 처리 결과들을 각각의 클라에게 넘긴다
	// 추상 클래스 포인터 선언 및 초기화
	DatabaseJobContext* dbResult = nullptr;

	// DB Job 매니저에서 계속 Pop
	while ( g_DatabaseJobManager->PopDatabaseJobResult( dbResult ) )
	{
		if ( false == dbResult->m_Success )
		{
			printf( "DB JOB FAIL \n" );
		}
		else
		{

			//////////////////////////////////////////////////////////////////////////
			// typeid 를 사용하기 위해 typeinfo.h 인클루드
			// stdafx.h 에 #include <typeinfo> 되어 있음
			//
			// class의 타입을 알기 위한 키워드
			//
			// DatabaseJobContext가 추상 클래스이므로 해당 클래스를 상속 받은 하위 클래스의
			// 타입에 따라서 수행해야 할 함수들을 다르게 지정 할 수 있음
			//////////////////////////////////////////////////////////////////////////

			if ( typeid( *dbResult ) == typeid( CreatePlayerDataContext ) )
			{
				CreatePlayerDone( dbResult );
			}
			else if ( typeid( *dbResult ) == typeid( DeletePlayerDataContext ) )
			{
				DeletePlayerDone( dbResult );
			}
			else
			{
				/// 여기는 해당 DB요청을 했던 클라이언트에서 직접 해줘야 는 경우다
				auto& it = m_ClientList.find( dbResult->m_SockKey );

				if ( it != m_ClientList.end() && it->second->IsConnected() )
				{
					/// dispatch here....
					it->second->DatabaseJobDone( dbResult );
				}
			}
		}


		/// 완료된 DB 작업 컨텍스트는 삭제해주자
		DatabaseJobContext* toBeDelete = dbResult;
		delete toBeDelete;
	}
}

void ClientManager::FlushClientSend()
{
	for ( auto& it : m_ClientList )
	{
		ClientSession* client = it.second;
		if ( false == client->SendFlush() )
		{
			client->Disconnect();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// DB쪽에 요청 처리하는 이해를 돕기 위한 주석
// 
// Push Request / Pop Result 로 구성 되어 있음
// DatabaseJobManager.cpp 참조
//////////////////////////////////////////////////////////////////////////
void ClientManager::DBCreatePlayer( int pid, double x, double y, double z, const char* name, const char* comment )
{
	CreatePlayerDataContext* newPlayerJob = new CreatePlayerDataContext();
	newPlayerJob->m_PlayerId = pid;
	newPlayerJob->m_PosX = x;
	newPlayerJob->m_PosY = y;
	newPlayerJob->m_PosZ = z;
	strcpy_s( newPlayerJob->m_PlayerName, name );
	strcpy_s( newPlayerJob->m_Comment, comment );

	g_DatabaseJobManager->PushDatabaseJobRequest( newPlayerJob );

}

//////////////////////////////////////////////////////////////////////////
// 삭제 컨텍스트를 만들어서 해당 객체를 Push Request 함수를 이용해서 처리 요청
//////////////////////////////////////////////////////////////////////////
void ClientManager::DBDeletePlayer( int pid )
{
	DeletePlayerDataContext* delPlayerJob = new DeletePlayerDataContext( pid );
	g_DatabaseJobManager->PushDatabaseJobRequest( delPlayerJob );
}

void ClientManager::CreatePlayerDone( DatabaseJobContext* dbJob )
{
	// 부모의 추상화 클래스의 포인터를 인자로 받았으므로 dynamic_cast로 클래스 형 변환 해서
	// 자식 클래스인 CreatePlayerDataContext로 캐스팅
	CreatePlayerDataContext* createJob = dynamic_cast<CreatePlayerDataContext*>( dbJob );

	printf( "PLAYER[%d] CREATED: %s \n", createJob->m_PlayerId, createJob->m_PlayerName );
}

void ClientManager::DeletePlayerDone( DatabaseJobContext* dbJob )
{
	// CreatePlayerDone과 같음
	DeletePlayerDataContext* deleteJob = dynamic_cast<DeletePlayerDataContext*>( dbJob );

	printf( "PLAYER [%d] DELETED\n", deleteJob->m_PlayerId );

}

bool ClientManager::DeleteClient( ClientSession* client )
{
	if ( client->m_Socket != NULL )
	{
		m_ClientList.erase( client->m_Socket );
		return true;
	}

	return false;
}

void ClientManager::PrintClientList()
{
	for ( auto& it : m_ClientList )
	{
		ClientSession* client = it.second;
		printf_s( "[%d][%s] \n", client->m_PlayerID, client->m_PlayerName );
	}
}




bool ClientManager::ReadMapFile( const char* filename )
{
	FILE* f = NULL;
	fopen_s( &f, filename, "rb" );

	if ( f == NULL )
	{
		return false;
	}

	unsigned char info[54];
	fread( info, sizeof( unsigned char ), 54, f ); // read the 54-byte header

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	m_Map.reserve( height );

	int row_padded = ( width * 3 + 3 ) & ( ~3 );
	unsigned char* data = new unsigned char[row_padded];


	unsigned char tmp;
	for ( int i = 0; i < height; i++ )
	{
		std::vector<Tile> row;
		row.reserve( width );
		fread( data, sizeof( unsigned char ), row_padded, f );
		for ( int j = 0; j < width * 3; j += 3 )
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			//Tile tile = { (int)data[j], (int)data[j + 1], (int)data[j + 2] };
			Tile tile = { data[j], data[j + 1], data[j + 2] };

			// 맵이 뒤집어져 있을 것 같다!!
			row.push_back( tile );
		}
		m_Map.push_back( row );
	}
	printf_s( "[%d][%d] Map Loaded! \n", m_Map.size(), m_Map.at( 0 ).size() );

	fclose( f );

	return true;
}

int ClientManager::GenerateCorps( int playerID, UnitType type, PositionInfo position )
{
	Corps* corps = nullptr;
	switch ( type )
	{
		case UnitType::UNIT_ARROW:
			corps = new Arrow( playerID, ++m_CorpsIDCount, position );
			break;
		case  UnitType::UNIT_GUARD:
			corps = new Guard( playerID, ++m_CorpsIDCount, position );
			break;
		case  UnitType::UNIT_KNIGHT:
			corps = new Knight( playerID, ++m_CorpsIDCount, position );
			break;
		case  UnitType::UNIT_PIKE:
			corps = new Pike( playerID, ++m_CorpsIDCount, position );
			break;
		case  UnitType::UNIT_SWORD:
			corps = new Sword( playerID, ++m_CorpsIDCount, position );
			break;
		default:
			return -1;
	}

	m_CorpsList.insert( CorpsList::value_type( m_CorpsIDCount, corps ) );
	return corps->GetCorpsID();
}

Corps* ClientManager::GetCorpsByCorpsID( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end( ) )
	{
		return m_CorpsList.find( corpsID )->second;
	}

	return nullptr;
}

void ClientManager::AddActionToScheduler( Action* addedAction, int64_t remainTime )
{
	m_ActionScheduler->AddActionToScheduler( addedAction, remainTime );
}

