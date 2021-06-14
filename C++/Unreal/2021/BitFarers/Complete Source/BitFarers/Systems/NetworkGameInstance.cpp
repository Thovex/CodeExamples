// Copyright Baer and Hoggo Games


#include "NetworkGameInstance.h"

#include "Kismet/GameplayStatics.h"

UPlayerCustomizationSave* UNetworkGameInstance::GetPlayerCustomizationSave()
{
	if (UGameplayStatics::DoesSaveGameExist("CosmeticsConfig", 0))
	{
		return Cast<UPlayerCustomizationSave>(UGameplayStatics::LoadGameFromSlot("CosmeticsConfig", 0));	
	} 
	return Cast<UPlayerCustomizationSave>(UGameplayStatics::CreateSaveGameObject(UPlayerCustomizationSave::StaticClass()));
}
