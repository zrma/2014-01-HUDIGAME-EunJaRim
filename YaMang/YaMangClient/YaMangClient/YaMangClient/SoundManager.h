﻿#pragma once
#include "MacroSet.h"
#include "fmod_studio.hpp"

enum SoundType
{
	SOUND_NONE,
	SOUND_SYSTEM_MOUSE_CLICK,
	SOUND_SCENE_PLAY_BGM1,
	SOUND_SCENE_PLAY_BGM2,
	SOUND_CORPS_GENERATE,
	SOUND_CORPS_MOVE,
	SOUND_CORPS_ATTACK_ARROW,
	SOUND_CORPS_ATTACK_GUARD,
	SOUND_CORPS_ATTACK_KNIGHT,
	SOUND_CORPS_ATTACK_PIKE,
	SOUND_CORPS_ATTACK_SWORD,
	SOUND_MAX,
};

class SoundManager:public Singleton<SoundManager>
{
public:
	SoundManager();
	~SoundManager();

	bool							PlaySound( SoundType soundType );
	bool							StopSound( SoundType soundType );
	
private:
	void							LoadBank( const char* fileName );
	FMOD::Studio::EventInstance*	GetEventInstance( const char* path );

private:
	FMOD::Studio::System*			m_System = NULL;
	std::array<FMOD::Studio::EventInstance*, SOUND_MAX> m_TextList;
};

