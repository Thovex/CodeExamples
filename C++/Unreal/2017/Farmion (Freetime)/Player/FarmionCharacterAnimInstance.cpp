// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "FarmionCharacterAnimInstance.h"
#include "FarmionCharacter.h"
#include "UnrealNetwork.h"
#include "Engine.h"
#include "Interfaces/ICanTakeDamage.h"
#include "Items/ToolBase.h"

void UFarmionCharacterAnimInstance::NativeInitializeAnimation() {
	Super::NativeInitializeAnimation();
}

void UFarmionCharacterAnimInstance::WoodcutHit(APawn* ThePawn) {
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Cyan, "MultiCast Call");

	if(!ThePawn) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, "ERROR WITH THE PAWN");
		return;
	}

	AFarmionCharacter* OwningFarmionCharacter = Cast<AFarmionCharacter>(ThePawn);

	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Cyan, OwningFarmionCharacter->GetName());

	if(OwningFarmionCharacter->ResourceTarget) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Cyan, OwningFarmionCharacter->ResourceTarget->GetName());
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Red, "ERROR WITH THE ResourceTarget");

	}
	

	TArray<UDestructibleComponent*> OwningDestructibleMeshes;
	TArray<UStaticMeshComponent*> OwningStaticMeshes;

	OwningFarmionCharacter->ResourceTarget->GetComponents<UStaticMeshComponent>(OwningStaticMeshes);
	OwningFarmionCharacter->ResourceTarget->GetComponents<UDestructibleComponent>(OwningDestructibleMeshes);

	for(UDestructibleComponent* DestructibleMesh : OwningDestructibleMeshes) {
		DestructibleMesh->ApplyRadiusDamage(550, OwningFarmionCharacter->GetActorLocation(), 355, 25.0F, true);
		DestructibleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		DestructibleMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	}

	for(UStaticMeshComponent* StaticMesh : OwningStaticMeshes) {
		//StaticMesh->SetSimulatePhysics(true);
	}
}

void UFarmionCharacterAnimInstance::MiningHit(APawn* ThePawn) {
	GEngine->AddOnScreenDebugMessage(-1, 5.0F, FColor::Cyan, "MINING HIT");
	AFarmionCharacter* OwningFarmionCharacter = Cast<AFarmionCharacter>(ThePawn);

	FHitResult LookingAt = OwningFarmionCharacter->CameraToCursorLT(OwningFarmionCharacter->InteractRange, false);
	AActor* LookingAtActor = LookingAt.GetActor();
	if(LookingAtActor) {
		if(LookingAtActor->GetClass()->ImplementsInterface(UICanTakeDamage::StaticClass())) {
			IICanTakeDamage::Execute_DamageEvent(LookingAtActor,Cast<AToolBase>(OwningFarmionCharacter->CurrentHeldItem));

		}
	}

}



// 
// //This function is like PostInitAnimtree in UE3
// void UFarmionCharacterAnimInstance::NativeInitializeAnimation() {
// 	//Very Important Line
// 	Super::NativeInitializeAnimation();
// 
// 	OwningPawn = TryGetPawnOwner();
// }
// 
// //Tick
// void UFarmionCharacterAnimInstance::NativeUpdateAnimation(float DeltaTimeX) {
// 	Super::NativeUpdateAnimation(DeltaTimeX);
// 
// }

