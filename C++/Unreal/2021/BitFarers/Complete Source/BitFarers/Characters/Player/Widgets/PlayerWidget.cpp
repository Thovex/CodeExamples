// Copyright Baer and Hoggo Games


#include "PlayerWidget.h"

#include "BitFarers/Macros.h"
#include "../PlayerCharacter.h"

UPlayerWidget::UPlayerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UPlayerWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	AActor* Owner = GetOwningPlayerPawn();

	if (Owner)
	{
		if (Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter = Cast<APlayerCharacter>(Owner);
		} else
		{
			PRINT("PlayerWidget on non-APlayerCharacter owner!");
		}
	}
}