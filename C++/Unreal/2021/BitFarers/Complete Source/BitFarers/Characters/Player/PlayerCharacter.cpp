// Copyright Baer and Hoggo Games


#include "PlayerCharacter.h"

#include "BitFarers/Macros.h"
#include "BitFarers/Systems/Movement/CustomCharacterMovement.h"

#include "BitFarers/Utilities/Components/LerpComponent.h"
#include "BitFarers/Utilities/Components/SoundCasterComponent.h"
#include "BitFarers/Weapons/Ammo/AmmoComponent.h"
#include "BitFarers/Characters/Player/WeaponComponent.h"
#include "BitFarers/Systems/NetworkGameInstance.h"
#include "Customization/PlayerCustomizationSave.h"

#include "Net/UnrealNetwork.h"

#include "GameFramework/Character.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(
	ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovement>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	if (GetMesh())
	{
		GetMesh()->DestroyComponent();
		GetMesh()->SetActive(false);
	}

	// -- Leg Construction --
	INI_COMP_ATTACH(PlayerLegsSpringArm, RootComponent, USpringArmComponent, "PlayerLegsSpringArm");
	{
		PlayerLegsSpringArm->TargetOffset = FVector::ZeroVector;
	}
	INI_COMP_ATTACH(PivotPlayerLegs, PlayerLegsSpringArm, USceneComponent, "PivotPlayerLegs")
	INI_COMP_ATTACH(PlayerLegsSK, PivotPlayerLegs, USkeletalMeshComponent, "PlayerLegsSK");

	// -- Body Construction --
	INI_COMP(PlayerBodySpringArm, USpringArmComponent, "PlayerBodySpringArm")
	{
		PlayerBodySpringArm->TargetOffset = FVector::ZeroVector;
		PlayerBodySpringArm->SetupAttachment(PlayerLegsSK, FName("pelvis"));
	}

	INI_COMP_ATTACH(PivotPlayerBody, PlayerBodySpringArm, USceneComponent, "PivotPlayerBody");
	INI_COMP_ATTACH(PlayerBodySM, PivotPlayerBody, UStaticMeshComponent, "PlayerBodySM");
	{
		PlayerBodySM->SetOwnerNoSee(true);
	}

	INI_COMP_ATTACH(PlayerWeaponArmSK, PivotPlayerBody, USkeletalMeshComponent, "PlayerWeaponArmSK");
	{
		PlayerWeaponArmSK->SetOwnerNoSee(true);
		PlayerWeaponArmSK->bCastHiddenShadow = true;
	}

	INI_COMP_ATTACH(PlayerSkillArmSK, PivotPlayerBody, USkeletalMeshComponent, "PlayerSkillArmSK");
	{
		PlayerSkillArmSK->SetOwnerNoSee(true);
		PlayerSkillArmSK->bCastHiddenShadow = true;
	}

	INI_COMP_ATTACH(Camera_SpringArm, RootComponent, USpringArmComponent, "Camera_SpringArm");
	INI_COMP_ATTACH(PlayerCamera, Camera_SpringArm, UCameraComponent, "PlayerCamera");

	INI_COMP_ATTACH(FPS_SpringArm, PlayerCamera, USpringArmComponent, "FPS_SpringArm");
	{
		FPS_SpringArm->CameraRotationLagSpeed = 25.0F;
		FPS_SpringArm->bEnableCameraRotationLag = true;
	}

	INI_COMP_ATTACH(FPS_WeaponArmSK, FPS_SpringArm, USkeletalMeshComponent, "FPS_WeaponArmSK_New");
	{
		FPS_WeaponArmSK->SetOnlyOwnerSee(true);
		FPS_WeaponArmSK->SetCastShadow(false);
	}

	INI_COMP_ATTACH(FPS_SkillArmSK, FPS_SpringArm, USkeletalMeshComponent, "FPS_SkillArmSK");
	{
		FPS_SkillArmSK->SetOnlyOwnerSee(true);
		FPS_SkillArmSK->SetCastShadow(false);
	}

	// -- Components --
	//INI_COMP(PlayerStatus, UStatusComponent, "PlayerStatus");
	//INI_COMP(PlayerTraits, UOldTraitComponent, "PlayerTraits");

	INI_COMP(WeaponComponent, UWeaponComponent, "Weapon");
	INI_COMP(AmmoComponent, UAmmoComponent, "AmmoComponent");
	INI_COMP(StatusComponent, UStatusComponent, "Status");

	//INI_COMP(PlayerInventory, UPlayerInventory, "PlayerInventory");

	INI_COMP(SoundCasterComponent, USoundCasterComponent, "PlayerAudio");

	//INI_COMP(PlayerSelecting, UPlayerSelecting, "PlayerSelecting");
	INI_COMP(Lerp, ULerpComponent, "PlayerLerp");

	INI_COMP(SmoothSync, USmoothSync, "SmoothSync");

	//INI_COMP(PlayerLevel, UPlayerLevel, "PlayerLevel");

}

void APlayerCharacter::BeginInitialize_Implementation()
{
	bInitialized = true;
}

void APlayerCharacter::TransferOwnership_Implementation(AActor* Target)
{
	if (Target) {
		Target->SetOwner(this);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire_Start);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnFire_End);

	PlayerInputComponent->BindAction("SetWeaponMode", IE_Pressed, this, &APlayerCharacter::SetWeaponMode);

	PlayerInputComponent->BindAction<FSwapWeaponIndex>("Weapon1", IE_Pressed, this,
                                                       &APlayerCharacter::SwapWeapon, 0);
	PlayerInputComponent->BindAction<FSwapWeaponIndex>("Weapon2", IE_Pressed, this,
                                                       &APlayerCharacter::SwapWeapon, 1);
	PlayerInputComponent->BindAction<FSwapWeaponIndex>("Weapon3", IE_Pressed, this,
                                                       &APlayerCharacter::SwapWeapon, 2);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerCharacter::OnFire_Start()
{
	if (WeaponComponent)
	{
		WeaponComponent->OnFire_Start_Request();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerCharacter::OnFire_End()
{
	if (WeaponComponent)
	{
		WeaponComponent->OnFire_End_Request();
	}
}


// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerCharacter::SetWeaponMode()
{
	if (WeaponComponent)
	{
		WeaponComponent->OnSwapWeaponMode_Request();
	}
}

// SwapWeapon Wrapper
// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerCharacter::SwapWeapon(int32 WeaponIndex)
{
	if (WeaponComponent)
	{
		WeaponComponent->SwapWeapon_Request(WeaponIndex);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APlayerCharacter::Reload()
{
	if (WeaponComponent && AmmoComponent)
	{
		WeaponComponent->Reload_Request();
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!bInitialized)
	{
		bInitialized = true;
		
		BeginInitialize();

		if (IsLocallyControlled())
		{
			Execute_InitializeStatus(this);
		}
	}

	SetCosmetics_Request();
}


float APlayerCharacter::TakeDamage_Implementation(const EElementalType ElementalType, const float Damage,
	const AActor* DamageInstigator, const UActorComponent* ImpactComponent,
    const FVector ImpactPoint)
{
	return StatusComponent->TakeDamage(ElementalType, Damage, DamageInstigator, ImpactComponent, ImpactPoint);
}

bool APlayerCharacter::IsPlayer_Implementation()
{
	return true;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const FVector Velocity = GetVelocity();

	if (Velocity.Size() > 0.1F)
	{
		if (!GetCharacterMovement()->IsFalling())
		{
			FRotator LegRotation = UKismetMathLibrary::MakeRotFromX(Velocity);
			LegRotation.Pitch = 0;
			LegRotation.Roll = 0;

			PivotPlayerLegs->SetWorldRotation(FMath::Lerp(PivotPlayerLegs->GetComponentRotation(), LegRotation,
                                                          DeltaSeconds * 10));
		}

		const float MoveRightVal = FVector::DotProduct(PlayerCamera->GetRightVector(), Velocity.GetSafeNormal());

		CameraLeanLerp = MoveRightVal * 3.8575f;
	}
	else
	{
		CameraLeanLerp = 0;
	}

	PivotPlayerBody->SetWorldRotation(FMath::Lerp(PivotPlayerBody->GetComponentRotation(),
                                                  PlayerCamera->GetComponentRotation(), DeltaSeconds * 10));

	FRotator PlayerCameraRotation = PlayerCamera->GetRelativeRotation();
	if (!FMath::IsNearlyEqual(PlayerCameraRotation.Roll, CameraLeanLerp))
	{
		PlayerCameraRotation.Roll = FMath::Lerp(PlayerCameraRotation.Roll, CameraLeanLerp, DeltaSeconds * 10);
		PlayerCamera->SetRelativeRotation(PlayerCameraRotation);
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value == 0.0f) return;
	const FRotator YawOnlyRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	AddMovementInput(FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X), Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value == 0.0f) return;
	RETURN_IF_NULLPTR(PlayerCamera);
	AddMovementInput(PlayerCamera->GetRightVector(), Value);
}


void APlayerCharacter::Turn(float Value)
{
	if (Value == 0.0f) return;
	AddControllerYawInput(Value);
}

void APlayerCharacter::LookUp(float Value)
{
	if (Value == 0.0f) return;
	AddControllerPitchInput(Value);
}

UAmmoComponent* APlayerCharacter::RetrieveAmmoComponent_Implementation()
{
	return AmmoComponent;
}

void APlayerCharacter::OnRep_CustomizationColors()
{
	CosmeticImplementation_Colors();
}

void APlayerCharacter::SetCosmetics_Request()
{
	UNetworkGameInstance* GameInstance = Cast<UNetworkGameInstance>(UGameplayStatics::GetGameInstance(this));
	SetCosmetics_Response(GameInstance->GetPlayerCustomizationSave()->Colors);
}

void APlayerCharacter::SetCosmetics_Response_Implementation(const FPlayerCustomizationColors Colors)
{

	CustomizationColors = Colors;
	OnRep_CustomizationColors();
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, CustomizationColors);

}
