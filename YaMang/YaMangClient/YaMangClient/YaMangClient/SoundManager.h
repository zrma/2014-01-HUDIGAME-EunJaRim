#pragma once
#include "MacroSet.h"
#include "fmod_studio.hpp"

enum SoundType
{
	SOUND_NONE,
	SOUND_SYSTEM_MOUSE_CLICK,
	SOUND_SCENE_PLAY_BGM1,
	SOUND_SCENE_PLAY_BGM2,
<<<<<<< HEAD
	SOUND_SCENE_PLAY_BGM3,
	SOUND_SCENE_PLAY_BGM4,
=======
>>>>>>> 7dcf2c7b0acdc2245fe7241a99e93c22a00241eb
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
<<<<<<< HEAD
	void							StopAllSound();

=======
	
>>>>>>> 7dcf2c7b0acdc2245fe7241a99e93c22a00241eb
	bool							SetVolume( SoundType soundType, float volume ); // 1.0f max
private:
	void							LoadBank( const char* fileName );
	FMOD::Studio::EventInstance*	GetEventInstance( const char* path );

private:
	FMOD::Studio::System*			m_System = NULL;
<<<<<<< HEAD
	std::array<FMOD::Studio::EventInstance*, SOUND_MAX> m_SoundList;
=======
	std::array<FMOD::Studio::EventInstance*, SOUND_MAX> m_TextList;
>>>>>>> 7dcf2c7b0acdc2245fe7241a99e93c22a00241eb
};

