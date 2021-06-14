// Copyright Baer and Hoggo Games


#include "PlayerCrosshairWidget.h"
#include "BitFarers/Macros.h"

void UPlayerCrosshairWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RETURN_IF_NULLPTR(PlayerCharacter);

	//PlayerCharacter->OnHit.AddDynamic(this, &UPlayerCrosshairWidget::HitTarget);
}

//void UPlayerCrosshairWidget::HitTarget_Implementation(const EWeaponDamageType DamageType, const float Damage, const bool bCritical, const AActor* Actor)
//{
	
//}
