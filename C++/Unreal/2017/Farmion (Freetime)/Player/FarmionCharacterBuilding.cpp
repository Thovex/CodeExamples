// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "FarmionCharacterBuilding.h"
#include "UnrealNetwork.h"
#include "Buildings/BuildingBase.h"

UFarmionCharacterBuilding::UFarmionCharacterBuilding() {
	bReplicates = true;

	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UMaterial> PreviewMaterialGood(TEXT("Material'/Game/Materials/PlayerBuilding/M_Building_Preview.M_Building_Preview'"));

	if(PreviewMaterialGood.Object != NULL) {
		BuildingMaterialGood = (UMaterial*)PreviewMaterialGood.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> PreviewMaterialBad(TEXT("Material'/Game/Materials/PlayerBuilding/M_Building_Preview_Collision.M_Building_Preview_Collision'"));

	if(PreviewMaterialBad.Object != NULL) {
		BuildingMaterialBad = (UMaterial*)PreviewMaterialBad.Object;
	}
}

void UFarmionCharacterBuilding::BeginPlay() {
	Super::BeginPlay();
	SetBeginValues();
}

void UFarmionCharacterBuilding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ConstantCheckIfHasBook();

	SlowTickrate -= DeltaTime;

	if(SlowTickrate <= 0) {
		SlowTick();
	}

	if(BuildCooldownTimer > 0) {
		BuildCooldownTimer -= DeltaTime;
	}

	if(ECurrentBuildingState == EBuildingState::EBS_Building) {
		CheckIfNullMaterial();
		ClientsideLocationCheck(DeltaTime);
	}
	DoCameraLerp();
}

void UFarmionCharacterBuilding::SelectMesh(UStaticMesh * TheMesh) {
	ECurrentBuildingState = EBuildingState::EBS_Building;

	SetInputInactive();
	CurrentBook->CloseBookWidget();

	ServerSetMesh(CurrentOwner, TheMesh);
}

void UFarmionCharacterBuilding::PressB(AFarmionCharacter* TheOwner) {
	if(ECurrentBuildingState == EBuildingState::EBS_Building) {
		ServerSetCameraBoomLow(TheOwner);
	}
	ServerPressB(TheOwner);
}

// TODO: Serverside checks for BuildObject (Right now its 100% clientside checked. 25% server checked).
void UFarmionCharacterBuilding::BuildObject(AFarmionCharacter* TheOwner) {
	if(CurrentOwner == TheOwner) {
		if(ECurrentBuildingState == EBuildingState::EBS_Building) {
			if(TheOwner && CurrentBook) {
				if(SelectedVisibility) {
					if(!SelectedCollision) {
						if(BuildCooldownTimer <= 0) {
							ServerBuildObject(TheOwner, SelectedLocation, SelectedRotation, FVector(1, 1, 1), SelectedMesh, CurrentBook->BuildingBookType, SelectedSphere);
							BuildCooldownTimer = 1.0F;
						}
					}
				}
			}
		}
		if(SelectedSphere) {
			SelectedSphere = nullptr;
		}
	}
}

void UFarmionCharacterBuilding::SetBeginValues() {
	SlowTickrate = .15F;

	bBuildActive = false;

	ECurrentBuildingState = EBuildingState::EBS_None;

	CurrentOwner = Cast<AFarmionCharacter>(GetOwner());
	CurrentOwnerPreviewMesh = CurrentOwner->PreviewBuildingMeshComp;

	BuildingMaterialGoodDyn = UMaterialInstanceDynamic::Create(BuildingMaterialGood, CurrentOwnerPreviewMesh);
	BuildingMaterialBadDyn = UMaterialInstanceDynamic::Create(BuildingMaterialBad, CurrentOwnerPreviewMesh);
}

void UFarmionCharacterBuilding::SetInputActive() {
	APlayerController* ThePlayer = GetWorld()->GetFirstPlayerController();

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetWidgetToFocus(CurrentBook->BuildingBookWidgetInstance->TakeWidget());

	ThePlayer->SetInputMode(InputMode);
	ThePlayer->bShowMouseCursor = true;
}

void UFarmionCharacterBuilding::SetInputInactive() {
	APlayerController* ThePlayer = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);

	ThePlayer->SetInputMode(InputMode);
	ThePlayer->bShowMouseCursor = false;
}

void UFarmionCharacterBuilding::SlowTick() {
	SlowTickrate = .25F;

	if(CurrentBook) {
		if(ECurrentBuildingState == EBuildingState::EBS_Building) {
			if(CurrentBook->BuildingBookStyle == EBuildingBookStyle::EBBS_Free) {
				if(FVector::Dist(CurrentLocation, SelectedLocation) > 0.5F) {
					ServerSetLocation(CurrentOwner);
				}
			} else {
				ServerSetLocation(CurrentOwner);
			}
		}
	}
}

void UFarmionCharacterBuilding::DoCameraLerp() {
	if(bCameraLerp) {
		USpringArmComponent* CameraBoom = CurrentOwner->GetCameraBoom();
		FVector CurrentSocketOffset = CameraBoom->SocketOffset;
		FVector SocketOffsetLerp = FMath::Lerp(CurrentSocketOffset, CameraBoomLerpLocation, .1F);

		CameraBoom->SocketOffset = SocketOffsetLerp;

		if(CurrentSocketOffset.Equals(CameraBoomLerpLocation, .1F)) {
			bCameraLerp = false;
		}
	}
}

void UFarmionCharacterBuilding::ConstantCheckIfHasBook() {
	if(!CurrentOwner->bIsHoldingItem && ECurrentBuildingState != EBuildingState::EBS_None) {
		ECurrentBuildingState = EBuildingState::EBS_None;
		ClientCloseBook(CurrentOwner);
		MulticastClearBook(CurrentOwner);
		SelectedVisibility = false;
		OnRep_SetVisibility();
	}
}

void UFarmionCharacterBuilding::CheckIfNullMaterial() {
	if(CurrentOwnerPreviewMesh->GetMaterial(0) == nullptr) {
		SelectedCollision = true;
		OnRep_SetCollision();
	}
}

void UFarmionCharacterBuilding::CheckPreviewCollisions() {
	if(CurrentBook) {
		if(CurrentBook->BuildingBookStyle == EBuildingBookStyle::EBBS_Free) {
			if(CurrentOwnerPreviewMesh) {
				TArray<AActor*> OverlappingActors;
				CurrentOwnerPreviewMesh->GetOverlappingActors(OverlappingActors);

				if(OverlappingActors.Num() > 0) {
					if(!SelectedCollision) {
						ServerCheckPreviewCollisions(true);
					}
				} else {
					if(SelectedCollision) {
						ServerCheckPreviewCollisions(false);
					}
				}
			}
		}
	}
}

void UFarmionCharacterBuilding::ClientsideLocationCheck(float DeltaTime) {
	FHitResult TheHitResult = CurrentOwner->CameraToCursorLT(2500, false);
	if(TheHitResult.IsValidBlockingHit()) {
		if(CurrentBook) {
			switch(CurrentBook->BuildingBookStyle) {
				case EBuildingBookStyle::EBBS_Free:
				{
					CurrentLocation = TheHitResult.Location;
					FVector LerpedLocation = FMath::Lerp(CurrentLocation, SelectedLocation, DeltaTime * 3.0F);
					CurrentOwnerPreviewMesh->SetWorldLocation(LerpedLocation);
					CheckPreviewCollisions();
				}
				break;
				case EBuildingBookStyle::EBBS_Snap:
				{
					if(SelectedCollision) {
						SelectedCollision = false;
						OnRep_SetCollision();
					}
					if(TheHitResult.GetComponent()->GetAttachmentRootActor() != NULL) {
						if(TheHitResult.GetActor()->IsA(ABuildingBase::StaticClass())) {
							ABuildingBase* BuildingBase = Cast<ABuildingBase>(TheHitResult.GetActor());
							BuildingBase->AddType(CurrentBook->BuildingBookType);
						}
						CurrentOwnerPreviewMesh->SetWorldLocation(SelectedLocation);
					}
				}
				break;
				default:
				{

				}
				break;
			}

		}
	}
}

void UFarmionCharacterBuilding::OnRep_SetMesh() {
	if(CurrentOwnerPreviewMesh) {
		ECurrentBuildingState = EBuildingState::EBS_Building;
		CurrentOwnerPreviewMesh->SetStaticMesh(SelectedMesh);
	}
}

void UFarmionCharacterBuilding::OnRep_SetCollision() {
	if(CurrentOwnerPreviewMesh) {
		int32 NumMaterials = CurrentOwnerPreviewMesh->GetNumMaterials();

		if(NumMaterials) {
			for(int i = 0; i < NumMaterials; i++) {
				if(SelectedCollision) {
					CurrentOwnerPreviewMesh->SetMaterial(i, BuildingMaterialBadDyn);
				} else {
					CurrentOwnerPreviewMesh->SetMaterial(i, BuildingMaterialGoodDyn);
				}
			}
		}
	}
}

void UFarmionCharacterBuilding::OnRep_SetRotation() {
	if(CurrentOwnerPreviewMesh) {
		CurrentOwnerPreviewMesh->SetRelativeRotation(SelectedRotation);
	}
}

void UFarmionCharacterBuilding::OnRep_SetVisibility() {
	if(CurrentOwnerPreviewMesh) {
		CurrentOwnerPreviewMesh->SetVisibility(SelectedVisibility);
	}
}

void UFarmionCharacterBuilding::OnRep_SetLocation() {

}

bool UFarmionCharacterBuilding::ServerRotateObject_Validate(FRotator PlusRotation) {
	return true;
}

void UFarmionCharacterBuilding::ServerRotateObject_Implementation(FRotator PlusRotation) {
	SelectedRotation += PlusRotation;
	OnRep_SetRotation();
}

bool UFarmionCharacterBuilding::ServerCheckPreviewCollisions_Validate(bool Active) {
	return true;
}

void UFarmionCharacterBuilding::ServerCheckPreviewCollisions_Implementation(bool Active) {
	SelectedCollision = Active;
	OnRep_SetCollision();
}

bool UFarmionCharacterBuilding::ServerSetCameraBoomHigh_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::ServerSetCameraBoomHigh_Implementation(AFarmionCharacter* TheOwner) {
	USpringArmComponent* CameraBoom = TheOwner->GetCameraBoom();

	CameraBoom->TargetArmLength = 300;
	CameraBoomLerpLocation = FVector(-300, 0, 150);

	bCameraLerp = true;
}

bool UFarmionCharacterBuilding::ServerSetCameraBoomLow_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::ServerSetCameraBoomLow_Implementation(AFarmionCharacter* TheOwner) {
	USpringArmComponent* CameraBoom = TheOwner->GetCameraBoom();

	CameraBoom->TargetArmLength = 300;
	CameraBoomLerpLocation = FVector::ZeroVector;

	bCameraLerp = true;
}

bool UFarmionCharacterBuilding::ServerSetMesh_Validate(AFarmionCharacter* TheOwner, UStaticMesh* TheMesh) {
	return true;
}

void UFarmionCharacterBuilding::ServerSetMesh_Implementation(AFarmionCharacter* TheOwner, UStaticMesh* TheMesh) {
	SelectedMesh = TheMesh;
	ServerSetCameraBoomHigh(TheOwner);

	SelectedVisibility = true;
	OnRep_SetVisibility();
	OnRep_SetMesh();
}

bool UFarmionCharacterBuilding::ServerSetLocation_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::ServerSetLocation_Implementation(AFarmionCharacter* TheOwner) {
	if(CurrentOwner == TheOwner) {
		FHitResult TheHitResult = TheOwner->CameraToCursorLT(2500, false);

		if(TheHitResult.IsValidBlockingHit()) {
			if(TheHitResult.GetActor()) {
				if(CurrentBook) {
					switch(CurrentBook->BuildingBookStyle) {
						case EBuildingBookStyle::EBBS_Free:
						{
							SelectedLocation = TheHitResult.Location;
							OnRep_SetLocation();
						}
							break;
						case EBuildingBookStyle::EBBS_Snap:
						{
							if(TheHitResult.GetComponent()->GetAttachmentRootActor() != NULL) {
								if(TheHitResult.GetActor()->IsA(ABuildingBase::StaticClass())) {
									ABuildingBase* BuildingBase = Cast<ABuildingBase>(TheHitResult.GetActor());
									BuildingBase->AddType(CurrentBook->BuildingBookType);
									if(TheHitResult.GetComponent()->IsA(UBuildingBaseSphereComponent::StaticClass())) {
										TheOwner->FarmionBuilding->SelectedSphere = Cast<UBuildingBaseSphereComponent>(TheHitResult.GetComponent());
										FString BookType = GetEnumValueToString<EBuildingBookType>("EBuildingBookType", CurrentBook->BuildingBookType);
										TArray<FString> StringArray;
										BookType.ParseIntoArray(StringArray, TEXT("EBBT_"), true);
										BookType = StringArray[1];

										if(SelectedSphere->BuildingSphereTags.Contains(FName(*BookType))) {
											SelectedCollision = true;
											OnRep_SetCollision();
											SelectedVisibility = true;
											OnRep_SetVisibility();

											FTransform SphereTransform = SelectedSphere->GetComponentTransform();

											SelectedLocation = SphereTransform.GetLocation();
											SelectedRotation = FRotator(SphereTransform.GetRotation());
											OnRep_SetLocation();
											OnRep_SetRotation();
										}
									} else {
										SelectedVisibility = false;
										OnRep_SetVisibility();
									}
								} else {
									SelectedVisibility = false;
									OnRep_SetVisibility();
								}
							}
						}
						break;
						default:
							break;
					}
				}
			}
		}
	}
}

bool UFarmionCharacterBuilding::ServerBuildObject_Validate(AFarmionCharacter* TheOwner, FVector TheLocation, FRotator TheRotation, FVector TheScale, UStaticMesh* TheMesh, EBuildingBookType TheBuildingBookType, UBuildingBaseSphereComponent* TheSphere) {
	return true;
}

void UFarmionCharacterBuilding::ServerBuildObject_Implementation(AFarmionCharacter* TheOwner, FVector TheLocation, FRotator TheRotation, FVector TheScale, UStaticMesh* TheMesh, EBuildingBookType TheBuildingBookType, UBuildingBaseSphereComponent* TheSphere) {
	if(TheOwner->PreviewBuildingMeshComp->IsVisible()) {
		if(TheSphere) {
			ABuildingBase* SelectedBuilding = Cast<ABuildingBase>(TheSphere->GetOwner());
			SelectedBuilding->RemoveSphere(TheSphere);
			SelectedSphere = nullptr;
		}

		TheOwner->FarmionBuilding->BuildCooldownTimer = 1.0F;

		FActorSpawnParameters SpawnInfo;
		ABuildingBase* CreatedBuilding = GetWorld()->SpawnActor<ABuildingBase>(TheLocation, TheRotation, SpawnInfo);

		CreatedBuilding->BuildingMesh = TheMesh;
		CreatedBuilding->BuildingType = TheBuildingBookType;

		CreatedBuilding->OnRep_SetBuildingMesh();
	}
}

bool UFarmionCharacterBuilding::ServerPressB_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::ServerPressB_Implementation(AFarmionCharacter* TheOwner) {
	switch(TheOwner->FarmionBuilding->ECurrentBuildingState) {
		case EBuildingState::EBS_None:
		{
			bBuildActive = true;
			ECurrentBuildingState = EBuildingState::EBS_Selecting;
			MulticastGetBook(TheOwner);
			ClientOpenBook(TheOwner);

		}
		break;
		case EBuildingState::EBS_Selecting:
		{

			bBuildActive = false;
			ECurrentBuildingState = EBuildingState::EBS_None;
			ClientCloseBook(TheOwner);
			MulticastClearBook(TheOwner);
			SelectedVisibility = false;
			OnRep_SetVisibility();
		}
		break;
		case EBuildingState::EBS_Building:
		{
			bBuildActive = true;
			ECurrentBuildingState = EBuildingState::EBS_Selecting;
			SelectedVisibility = false;
			ClientOpenBook(TheOwner);
			OnRep_SetVisibility();
		}
		break;
		default:
			break;
	}
}

bool UFarmionCharacterBuilding::MulticastGetBook_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::MulticastGetBook_Implementation(AFarmionCharacter* TheOwner) {
	if(CurrentOwner == TheOwner) {
		CurrentBook = Cast<ABuildingBookBase>(CurrentOwner->CurrentHeldItem);
	}
}

bool UFarmionCharacterBuilding::MulticastClearBook_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::MulticastClearBook_Implementation(AFarmionCharacter* TheOwner) {
	if(CurrentOwner == TheOwner) {
		CurrentBook = nullptr;
	}
}

bool UFarmionCharacterBuilding::ClientOpenBook_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::ClientOpenBook_Implementation(AFarmionCharacter* TheOwner) {
	if(CurrentOwner == TheOwner) {
		CurrentBook->OpenBookWidget();
		SetInputActive();
	}
}

bool UFarmionCharacterBuilding::ClientCloseBook_Validate(AFarmionCharacter* TheOwner) {
	return true;
}

void UFarmionCharacterBuilding::ClientCloseBook_Implementation(AFarmionCharacter* TheOwner) {
	if(CurrentOwner == TheOwner) {
		CurrentBook->CloseBookWidget();
		SetInputInactive();
		SelectedVisibility = false;
		ServerSetCameraBoomLow(TheOwner);

		OnRep_SetVisibility();
	}
}

void UFarmionCharacterBuilding::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	DOREPLIFETIME(UFarmionCharacterBuilding, bBuildActive);
	DOREPLIFETIME(UFarmionCharacterBuilding, ECurrentBuildingState);

	DOREPLIFETIME(UFarmionCharacterBuilding, bCameraLerp);
	DOREPLIFETIME(UFarmionCharacterBuilding, CameraBoomLerpLocation);

	DOREPLIFETIME(UFarmionCharacterBuilding, CurrentBook);
	DOREPLIFETIME(UFarmionCharacterBuilding, CurrentOwner);
	DOREPLIFETIME(UFarmionCharacterBuilding, CurrentBook);

	DOREPLIFETIME(UFarmionCharacterBuilding, SelectedMesh);
	DOREPLIFETIME(UFarmionCharacterBuilding, SelectedLocation);
	DOREPLIFETIME(UFarmionCharacterBuilding, SelectedCollision);
	DOREPLIFETIME(UFarmionCharacterBuilding, SelectedRotation);
	DOREPLIFETIME(UFarmionCharacterBuilding, SelectedVisibility);
	DOREPLIFETIME(UFarmionCharacterBuilding, SelectedSphere);

	DOREPLIFETIME(UFarmionCharacterBuilding, BuildCooldownTimer);
}


