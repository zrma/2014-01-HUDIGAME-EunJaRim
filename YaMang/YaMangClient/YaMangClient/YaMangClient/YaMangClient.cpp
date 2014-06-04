// YaMangClient.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "YaMangClient.h"

#ifdef DEBUG
#include <crtdbg.h>
#endif // DEBUG

#include "Logger.h"
#include "MainWindow.h"
#include "NetworkManager.h"
#include "Exception.h"
#include <tlhelp32.h>
INT_PTR g_LogHandle = -1;




//
//    프로세스 간 통신을 하기 전에 전송할 데이터 종류에 대한 프로토콜 정의 ..
//
struct Protocol
{
	enum { PROTOCOL_TEST };
};
struct MessageInfo
{
	int     nVersion;
	char   szTest[10];
};



int APIENTRY _tWinMain( _In_ HINSTANCE hInstance,
						_In_opt_ HINSTANCE hPrevInstance,
						_In_ LPTSTR    lpCmdLine,
						_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hInstance );
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// 메모리 릭을 체크하려면 아래의 #### 칸에 릭 난 곳 { 숫자 } 표기 된 숫자를 넣어주면 됩니다.
	// _CrtSetBreakAlloc( #### );
	// _CrtSetBreakAlloc( 192 );

	// SetUnhandledExceptionFilter( ExceptionFilter );

	InstallCrashReporter();

	// set_terminate() must be called from every thread
	BT_SetTerminate();
	BT_InsLogEntry( g_LogHandle, BTLL_INFO, _T( "Entering main() function" ) );

#ifdef _PRINT_CONSOLE
	Logger::GetInstance()->CreateConsole();
#endif

	//////////////////////////////////////////////////////////////////////////

	MessageInfo sMessageInfo;

	sMessageInfo.nVersion = 100;
	strcpy_s( sMessageInfo.szTest, "TEST" );

	COPYDATASTRUCT CDS;
	CDS.dwData = Protocol::PROTOCOL_TEST;
	CDS.cbData = sizeof( sMessageInfo );
	CDS.lpData = &sMessageInfo;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof( PROCESSENTRY32 );

	HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

	if ( Process32First( snapshot, &entry ) == TRUE )
	{
		while ( Process32Next( snapshot, &entry ) == TRUE )
		{
			//if ( _wcsicmp( entry.szExeFile, _T( "YaMangLoader.exe" ) ) == 0 )
			if ( _wcsicmp( entry.szExeFile, _T( "YaMangLoader.vshost.exe") ) == 0 )
			{
				//HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID );
				HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID );
				HWND hwnd = (HWND)hProcess;
				// Do stuff..
				HWND hwnd1 = FindWindow( _T( "YaMangLoader.vshost.exe" ), NULL );//1

				int WM_USER1 = WM_USER + 1;
				BOOL bReturnValue = SendMessage( HWND_BROADCAST, WM_USER, (WPARAM)hwnd, (LPARAM)(LPVOID)&CDS ) == TRUE;
				CloseHandle( hProcess );
			}
		}
	}

	CloseHandle( snapshot );
	/*
	//find running processes and send them a message
	//can't just search for "MYAPP.exe" as will be called "MYAPP.exe *32" on a 64bit machine
	std::array<System::Diagnostics::Process^>^allProcesses = System::Diagnostics::Process::GetProcesses();

	for each ( System::Diagnostics::Process^ targetProcess in allProcesses )
	{
		if ( targetProcess->ProcessName->StartsWith( "MYAPP", System::StringComparison::OrdinalIgnoreCase ) )
		{
			HWND handle = static_cast<HWND>( targetProcess->MainWindowHandle.ToPointer() );

			BOOL bReturnValue = SendMessage( handle, WM_COPYDATA, (WPARAM)0, (LPARAM)&CDS ) == TRUE;
		}
	}
	*/

	//////////////////////////////////////////////////////////////////////////


	// test code
	std::wstring parameter = lpCmdLine;
	printf_s( "parameter:%S \n", parameter.c_str() );

	srand( static_cast<unsigned int>( time( NULL ) ) );
	unsigned int pos = parameter.find( ' ' );
	if ( -1 == pos )
	{
		NetworkManager::GetInstance()->SetRoomNumber( 1 );
		NetworkManager::GetInstance()->SetMyPlayerID( 1000 + rand() % 101 );
	}
	else
	{
		if ( -1 == parameter.find( ' ', pos + 1 ) )
		{
			std::wstring roomNumber = parameter.substr( 0, pos );
			std::wstring playerID = parameter.substr( pos+1 );
			printf_s( "ROOM:%S PLAYER:%S \n", roomNumber.c_str( ), playerID.c_str( ) );

			NetworkManager::GetInstance( )->SetRoomNumber( std::stoi( roomNumber ) );
			NetworkManager::GetInstance( )->SetMyPlayerID( std::stoi( playerID ) );
		}
		else
		{
			NetworkManager::GetInstance()->SetRoomNumber( 1 );
			NetworkManager::GetInstance()->SetMyPlayerID( 1000 + rand() % 101 );
		}
	}

	// WS_POPUPWINDOW
	// WS_OVERLAPPEDWINDOW
	if ( false == MainWindow::GetInstance()->Create( L"YaMang", WS_OVERLAPPEDWINDOW ) )
	{
		return 0;
	}

	MainWindow::GetInstance()->Display( nCmdShow );

#ifndef DEBUG
	HANDLE mutex = CreateMutex( NULL, FALSE, L"YaMangClientMutex" );
	if ( mutex == NULL )
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"Create Mutex Error", L"Mutex", MB_OK | MB_ICONERROR );
		return -1;
	}

	DWORD waitResult = WaitForSingleObject( mutex, 1 );
	switch ( waitResult )
	{
		case WAIT_TIMEOUT:
		{	
							 MessageBox( MainWindow::GetInstance()->Window(), L"Client Already Running", L"Mutex", MB_OK | MB_ICONERROR );
							 MainWindow::Release();
							 return -1;
		}
			break;
		case WAIT_OBJECT_0:
		case WAIT_ABANDONED:
		default:
			break;
	}
#endif

	int result = MainWindow::GetInstance()->RunGame();
	MainWindow::Release();

#ifdef _PRINT_CONSOLE
	Logger::GetInstance()->DestroyConsole();
	Logger::Release();
#endif

	BT_InsLogEntry( g_LogHandle, BTLL_INFO, _T( "Leaving main() function" ) );
	BT_CloseLogFile( g_LogHandle );

	return result;
}