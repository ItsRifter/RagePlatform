
#include "Gameplay/RageUserGameSettings.h"
#include "RageUserGameSettings.h"

URageUserGameSettings::URageUserGameSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MasterVolume = 1.0f;
	DialogueVolume = 0.8f;
	SFXVolume = 0.8f;
	MusicVolume = 0.7f;
}

URageUserGameSettings* URageUserGameSettings::GetRageOptionGameUserSettings()
{
	return Cast<URageUserGameSettings>(UGameUserSettings::GetGameUserSettings());
}

void URageUserGameSettings::SetMasterVolume(float newVolume)
{
	MasterVolume = newVolume;
}

void URageUserGameSettings::SetSFXVolume(float newVolume)
{
	SFXVolume = newVolume;
}

void URageUserGameSettings::SetDialogueVolume(float newVolume)
{
	DialogueVolume = newVolume;
}

void URageUserGameSettings::SetMusicVolume(float newVolume)
{
	MusicVolume = newVolume;
}

float URageUserGameSettings::GetMasterVolume()
{
	return MasterVolume;
}

float URageUserGameSettings::GetSFXVolume()
{
	return SFXVolume;
}

float URageUserGameSettings::GetDialogueVolume()
{
	return DialogueVolume;
}

float URageUserGameSettings::GetMusicVolume()
{
	return MusicVolume;
}
