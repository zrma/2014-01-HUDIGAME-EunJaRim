#include "stdafx.h"
#include "SoundManager.h"
#include "fmod_studio.hpp"
#include "MainWindow.h"
#include "ResourceDef.h"

SoundManager::SoundManager()
{
	m_TextList.fill( nullptr );
	
	FMOD_RESULT result;
	FMOD::Studio::System::create( &m_System );
	result = m_System->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL );
	if ( result )
	{
		Log( "System Initialize Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"System Initialize Fail", L"Sound Error", MB_ICONERROR );
	}

	USES_CONVERSION;
	LoadBank( W2A(SOUND_FILE_MASTER_BANK) );
	LoadBank( W2A(SOUND_FILE_MASTER_BANK_STRINGS) );

	LoadBank( W2A(SOUND_FILE_LOOP) );
	LoadBank( W2A( SOUND_FILE_SFX_BANK ) );

	//////////////////////////////////////////////////////////////////////////


	m_TextList[SOUND_SYSTEM_MOUSE_CLICK] = GetEventInstance( "event:/sfx/system/mouse_click" );
	m_TextList[SOUND_SCENE_PLAY_BGM1] = GetEventInstance( "event:/loop/scene_play_01" );
	m_TextList[SOUND_SCENE_PLAY_BGM2] = GetEventInstance( "event:/loop/scene_play_02" );
	m_TextList[SOUND_CORPS_GENERATE] = GetEventInstance( "event:/sfx/play/corps/generate_corps" );
	m_TextList[SOUND_CORPS_MOVE] = GetEventInstance( "event:/sfx/play/corps/generate_corps" );
	m_TextList[SOUND_CORPS_ATTACK_ARROW] = GetEventInstance( "event:/sfx/play/corps/attack/attack_arrow" );
	m_TextList[SOUND_CORPS_ATTACK_GUARD] = GetEventInstance( "event:/sfx/play/corps/attack/attack_guard" );
	m_TextList[SOUND_CORPS_ATTACK_KNIGHT] = GetEventInstance( "event:/sfx/play/corps/attack/attack_horse" );
	m_TextList[SOUND_CORPS_ATTACK_PIKE] = GetEventInstance( "event:/sfx/play/corps/attack/attack_pike" );
	m_TextList[SOUND_CORPS_ATTACK_SWORD] = GetEventInstance( "event:/sfx/play/corps/attack/attack_sword" );

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

bool SoundManager::PlaySound( SoundType soundType )
{
	if ( m_TextList[soundType] != nullptr)
	{
		m_TextList[soundType]->start();
		m_System->update();

		return true;
	}
	
	return false;
}

bool SoundManager::StopSound( SoundType soundType )
{
	if ( m_TextList[soundType] != nullptr )
	{

		return true;
	}

	return false;
}

bool SoundManager::SetVolume( SoundType soundType, float volume )
{
	if ( m_TextList[soundType] != nullptr )
	{
		m_TextList[soundType]->setVolume( volume );
		m_System->update( );
		return true;
	}

	return false;
}
