// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "FarmionCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Items/ItemBase.h"
#include "FarmionMacros.h"
#include "UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/ToolBase.h"
#include "Objects/Object_Button.h"
#include "Player/FarmionCharacterBuilding.h"
#include "Private/KismetTraceUtils.h"

DEFINE_LOG_CATEGORY(FarmionCharacterLog);

class UFarmionCharacterBuilding;

AFarmionCharacter::AFarmionCharacter() {
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	TArray<USkeletalMeshComponent*> SkeletalMeshComps;
	GetComponents<USkeletalMeshComponent>(SkeletalMeshComps);

	for(int i = 0; i < 4; i++) {
		FName IKName = TEXT("DefaultIKHandleName");

		switch(i) {
			case 0:
				IKName = TEXT("IK_LeftHand");
				break;
			case 1:
				IKName = TEXT("IK_RightHand");
				break;
			case 2:
				IKName = TEXT("IK_LeftElbow");
				break;
			case 3:
				IKName = TEXT("IK_RightElbow");
				break;
		}
		USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(IKName);

		SceneComponent->ComponentTags.Add(TEXT("IK"));
		SceneComponent->AttachToComponent(SkeletalMeshComps[0], FAttachmentTransformRules::KeepWorldTransform, NAME_None);

		IKComponents.Add(SceneComponent);

	}

	FarmionBuilding = CreateDefaultSubobject<UFarmionCharacterBuilding>(TEXT("Farmion Building"));

	PreviewBuildingMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("PreviewBuildingMeshComp");
	PreviewBuildingMeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AFarmionCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFarmionCharacter::Interact);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AFarmionCharacter::Throw);
	PlayerInputComponent->BindAction("Build", IE_Pressed, this, &AFarmionCharacter::Build);

	PlayerInputComponent->BindAction("MWheelDown", IE_Pressed, this, &AFarmionCharacter::MWheelDown);
	PlayerInputComponent->BindAction("MWheelUp", IE_Pressed, this, &AFarmionCharacter::MWheelUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AFarmionCharacter::LeftMouseButton);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AFarmionCharacter::LeftMouseButtonRelease);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFarmionCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFarmionCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFarmionCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFarmionCharacter::LookUpAtRate);
}

void AFarmionCharacter::Interact() {
	if(!CurrentHeldItem) {
		InteractPickup();
		InteractPress();
	} else {
		InteractDrop(false);
	}
}

void AFarmionCharacter::Throw() {
	if(CurrentHeldItem) {
		InteractDrop(true);
	}
}

void AFarmionCharacter::Build() {
	if(CurrentHeldItem) {
		if(CurrentHeldItem->bIsBuildingItem) {
			FarmionBuilding->PressB(this);
		}
	}
}

void AFarmionCharacter::MWheelUp() {
	if(CurrentHeldItem) {
		if(CurrentHeldItem->bIsBuildingItem) {
			FarmionBuilding->ServerRotateObject(FRotator(0, -5, 0));
		}
	}
}

void AFarmionCharacter::MWheelDown() {
	if(CurrentHeldItem) {
		if(CurrentHeldItem->bIsBuildingItem) {
			FarmionBuilding->ServerRotateObject(FRotator(0, 5, 0));
		}
	}
}

void AFarmionCharacter::LeftMouseButton() {
	ServerLeftMouseEvent();
}

void AFarmionCharacter::LeftMouseButtonRelease() {
	ServerLeftMouseEvent();
	/*if(bIsWoodcutting) {
		ServerStopWoodcutting();
	}*/
}

void AFarmionCharacter::ServerLeftMouseEvent_Implementation() {
	LeftMouseButtonDown = !LeftMouseButtonDown;
	if(LeftMouseButtonDown) {
		if(CurrentHeldItem) {
			if(CurrentHeldItem->bIsBuildingItem) {
				FarmionBuilding->BuildObject(this);
			}

			if(CurrentHeldItem->bIsToolItem) {
				AToolBase* CurrentHeldTool = Cast<AToolBase>(CurrentHeldItem);
				switch(CurrentHeldTool->CurrentToolType) {
					case EToolType::ETT_Hatchet:
					{
						bIsWoodcutting = true;
						MulticastPushToAnimator("bIsWoodcutting", true);
					}
					break;
					case EToolType::ETT_Pickaxe:
					{
						bIsMining = true;
						MulticastPushToAnimator("bIsMining", true);
					}
					break;
					default:
					{

					}
					break;
				}
			}

		}


	} else {
		if(bIsWoodcutting) {
			bIsWoodcutting = false;
			MulticastPushToAnimator("bIsWoodcutting", false);
		} else if (bIsMining) {
			bIsMining = false;
			MulticastPushToAnimator("bIsMining", false);
		}
		
	}
}

void AFarmionCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(bIsHoldingItem) {
		SetIKData();
	}
	if(!HasAuthority() && !IsLocallyControlled()) {
		PlayerBelowObjectEnableCollision();
	}
}

void AFarmionCharacter::MoveForward(float Value) {
	if((Controller != NULL) && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFarmionCharacter::MoveRight(float Value) {
	if((Controller != NULL) && (Value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void AFarmionCharacter::TurnAtRate(float Rate) {
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFarmionCharacter::LookUpAtRate(float Rate) {
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FVector AFarmionCharacter::CameraToCursorVector(int32 VectorLength, bool StartVector) const {
	FVector ReturnVector = FVector::ZeroVector;
	FTransform CameraTransform = GetFollowCamera()->GetComponentTransform();

	if(StartVector) {
		return CameraTransform.GetLocation();
	} else if(!StartVector) {
		return ReturnVector = (CameraTransform.GetRotation().GetForwardVector() * VectorLength) + CameraTransform.GetLocation();
	}
	return ReturnVector;
}

FHitResult AFarmionCharacter::CameraToCursorLT(int32 LT_Distance, bool Debug) const {
	FCollisionQueryParams LT_TraceParams = FCollisionQueryParams(FName(TEXT("LT_HitResult")), true, this);
	LT_TraceParams.bTraceComplex = true;
	LT_TraceParams.bTraceAsyncScene = true;
	LT_TraceParams.bReturnPhysicalMaterial = false;

	FHitResult LT_HitResult(ForceInit);

	FVector StartTrace = CameraToCursorVector(0, true);
	FVector EndTrace = CameraToCursorVector(LT_Distance, false);

	GetWorld()->LineTraceSingleByChannel(LT_HitResult, StartTrace, EndTrace, ECC_Visibility, LT_TraceParams);

	if(Debug) {
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, .1f, 1, 1.0F);

		UE_LOG(FarmionCharacterLog, Warning, TEXT("___________________Camera to Cursor LT Debug Results___________________"));

		UE_LOG(FarmionCharacterLog, Display, TEXT("Distance: %f"), LT_HitResult.Distance);
		UE_LOG(FarmionCharacterLog, Display, TEXT("Impact Point: %s"), *LT_HitResult.ImpactPoint.ToString());

		if(LT_HitResult.GetActor()) {
			UE_LOG(FarmionCharacterLog, Display, TEXT("Actor Name: %s"), *LT_HitResult.GetActor()->GetName());
			UE_LOG(FarmionCharacterLog, Display, TEXT("Actor Class: %s"), *LT_HitResult.GetActor()->GetClass()->GetName());

			UE_LOG(FarmionCharacterLog, Display, TEXT("Component Name: %s"), *LT_HitResult.GetComponent()->GetName());
			UE_LOG(FarmionCharacterLog, Display, TEXT("Component Class: %s"), *LT_HitResult.GetComponent()->GetClass()->GetName());
		}
	}

	return LT_HitResult;
}

void AFarmionCharacter::PlayerBelowObjectEnableCollision() {

	UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation(),
		FVector(80, 80, 10),
		GetActorRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitActors,
		false,
		FLinearColor(255, 255, 0),
		FLinearColor(255, 0, 255),
		-1
	);
	
	LastAddedCollisionActors.Empty();
	for(int i = 0; i < HitActors.Num(); i++) {
		if(HitActors[i].GetComponent()->GetAttachmentRootActor() != NULL) {
			if(HitActors[i].GetActor()->IsA(AItemBase::StaticClass())) {
				MoveIgnoreActorAdd(HitActors[i].GetActor());
				LastAddedCollisionActors.Add(HitActors[i].GetActor());
				LastRemovedCollisionActors.Remove(HitActors[i].GetActor());
			}
		}
	}

	for(int i = 0; i < LastRemovedCollisionActors.Num(); i++) {
		MoveIgnoreActorRemove(LastRemovedCollisionActors[i]);
	}
	LastRemovedCollisionActors = LastAddedCollisionActors;

}

void AFarmionCharacter::ServerStartWoodcutting_Implementation() {
	FHitResult LookingAt = CameraToCursorLT(InteractRange, false);

	if(LookingAt.GetActor()) {
		AActor* LookingAtActor = LookingAt.GetActor();

		if(LookingAtActor->GetClass()->ImplementsInterface(UIWoodcutAble::StaticClass())) {
			ResourceTarget = Cast<AResourceBase>(LookingAtActor);
			if(ResourceTarget) {
				GEngine->AddOnScreenDebugMessage(-1, 1.0F, FColor::Red, "Resource Target");
			}
			bIsWoodcutting = true;
			MulticastPushToAnimator("bIsWoodcutting", true);
		} else {
			bIsWoodcutting = false;
			MulticastPushToAnimator("bIsWoodcutting", false);
		}
	} else {
		bIsWoodcutting = false;
		MulticastPushToAnimator("bIsWoodcutting", false);
	}
}

void AFarmionCharacter::ServerStopWoodcutting_Implementation() {
	MulticastPushToAnimator("bIsWoodcutting", false);
}

void AFarmionCharacter::InteractPress() {
	ServerInteractPress();
}

void AFarmionCharacter::ServerInteractPress_Implementation() {
	FHitResult LookingAt = CameraToCursorLT(InteractRange, false);
	AActor* LookingAtActor = LookingAt.GetActor();

	if(LookingAtActor) {
		if(LookingAtActor->GetClass()->ImplementsInterface(UIButtonPressAble::StaticClass())) {
			AObject_Button* ButtonObject = Cast<AObject_Button>(LookingAtActor);

			ButtonObject->ButtonPress();
		}
	}
}

void AFarmionCharacter::InteractPickup() {
	ServerInteractPickup();
}

void AFarmionCharacter::ServerInteractPickup_Implementation() {
	FHitResult LookingAt = CameraToCursorLT(InteractRange, false);
	AActor* LookingAtActor = LookingAt.GetActor();
	if(LookingAtActor) {
		if(LookingAtActor->GetClass()->ImplementsInterface(UIPickupAble::StaticClass())) {
			AItemBase* ItemBase = Cast<AItemBase>(LookingAtActor);

			if(!ItemBase->bIsPickedUp && ItemBase->bPickUpEnabled) {
				CurrentHeldItem = ItemBase;
				CurrentHeldItem->PickUp(FindComponentByClass<USkeletalMeshComponent>());
				bIsHoldingItem = true;
				this->MoveIgnoreActorAdd(CurrentHeldItem);
			}
		}
	}
}

void AFarmionCharacter::InteractDrop(bool bIsTossed) {
	ServerInteractDropData(bIsTossed);
}

void AFarmionCharacter::ServerInteractDropData_Implementation(bool bIsTossed) {
	CurrentHeldItem->Drop();

	if(bIsTossed) {
		if(bIsHoldingItem && CurrentHeldItem) {

			AItemBase* TempBase = CurrentHeldItem;
			FTransform CameraTransform = GetFollowCamera()->GetComponentTransform();
			FVector ThrowVelocity = CameraToCursorVector(ThrowForce, false);

			TArray<UStaticMeshComponent*> HeldItemMeshComps;
			TempBase->GetComponents<UStaticMeshComponent>(HeldItemMeshComps);

			if(HeldItemMeshComps.Num() > 0) {
				HeldItemMeshComps[0]->AddForce(ThrowVelocity, NAME_None, false);
			}
		}
	}

	this->MoveIgnoreActorRemove(CurrentHeldItem);

	bBuildingPressedState = !bBuildingPressedState;
	bIsHoldingItem = false;
	CurrentHeldItem = nullptr;
	CurrentHeldItemMeshComp = nullptr;
}


void AFarmionCharacter::SetIKData() {
	CurrentHeldItemMeshComp = CurrentHeldItem->FindComponentByClass<UStaticMeshComponent>();
	/*if (!CurrentHeldItemMeshComp) {
		CurrentHeldItemMeshComp = CurrentHeldItem->FindComponentByClass<UStaticMeshComponent>();
	}*/

	FTransform LeftHandTransform = CurrentHeldItemMeshComp->GetSocketTransform(TEXT("LeftHand"), RTS_World);
	FTransform RightHandTransform = CurrentHeldItemMeshComp->GetSocketTransform(TEXT("RightHand"), RTS_World);
	FTransform LeftElbowTransform = CurrentHeldItemMeshComp->GetSocketTransform(TEXT("LeftElbow"), RTS_World);
	FTransform RightElbowTransform = CurrentHeldItemMeshComp->GetSocketTransform(TEXT("RightElbow"), RTS_World);

	for(int i = 0; i < IKComponents.Num(); i++) {
		switch(i) {
			case 0:
				IKComponents[i]->SetWorldTransform(LeftHandTransform);
				break;
			case 1:
				IKComponents[i]->SetWorldTransform(RightHandTransform);
				break;
			case 2:
				IKComponents[i]->SetWorldTransform(LeftElbowTransform);
				break;
			case 3:
				IKComponents[i]->SetWorldTransform(RightElbowTransform);
				break;
			default:
				break;
		}

	}
}

void AFarmionCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	if(GetMesh() && (GetRemoteRole() == ROLE_AutonomousProxy && GetNetConnection() != nullptr) && bReplicateMovement) {
		GetMesh()->bOnlyAllowAutonomousTickPose = true;
	} else {
		GetMesh()->bOnlyAllowAutonomousTickPose = false;
	}
}

void AFarmionCharacter::MulticastPushToAnimator_Implementation(FName BoolName, bool BoolValue) {
	PushToAnimator(BoolName, BoolValue);
}

void AFarmionCharacter::PushToAnimator_Implementation(FName BoolName, bool BoolValue) {
	// Soley used in blueprints.
}


bool AFarmionCharacter::ServerLeftMouseEvent_Validate() {
	return true;
}

bool AFarmionCharacter::ServerInteractDropData_Validate(bool bIsTossed) {
	return true;
}

bool AFarmionCharacter::ServerStartWoodcutting_Validate() {
	return true;
}

bool AFarmionCharacter::ServerStopWoodcutting_Validate() {
	return true;
}

bool AFarmionCharacter::ServerInteractPress_Validate() {
	return true;
}

bool AFarmionCharacter::ServerInteractPickup_Validate() {
	return true;
}

bool AFarmionCharacter::MulticastPushToAnimator_Validate(FName BoolName, bool BoolValue) {
	return true;
}

void AFarmionCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFarmionCharacter, bIsHoldingItem);
	DOREPLIFETIME(AFarmionCharacter, CurrentHeldItem);
	DOREPLIFETIME(AFarmionCharacter, ResourceTarget);
}