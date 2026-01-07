#ifndef AUDIO_H
#define AUDIO_H

#include <fmod/fmod.hpp>
#include "battleeventlistener.h"
#include "inputeventlistener.h"
#include "battlesystem.h"

class Audio : public BattleEventListener, public InputEventListener
{
public:
	Audio();
	void PlayPreBattleBgm();
	void StopPreBattleBgm();
	void PlayBattleBgm();
	void StopBattleBgm();
    void PlayMenuTick();
    void StopMenuTick();
	double PreBattleBgmLength;

    BattleSystem* BattleSystem;

private:
	FMOD::System* System;

	FMOD::Sound* PreBattleBgm;
	FMOD::Channel* PreBattleBgmCh;

	FMOD::Sound* BattleBgm;
	FMOD::Channel* BattleBgmCh;

	FMOD::Sound* MenuTick;
	FMOD::Channel* MenuTickCh;


	void OnBattlePhaseChange(BattlePhase bp) override;
    void OnMenuActionButtonPress(MenuAction button) override;
};

#endif
