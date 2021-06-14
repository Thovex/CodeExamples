// Copyright Baer and Hoggo Games

#pragma once

#include "EZoneDifficulty.h"
#include "EZoneType.h"
#include "Engine/DataTable.h"
#include "ActZoneLevelsRow.generated.h"

USTRUCT(BlueprintType)
struct FActZoneLevelIntroData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<USoundBase*, FVector2D> SoundStartEndTime;
	
};

USTRUCT(BlueprintType)
struct FActZoneLevel
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName UniqueName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FActZoneLevelIntroData IntroData;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UWorld> LevelRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EZoneDifficulty> AllowedDifficulties;
};


USTRUCT(BlueprintType)
struct FActZoneLevels
{
	GENERATED_BODY()
public:

	FActZoneLevels() {};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FActZoneLevel> Levels;
	
};

USTRUCT(BlueprintType)
struct FActZoneLevelsRow : public FTableRowBase
{
	GENERATED_BODY()
public:

	FActZoneLevelsRow()
	{
		for (EZoneType ZoneType : TEnumRange<EZoneType>())
		{
			LevelData.Add(ZoneType, FActZoneLevels());
		}
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EZoneType, FActZoneLevels> LevelData;
};
