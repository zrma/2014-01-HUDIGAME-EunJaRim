#include "stdafx.h"
#include "SoundLowLevel.h"
#include "fmod_studio.hpp"


struct ProgrammerSoundContext
{
	FMOD::System* system;
	const char* soundName;
};

FMOD_RESULT F_CALLBACK programmerSoundCallback( FMOD_STUDIO_EVENT_CALLBACK_TYPE type, void *parameters );

SoundLowLevel::SoundLowLevel()
{
}


SoundLowLevel::~SoundLowLevel()
{
}

void SoundLowLevel::test()
{
	FMOD_RESULT result; // if(result) <-정상이 아니면 빠짐

	FMOD::Studio::System* system = NULL;
	FMOD::Studio::System::create( &system );
	result = system->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL );





	FMOD::Studio::Bank* masterBank = NULL;
	result = system->loadBankFile( "SoundFiles/Master Bank.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank );

	FMOD::Studio::Bank* stringsBank = NULL;
	result = system->loadBankFile( "SoundFiles/Master Bank.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank );
	
	FMOD::Studio::Bank* ambienceBank = NULL;
	result = system->loadBankFile( "SoundFiles/Character.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &ambienceBank );

	FMOD::Studio::Bank* loopAmbienceBank = NULL;
	result = system->loadBankFile( "SoundFiles/Surround_Ambience.bank" , FMOD_STUDIO_LOAD_BANK_NORMAL, &loopAmbienceBank );



	FMOD::Studio::ID loopingAmbienceID = { 0 };
	result = system->lookupID( "event:/Ambience/Country", &loopingAmbienceID );

	FMOD::Studio::EventDescription* loopingAmbienceDescription = NULL;
	result = system->getEvent( &loopingAmbienceID, FMOD_STUDIO_LOAD_BEGIN_NOW, &loopingAmbienceDescription );

	FMOD::Studio::EventInstance* loopingAmbienceInstance = NULL;
	result = loopingAmbienceDescription->createInstance( &loopingAmbienceInstance );





	FMOD::Studio::ID eventID = { 0 };
	result = system->lookupID( "event:/Character/Footsteps/Footsteps", &eventID );

	FMOD::Studio::EventDescription* eventDescription = NULL;
	result = system->getEvent( &eventID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription );

	FMOD::Studio::EventInstance* eventInstance = NULL;
	result = eventDescription->createInstance( &eventInstance );

	FMOD::Studio::ParameterInstance* surfaceParameter = NULL;
	result = eventInstance->getParameter( "Surface", &surfaceParameter );

	// Make the event audible to start with
	surfaceParameter->setValue( 1.0f );

	float surfaceParameterValue = 0;
	surfaceParameter->getValue( &surfaceParameterValue );





	eventInstance->start();
	loopingAmbienceInstance->start();
	system->update();
}

