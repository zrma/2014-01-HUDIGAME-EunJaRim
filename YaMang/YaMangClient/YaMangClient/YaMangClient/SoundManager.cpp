#include "stdafx.h"
#include "SoundManager.h"
#include "fmod_studio.hpp"
#include "MainWindow.h"

SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}

void SoundManager::Initialize()
{
	FMOD::Studio::System::create( &m_System );
	m_Result = m_System->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL );
	if ( m_Result )
	{
		Log( "System Initialize Fail \n" );
		MessageBox( MainWindow::GetInstance( )->Window( ), L"System Initialize Fail", L"Sound Error", MB_ICONERROR );
	}

	LoadBank( "SoundFiles/Master Bank.bank" );
	LoadBank( "SoundFiles/Master Bank.strings.bank" );

	LoadBank( "SoundFiles/loop.bank" );
	LoadBank( "SoundFiles/sfx.bank" );

}

void SoundManager::test()
{
	FMOD::Studio::EventInstance* eventInstance = GetEventInstance( "event:/loop/scene_play_01" );
	eventInstance->start();
	m_System->update();
}

void SoundManager::LoadBank( const char* fileName )
{
	FMOD::Studio::Bank* bank = NULL;
	m_Result = m_System->loadBankFile( fileName, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank );
	if ( m_Result )
	{
		Log( "Bank Load Fail:%s \n", fileName );
		std::wstring message = std::wstring( L"Bank Load Fail:" ) + std::wstring( fileName, fileName + strlen( fileName ) );
		MessageBox( MainWindow::GetInstance( )->Window( ), message.c_str(), L"Sound Error", MB_ICONERROR );
	}
}

FMOD::Studio::EventInstance* SoundManager::GetEventInstance( const char* path )
{

	FMOD::Studio::ID eventID = { 0 };
	m_Result = m_System->lookupID( path, &eventID );
	if ( m_Result )
	{
		Log( "Event Load Fail:%s \n", path );
		std::wstring message = std::wstring( L"Event Load Fail:" ) + std::wstring( path, path + strlen( path ) );
		MessageBox( MainWindow::GetInstance()->Window(), message.c_str(), L"Sound Error", MB_ICONERROR );
	}

	FMOD::Studio::EventDescription* eventDescription = NULL;
	m_Result = m_System->getEvent( &eventID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription );
	if ( m_Result )
	{
		Log( "GetEvent Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"GetEvent Fail", L"Sound Error", MB_ICONERROR );
	}

	FMOD::Studio::EventInstance* eventInstance = NULL;
	m_Result = eventDescription->createInstance( &eventInstance );
	if ( m_Result )
	{
		Log( "Create Instance Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"Create Instance Fail", L"Sound Error", MB_ICONERROR );
	}

	
	return eventInstance;
}
