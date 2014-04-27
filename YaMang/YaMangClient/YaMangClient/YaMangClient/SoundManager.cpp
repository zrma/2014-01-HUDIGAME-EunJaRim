#include "stdafx.h"
#include "SoundManager.h"
#include "SoundLowLevel.h"


SoundManager::SoundManager()
{
}


SoundManager::~SoundManager()
{
}

void SoundManager::Initialize()
{
	m_SoundLowLevel = new SoundLowLevel();
}

void SoundManager::test()
{
	m_SoundLowLevel->test();
}
