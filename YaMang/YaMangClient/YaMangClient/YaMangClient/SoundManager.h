#pragma once
#include "MacroSet.h"

class SoundLowLevel;
class SoundManager:public Singleton<SoundManager>
{
public:
	SoundManager();
	~SoundManager();

	void	Initialize();
	void	test();

private:
	SoundLowLevel* m_SoundLowLevel = nullptr;
};

