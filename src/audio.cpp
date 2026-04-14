#include "audio.h"
#include "battlesystem.h"

#include <fmod/fmod_errors.h>
#include <stdio.h>
#include <iostream>

Audio::Audio(BattleSystem& battleSystem)
{
    FMOD_RESULT result;

    result = FMOD::System_Create(&System);      // Create the main system object.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    }

    result = System->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    }

    System->createSound("audio\\battlepre.wav", 0, 0, &PreBattleBgm);
    System->createSound("audio\\battlestart.wav", 0, 0, &BattleBgm);

    System->createSound("audio\\menutick.wav", FMOD_LOOP_OFF, 0, &MenuTick);

    unsigned int length;
    PreBattleBgm->getLength(&length, FMOD_TIMEUNIT_MS);
    PreBattleBgmLength= length / 1000.0;

    //event subscription
    battleSystem.SubscribeToEvents(*this);
}

//TODO: don't get the mLength like this, set it during the init phase or something
void Audio::PlayPreBattleBgm()
{
    System->playSound(PreBattleBgm, 0, false, &PreBattleBgmCh);
}

void Audio::StopPreBattleBgm()
{
    PreBattleBgmCh->stop();
}

void Audio::PlayBattleBgm()
{
    bool isPlaying;
    BattleBgmCh->isPlaying(&isPlaying);

    if(!isPlaying)
        System->playSound(BattleBgm, 0, false, &BattleBgmCh);
}

void Audio::PlayMenuTick()
{
    FMOD::Channel* ch;
    System->playSound(MenuTick, 0, false, &ch);
    ch->setVolume(SfxVolume);
}

void Audio::StopBattleBgm()
{
    BattleBgmCh->stop();
}

void Audio::OnBattlePhaseChange(BattlePhase bp)
{
    switch (bp)
    {
    case BattlePhase::Preamble:
    {
		PlayPreBattleBgm();
    }
        break;
    case BattlePhase::Slide:
        break;
    case BattlePhase::Snap:
        break;
    case BattlePhase::Start:
		StopPreBattleBgm();
		PlayBattleBgm();
        break;
    case BattlePhase::End:
    {
		StopBattleBgm();
    }
        break;
    default:
        break;
    }
}

//TODO: ceate a function that the menu can call when it changes selecton that plays menu tick
