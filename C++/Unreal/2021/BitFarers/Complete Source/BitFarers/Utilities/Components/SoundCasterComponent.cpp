// Copyright Baer and Hoggo Games


#include "SoundCasterComponent.h"

#include "Kismet/GameplayStatics.h"
#include "BitFarers/Macros.h"


// Sets default values for this component's properties
USoundCasterComponent::USoundCasterComponent()
{
	SetIsReplicatedByDefault(true);
}

// -- Play One Shot Sound Attached --

void USoundCasterComponent::PlayOneShotSoundAttached_SoundBase(
    const bool bNetworked, USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier)
{
	if (bNetworked)
	{
		PlayOneShotSoundAttached_SoundBase_Server(Sound, VolumeMultiplier, PitchMultiplier);
		return;
	}

	PlayOneShotSoundAttached_Clientside(Sound, VolumeMultiplier, PitchMultiplier);
}

void USoundCasterComponent::PlayOneShotSoundAttached_Name(
	const bool bNetworked, const FName SoundName, const float VolumeMultiplier, const float PitchMultiplier)
{
	if (!Audio.Contains(SoundName))
	{
		PRINT("Error in PlayOneShotSoundAttached_Name with: " + SoundName.ToString());
		return;
	}

	const int32 AudioArrayCount = Audio[SoundName].Audio.Num();
	
	if (AudioArrayCount == 0)
	{
		PRINT("Error in PlayOneShotSoundAttached_Name with Audio Length for: " + SoundName.ToString());
	}

	const int32 SelectedAudioId = FMath::RandRange(0, AudioArrayCount - 1);

	if (bNetworked)
	{
		PlayOneShotSoundAttached_Name_Server(SoundName, SelectedAudioId, VolumeMultiplier, PitchMultiplier);
		return;
	}

	PlayOneShotSoundAttached_Clientside(Audio[SoundName].Audio[SelectedAudioId], VolumeMultiplier, PitchMultiplier );
}

void USoundCasterComponent::PlayOneShotSoundAttached_SoundBase_Server_Implementation(
    USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier)
{
	PlayOneShotSoundAttached_SoundBase_Multicast(Sound, VolumeMultiplier, PitchMultiplier);
}

void USoundCasterComponent::PlayOneShotSoundAttached_Name_Server_Implementation(
    const FName SoundName, const int32 SoundId, const float VolumeMultiplier, const float PitchMultiplier)
{
	PlayOneShotSoundAttached_Name_Multicast(SoundName, SoundId, VolumeMultiplier, PitchMultiplier);
}

void USoundCasterComponent::PlayOneShotSoundAttached_SoundBase_Multicast_Implementation(
    USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier)
{
	PlayOneShotSoundAttached_Clientside(Sound, VolumeMultiplier, PitchMultiplier);
}

void USoundCasterComponent::PlayOneShotSoundAttached_Name_Multicast_Implementation(
	const FName SoundName, const int32 SoundId, const float VolumeMultiplier, const float PitchMultiplier)
{
	PlayOneShotSoundAttached_Clientside(Audio[SoundName].Audio[SoundId], VolumeMultiplier, PitchMultiplier );
}

void USoundCasterComponent::PlayOneShotSoundAttached_Clientside(
	USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier) const
{
	UGameplayStatics::SpawnSoundAttached(Sound, GetOwner()->GetRootComponent(), NAME_None, FVector(ForceInit),
    FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false,
    VolumeMultiplier, PitchMultiplier, 0, Attenuation, nullptr, true);
}


// -- Play One Shot Sound At Location --

/*
void USoundCasterComponent::PlayOneShotSoundAtLocation(
    const bool bNetworked, USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier)
{
	if (bNetworked)
	{
		PlayOneShotSoundAtLocation_Server(Sound, Location, VolumeMultiplier, PitchMultiplier);
		return;
	}

	PlayOneShotSoundAtLocation_Clientside(Sound, Location, VolumeMultiplier, PitchMultiplier);
}

void USoundCasterComponent::PlayOneShotSoundAtLocation_Server_Implementation(
    USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier)
{
	PlayOneShotSoundAtLocation_Multicast(Sound, Location, VolumeMultiplier, PitchMultiplier);
}

void USoundCasterComponent::PlayOneShotSoundAtLocation_Multicast_Implementation(
	USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier)
{
	PlayOneShotSoundAtLocation_Clientside(Sound, Location, VolumeMultiplier, PitchMultiplier);
}

void USoundCasterComponent::PlayOneShotSoundAtLocation_Clientside(
    USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier) const
{
	const UWorld* World = GetWorld();
	
	UGameplayStatics::SpawnSoundAtLocation(World, Sound, Location, FRotator::ZeroRotator, VolumeMultiplier,
        PitchMultiplier, 0, Attenuation, nullptr, true);
}
*/

