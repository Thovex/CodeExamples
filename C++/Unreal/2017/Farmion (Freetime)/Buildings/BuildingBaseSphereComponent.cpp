// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "BuildingBaseSphereComponent.h"
#include "UnrealNetwork.h"

UBuildingBaseSphereComponent::UBuildingBaseSphereComponent() {
	SetIsReplicated(true);
	RegisterComponent();
}


bool UBuildingBaseSphereComponent::SetDetails_Validate(FName SocketName, FTransform SocketTransform, UStaticMeshComponent* MeshToAttachTo) {
	return true;
}

void UBuildingBaseSphereComponent::SetDetails_Implementation(FName SocketName, FTransform SocketTransform, UStaticMeshComponent* MeshToAttachTo) {

	//SetupAttachment(MeshToAttachTo);
	AttachToComponent(MeshToAttachTo, FAttachmentTransformRules::KeepWorldTransform, NAME_None);

	SetWorldTransform(SocketTransform);

	ShapeColor = FColor::White;
	SetSphereRadius(32);

	bVisible = false;
	bHiddenInGame = true;

	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	FString SocketNameToString = SocketName.ToString();

	if(SocketNameToString.Contains("/")) {
		FString TempStringLeftPt;
		FString TempStringRightPt;

		SocketNameToString.Split("/", &TempStringLeftPt, &TempStringRightPt);

		FName TagToAdd = FName(*TempStringLeftPt);
		BuildingSphereTags.Add(TagToAdd);

		TempStringLeftPt = "";
		TempStringRightPt.Split("_", &TempStringLeftPt, &TempStringRightPt);

		TagToAdd = FName(*TempStringLeftPt);
		BuildingSphereTags.Add(TagToAdd);


	} else {
		FString TempStringLeftPt;
		FString TempStringRightPt;

		SocketNameToString.Split("_", &TempStringLeftPt, &TempStringRightPt);

		FName TagToAdd = FName(*TempStringLeftPt);

		BuildingSphereTags.Add(TagToAdd);

	}

	RegisterComponent();
}

void UBuildingBaseSphereComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	DOREPLIFETIME(UBuildingBaseSphereComponent, BuildingSphereTags);
}