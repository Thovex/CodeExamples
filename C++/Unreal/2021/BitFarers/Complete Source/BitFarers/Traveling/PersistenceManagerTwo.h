// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "TravelPortal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "PersistenceManagerTwo.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPreTravelDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartTravelDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPostTravelDelegate);

UCLASS(Blueprintable)
class BITFARERS_API APersistenceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APersistenceManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* MapData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ATravelPortal> PortalClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentLevelName = FName("Spacehub");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActZoneLevelIntroData Intro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> Load;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> Unload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ATravelPortal*> Portals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AActor>> CryoFilter;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void DoWorldTransitionCleansing();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void DoWorldTransitionCleansingComplete();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void DoTravel();

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void DoTravelStart_Server();
	
	UFUNCTION(Reliable, NetMulticast)
	void DoTravelStart_Multicast();
	
	UFUNCTION(BlueprintCallable, Reliable, Server)
	void DoTravelEnd_Server();

	UFUNCTION(Reliable, NetMulticast)
	void DoTravelEnd_Multicast();

	UFUNCTION(BlueprintCallable)
	void StartMap();
	
	UFUNCTION(BlueprintCallable)
	void SpawnPortals();

	UFUNCTION()
	void UnreadyPortals(int32 PlayerId, AActor* Portal);

	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FPreTravelDelegate OnPreTravel;

	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FStartTravelDelegate OnStartTravel;

	UPROPERTY(EditAnywhere, BlueprintAssignable)
	FPostTravelDelegate OnPostTravel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EZoneDifficulty ZoneDifficulty = EZoneDifficulty::Normal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EAct ZoneAct = EAct::Other;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EZoneType ZoneType = EZoneType::Unknown;

	UFUNCTION()
	void DoCleansing(bool bAuthority);

protected:
	virtual void BeginPlay() override;

};
