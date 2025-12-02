#ifndef AUDIO_H
#define AUDIO_H

#include <fmod/fmod.hpp>
#include "battleeventlistener.h"

class Audio : public BattleEventListener
{
public:
	Audio();
	void PlayPreBattleBgm();
	void StopPreBattleBgm();
	void PlayBattleBgm();
	void StopBattleBgm();
	double PreBattleBgmLength;

private:
	FMOD::System* System;
	FMOD::Sound* PreBattleBgm;
	FMOD::Channel* PreBattleBgmCh;
	FMOD::Sound* BattleBgm;
	FMOD::Channel* BattleBgmCh;
	void OnBattlePhaseChange(BattlePhase bp) override;
};

#endif
