#include "stdafx.h"
#include "SoundManager.h"
#include "fmod_studio.hpp"
#include "MainWindow.h"

SoundManager::SoundManager()
{
	ZeroMemory( &m_TextList, sizeof( m_TextList ) );

	FMOD_RESULT result;
	FMOD::Studio::System::create( &m_System );
	result = m_System->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL );
	if ( result )
	{
		Log( "System Initialize Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"System Initialize Fail", L"Sound Error", MB_ICONERROR );
	}

	LoadBank( "SoundFiles/Master Bank.bank" );
	LoadBank( "SoundFiles/Master Bank.strings.bank" );

	LoadBank( "SoundFiles/loop.bank" );
	LoadBank( "SoundFiles/sfx.bank" );

	//////////////////////////////////////////////////////////////////////////


	m_TextList[SOUND_SYSTEM_MOUSE_CLICK] = GetEventInstance( "event:/sfx/generate_corps" );
	m_TextList[SOUND_SCENE_PLAY_BGM1] = GetEventInstance( "event:/loop/scene_play_01" );
	m_TextList[SOUND_CORPS_GENERATE] = GetEventInstance( "event:/sfx/generate_corps" );
	m_TextList[SOUND_CORPS_MOVE] = GetEventInstance( "event:/sfx/generate_corps" );

}


SoundManager::~SoundManager()
{
}


void SoundManager::LoadBank( const char* fileName )
{
	FMOD_RESULT result;
	FMOD::Studio::Bank* bank = NULL;
	result = m_System->loadBankFile( fileName, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank );
	if ( result )
	{
		Log( "Bank Load Fail:%s \n", fileName );
		std::wstring message = std::wstring( L"Bank Load Fail:" ) + std::wstring( fileName, fileName + strlen( fileName ) );
		MessageBox( MainWindow::GetInstance( )->Window( ), message.c_str(), L"Sound Error", MB_ICONERROR );
	}
}

FMOD::Studio::EventInstance* SoundManager::GetEventInstance( const char* path )
{
	FMOD_RESULT result;
	FMOD::Studio::ID eventID = { 0 };
	result = m_System->lookupID( path, &eventID );
	if ( result )
	{
		Log( "Event Load Fail:%s \n", path );
		std::wstring message = std::wstring( L"Event Load Fail:" ) + std::wstring( path, path + strlen( path ) );
		MessageBox( MainWindow::GetInstance()->Window(), message.c_str(), L"Sound Error", MB_ICONERROR );
	}

	FMOD::Studio::EventDescription* eventDescription = NULL;
	result = m_System->getEvent( &eventID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription );
	if ( result )
	{
		Log( "GetEvent Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"GetEvent Fail", L"Sound Error", MB_ICONERROR );
	}

	FMOD::Studio::EventInstance* eventInstance = NULL;
	result = eventDescription->createInstance( &eventInstance );
	if ( result )
	{
		Log( "Create Instance Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"Create Instance Fail", L"Sound Error", MB_ICONERROR );
	}

	
	return eventInstance;
}

void SoundManager::test()
{
	FMOD::Studio::EventInstance* eventInstance = GetEventInstance( "event:/loop/scene_play_01" );
	eventInstance->start();
	m_System->update();
}

void SoundManager::test2()
{
	FMOD::Studio::EventInstance* eventInstance = GetEventInstance( "event:/sfx/generate_corps" );
	eventInstance->start();
	m_System->update();
}

bool SoundManager::PlaySound( SoundType soundType )
{
	return true;
}

bool SoundManager::StopSound( SoundType soundType )
{
	return true;
}
