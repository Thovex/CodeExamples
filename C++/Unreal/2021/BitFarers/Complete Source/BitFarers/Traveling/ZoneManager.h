// Copyright Baer and Hoggo

#pragma once

#include "CoreMinimal.h"



#include "ActZoneLevelsRow.h"
#include "EAct.h"
#include "EZoneDifficulty.h"
#include "EZoneType.h"
#include "GameFramework/Actor.h"
#include "ZoneManager.generated.h"

class ATravelPortal;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZoneBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZoneFinished);

UCLASS()
class BITFARERS_API AZoneManager : public AActor
{
	GENERATED_BODY()

public:
	AZoneManager();

	UFUNCTION(BlueprintCallable)
	void InitializeZone(const FActZoneLevelIntroData Intro, const FName Name, const EAct Act, const EZoneType Type, const EZoneDifficulty Difficulty);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginZone();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void EndZone();

	UFUNCTION()
	void SetPortals(TArray<ATravelPortal*> InPortals);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<ATravelPortal*> Portals;

protected:

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FZoneBegin OnZoneBegin;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FZoneFinished OnZoneFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ZoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EZoneDifficulty ZoneDifficulty;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EAct ZoneAct;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EZoneType ZoneType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FActZoneLevelIntroData ZoneIntro;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

};
