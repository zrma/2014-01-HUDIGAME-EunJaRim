#pragma once
#include "MacroSet.h"
#include "fmod_studio.hpp"


class SoundManager:public Singleton<SoundManager>
{
public:
	SoundManager();
	~SoundManager();

	void							Initialize();
	void							test(); // 이벤트 인스턴스 관리를 어떻게 해줘야 할까...

	void							LoadBank( const char* fileName );
	FMOD::Studio::EventInstance*	GetEventInstance( const char* path ); // 안전 할려나...

private:
	FMOD::Studio::System*			m_System = NULL;

};

