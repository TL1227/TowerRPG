#ifndef AUDIO_H
#define AUDIO_H

#include <fmod/fmod.hpp>
#include "battleeventlistener.h"

class BattleSystem;

class Audio : public BattleEventListener
{
public:
	Audio(BattleSystem& systems);
	void PlayPreBattleBgm();
	void StopPreBattleBgm();
	void PlayBattleBgm();
	void StopBattleBgm();
    void PlayMenuTick();
    void StopMenuTick();
	double PreBattleBgmLength;

    float SfxVolume = 0.1f;

private:
	FMOD::System* System;

	FMOD::Sound* PreBattleBgm;
	FMOD::Channel* PreBattleBgmCh = nullptr;

	FMOD::Sound* BattleBgm;
	FMOD::Channel* BattleBgmCh = nullptr;

	FMOD::Sound* MenuTick;

	void OnBattlePhaseChange(BattlePhase bp) override;
};

#endif
