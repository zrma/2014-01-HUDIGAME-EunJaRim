#include "stdafx.h"
#include "YaMangServer.h"

#include "Config.h"
#include "PacketType.h"

#include "Exception.h"
#include "ClientSession.h"
#include "DatabaseJobManager.h"
#include "DbHelper.h"

#include "tinyxml.h"
#include "xpath_static.h"
#include "RoomManager.h"

#pragma comment(lib,"ws2_32.lib")


__declspec( thread ) int LThreadType = -1;

typedef ProducerConsumerQueue<SOCKET, 100> PendingAcceptList;

static int LISTEN_PORT = 9001;

int _tmain( int argc, _TCHAR* argv[] )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	// _CrtSetBreakAlloc( 611 );
	// _CrtSetBreakAlloc( #### );
	// 메모리 릭을 체크하려면 위의 #### 칸에 릭 난 곳 { 숫자 } 표기 된 숫자를 넣어주면 됩니다.


	// 패키징 테스트 ?? 정상인듯?
	printf( "[PACKET SIZE]%d\n", sizeof( GenerateCorpsRequest ) );
	printf( "[POSITION SIZE]%d\n", sizeof( Position ) );

	/// crash 발생시 dump 남기기 위해서
	// Exception.cpp 쪽에서 전역 함수로 선언 되어 있음
	SetUnhandledExceptionFilter( ExceptionFilter );
	
	// xml 로드 테스트
	TiXmlDocument document = TiXmlDocument( "../../SharedPreference/ServerConfig.xml" );
	bool m_LoadSuccess = document.LoadFile();

	if ( m_LoadSuccess )
	{
		std::string port;
		port = TinyXPath::S_xpath_string( document.RootElement( ), "/server/port/text()" ).c_str( );
		LISTEN_PORT = std::stoi( port );
		printf_s( "Loaded Port Number :%s \n", port.c_str( ) );
	}
	else
	{
		printf_s( "PORT Load Fail! \n" );
	}



	LThreadType = THREAD_MAIN;

	/// Manager Init
	// 각 매니저들은 각 헤더파일들에 extern 으로 선언 되어 있음
	// 여기서 new 해줌
	g_RoomManager = new RoomManager();
	g_DatabaseJobManager = new DatabaseJobManager();

	/// DB Helper 초기화
	if ( false == DbHelper::Initialize( DB_CONN_STR ) )
	{
		return -1;
	}

	/// 윈속 초기화
	// 윈도우 소켓 사용시 WSAStartup() 함수를 호출해야 winsock.dll을 이용 가능하다
	// 사용 시 유의점 : WSAStartup() / WSACleanup() 함수를 짝으로 맞춰 사
	// ws2_32.dll 파일 필요
	WSADATA wsa;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
	{
		return -1;
	}

	// AF_INET : The Internet Protocol version 4 (IPv4) address family.
	// SOCK_STREAM : TCP
	// socket protocol = 0
	// LAW SOCKET을 사용할 때 사용한다. 주로 사용 되지 않아 0이나 IPPROTO_TCP / IPPROTO_UPT로 사용 된다.
	SOCKET listenSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( listenSocket == INVALID_SOCKET )
	{
		return -1;
	}

	// 소켓의 세부 사항 조절 - getsockopt / setsockopt
	// http://vinchi.tistory.com/246 참조
	// SOL_SOCKET 레벨에서 SO_REUSEADDR 사용 - 이미 사용 된 주소를 재사용(bind) 하도록 한다.
	// -> Address already in use 방지
	int opt = 1;
	setsockopt( listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof( int ) );

	/// bind
	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET; // IPv4
	serveraddr.sin_port = htons( LISTEN_PORT );
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY ); // 0.0.0.0

	// socket() 함수로 소켓을 생성 이후에 소켓에 주소 값을 설정하는 함수가 bind()
	int ret = bind( listenSocket, (SOCKADDR*)&serveraddr, sizeof( serveraddr ) );
	if ( ret == SOCKET_ERROR )
	{
		return -1;
	}

	/// listen
	// SOMAXCONN = BackLog size : 연결 요청 대기 큐의 사이즈
	ret = listen( listenSocket, SOMAXCONN );
	if ( ret == SOCKET_ERROR )
	{
		return -1;
	}

	/// accepting list
	// producer consumer queue
	PendingAcceptList pendingAcceptList;

	/// Client Logic + I/O Thread
	// 스레드 쪽에 이벤트를 전달하기 위해서 hEvent를 전달 인자로 대입
	DWORD dwThreadId;
	HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, ClientHandlingThread, (LPVOID)&pendingAcceptList, 0, (unsigned int*)&dwThreadId );
	if ( hThread == NULL )
	{
		return -1;
	}

	/// DB Thread
	HANDLE hDbThread = (HANDLE)_beginthreadex( NULL, 0, DatabaseHandlingThread, NULL, 0, (unsigned int*)&dwThreadId );
	if ( hDbThread == NULL )
	{
		return -1;
	}

	/// accept loop
	while ( true )
	{
		SOCKET acceptedSocket = accept( listenSocket, NULL, NULL );
		if ( acceptedSocket == INVALID_SOCKET )
		{
			printf( "accept: invalid socket\n" );
			continue;
		}

		// accept 한것을 전달
		pendingAcceptList.Produce( acceptedSocket );
	}

	CloseHandle( hThread );
	CloseHandle( hDbThread );

	// 윈속 종료
	WSACleanup();

	// sqlite3_close
	DbHelper::Finalize();

	delete g_RoomManager;
	delete g_DatabaseJobManager;

	return 0;
}

unsigned int WINAPI ClientHandlingThread( LPVOID lpParam )
{
	// 이 스레드는 클라이언트 핸들링 스레드
	LThreadType = THREAD_CLIENT;

	PendingAcceptList* pAcceptList = (PendingAcceptList*)lpParam;

	/// Timer
	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL );
	if ( hTimer == NULL )
	{
		return -1;
	}

	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -10000000; ///< 1초 후부터 동작, SetWaitabletimer 시간 단위 = 100 나노초

	// 0.1초 주기로 TimerProc 함수 실행 하도록 콜백 설정
	if ( !SetWaitableTimer( hTimer, &liDueTime, 100, TimerProc, NULL, TRUE ) )
	{
		return -1;
	}

	while ( true )
	{
		SOCKET acceptSock = NULL;

		/// 새로 접속한 클라이언트 처리
		if ( pAcceptList->Consume( acceptSock, false ) )
		{
			/// 소켓 정보 구조체 할당과 초기화
			ClientSession* client = g_RoomManager->CreateClient( acceptSock );

			SOCKADDR_IN clientaddr;
			int addrlen = sizeof( clientaddr );
			// 소켓으로부터 클라이언트 네임(sockaddr 주소값)을 얻어옴
			getpeername( acceptSock, (SOCKADDR*)&clientaddr, &addrlen );

			// 클라 접속 처리
			if ( false == client->OnConnect( &clientaddr ) )
			{
				client->Disconnect();
			}

			continue; ///< 다시 대기로
		}

		/// 최종적으로 클라이언트들에 쌓인 send 요청 처리
		g_RoomManager->FlushClientSend( );

		/// APC Queue에 쌓인 작업들 처리
		SleepEx( INFINITE, TRUE );
	}

	CloseHandle( hTimer );

	return 0;
}

unsigned int WINAPI DatabaseHandlingThread( LPVOID lpParam )
{
	LThreadType = THREAD_DATABASE;

	g_DatabaseJobManager->ExecuteDatabaseJobs();

	return 0;
}

void CALLBACK TimerProc( LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue )
{
	assert( LThreadType == THREAD_CLIENT );

	// 클라이언트 쪽은 0.1초 단위로 콜백 함수 호출하면서 주기적으로 해야 할 일 처리
	g_RoomManager->OnPeriodWork( );
}