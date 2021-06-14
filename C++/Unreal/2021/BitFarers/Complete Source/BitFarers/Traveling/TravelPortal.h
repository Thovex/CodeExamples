// Copyright Baer and Hoggo

#pragma once

#include "CoreMinimal.h"



#include "ActZoneLevelsRow.h"
#include "EAct.h"
#include "EZoneDifficulty.h"
#include "EZoneType.h"
#include "BitFarers/Utilities/Interfaces/ButtonClickable.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GameFramework/Actor.h"
#include "TravelPortal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReadyDelegate, int32, PlayerId, AActor*, Portal);

UCLASS()
class BITFARERS_API ATravelPortal : public AActor, public IButtonClickable
{
private:
	GENERATED_BODY()

public:
	ATravelPortal();
	
	void SetIntro(FActZoneLevelIntroData IntroData);
	void SetZone(EAct InAct, EZoneType InZoneType, TArray<EZoneDifficulty> Difficulties);
	void SetLevels(FName LoadLevelName, TSoftObjectPtr<UWorld> LoadLevel, TSoftObjectPtr<UWorld> UnloadLevel);

	UFUNCTION()
	void ForceUnready(int32 PlayerId);

	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FReadyDelegate SetReady;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_Click(int32 PlayerId);

	UFUNCTION(Server, Reliable)
	void Server_DoTravel();

	UFUNCTION()
	void OnRep_Act();

	UFUNCTION()
	void OnRep_Type();

	UFUNCTION()
	void OnRep_Difficulty();

	UFUNCTION()
	void OnRep_Name();

	UPROPERTY(ReplicatedUsing=OnRep_Act, VisibleAnywhere, BlueprintReadOnly)
	EAct Act;

	UPROPERTY(ReplicatedUsing=OnRep_Type, VisibleAnywhere, BlueprintReadOnly)
	EZoneType ZoneType;

	UPROPERTY(ReplicatedUsing=OnRep_Difficulty, VisibleAnywhere, BlueprintReadOnly)
	EZoneDifficulty ZoneDifficulty;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FActZoneLevelIntroData Intro;
	
	UPROPERTY(ReplicatedUsing=OnRep_Name, EditAnywhere, BlueprintReadWrite)
	FName LoadName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> Load;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> Unload;

	UFUNCTION(BlueprintImplementableEvent)
	void PortalSetup();

	UFUNCTION(BlueprintImplementableEvent)
    void PortalOpen();


protected:
	virtual void BeginPlay() override;
	virtual void DoButtonClick_Implementation(int32 PlayerId, int32 FunctionalIndex) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


private:
	TMap<int32, bool> ReadyStatus;
	
	EZoneDifficulty DetermineDifficulty(TArray<EZoneDifficulty> Difficulties) const;

	void CheckReady();
	void DoTravel();
	
	FTimerHandle TravelTimer;

};
