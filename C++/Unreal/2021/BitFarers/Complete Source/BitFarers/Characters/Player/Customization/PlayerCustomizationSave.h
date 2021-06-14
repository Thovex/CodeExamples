// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerCustomizationSave.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerCustomizationColors
{
	GENERATED_BODY()

public:
	FPlayerCustomizationColors(){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customization")
	FLinearColor UserPrimaryColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customization")
	FLinearColor UserSecondaryColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customization")
    FLinearColor UserTertiaryColor = FLinearColor::Black;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customization")
    FLinearColor UserQuaternaryColor = FLinearColor::White;
};


UCLASS()
class UPlayerCustomizationSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Customization")
	FPlayerCustomizationColors Colors;
};