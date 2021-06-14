// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoundCasterComponent.generated.h"


USTRUCT(BlueprintType)
struct FAudioArray
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> Audio;
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BITFARERS_API USoundCasterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USoundCasterComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundAttenuation* Attenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, FAudioArray> Audio;

	// -- Play One Shot Sound Attached --

public:
	UFUNCTION(BlueprintCallable)
	void PlayOneShotSoundAttached_SoundBase(const bool bNetworked, USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier);

	UFUNCTION(BlueprintCallable)
	void PlayOneShotSoundAttached_Name(const bool bNetworked, const FName SoundName, const float VolumeMultiplier, const float PitchMultiplier);

private:
	UFUNCTION(Unreliable, Server)
	void PlayOneShotSoundAttached_SoundBase_Server(USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier);
		
	UFUNCTION(Unreliable, NetMulticast)
    void PlayOneShotSoundAttached_SoundBase_Multicast(USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier);
	
	UFUNCTION(Unreliable, Server)
	void PlayOneShotSoundAttached_Name_Server(const FName SoundName, const int32 SoundId, const float VolumeMultiplier, const float PitchMultiplier);

	UFUNCTION(Unreliable, NetMulticast)
	void PlayOneShotSoundAttached_Name_Multicast(const FName SoundName, const int32 SoundId, const float VolumeMultiplier, const float PitchMultiplier);

	UFUNCTION()
    void PlayOneShotSoundAttached_Clientside(USoundBase* Sound, const float VolumeMultiplier, const float PitchMultiplier) const;

	// -- Play One Shot Sound at Location --


/*
public:
	UFUNCTION(BlueprintCallable)
	void PlayOneShotSoundAtLocation(const bool bNetworked, USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier);

private:
	UFUNCTION(Unreliable, Server)
	void PlayOneShotSoundAtLocation_Server(USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier);
	
	UFUNCTION(Unreliable, NetMulticast)
	void PlayOneShotSoundAtLocation_Multicast(USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier);

	UFUNCTION()
	void PlayOneShotSoundAtLocation_Clientside(USoundBase* Sound, const FVector Location, const float VolumeMultiplier, const float PitchMultiplier) const;
	
*/
	
	//UFUNCTION(BlueprintCallable) void PlayOneShotSoundAtActor();
	//UFUNCTION(Unreliable, Server) void PlayOneShotSoundAtActor_Server();
	//UFUNCTION(Unreliable, NetMulticast) void PlayOneShotSoundAtActor_Multicast();

	
	
};
