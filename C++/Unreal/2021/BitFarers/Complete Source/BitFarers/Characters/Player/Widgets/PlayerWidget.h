// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../PlayerCharacter.h"
#include "PlayerWidget.generated.h"

class APlayerCharacter;

/**
 * 
 */
UCLASS(BlueprintType)
class BITFARERS_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerWidget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacter* PlayerCharacter;

protected:
	virtual void NativePreConstruct() override;
};
