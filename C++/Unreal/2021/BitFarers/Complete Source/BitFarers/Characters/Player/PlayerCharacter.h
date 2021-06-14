// Copyright Baer and Hoggo Games

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Character.h"
#include "SmoothSync.h"
#include "BitFarers/Utilities/Interfaces/AmmoInterface.h"
#include "BitFarers/Utilities/Interfaces/StatusInterface.h"
#include "BitFarers/Weapons/EElementalType.h"
#include "Customization/PlayerCustomizationSave.h"


#include "PlayerCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FSwapWeaponIndex, int32);

UCLASS()
class BITFARERS_API APlayerCharacter : public ACharacter, public IStatusInterface, public IAmmoInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Transform)
	USceneComponent* PlayerTransform;

	// -- Legs --

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Player|Legs")
	class USpringArmComponent* PlayerLegsSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Player|Legs")
	USceneComponent* PivotPlayerLegs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Player|Legs")
	TSubclassOf<UAnimInstance> PlayerLegsAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Player|Legs")
	USkeletalMeshComponent* PlayerLegsSK;

	// -- Body --

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Body")
	class USpringArmComponent* PlayerBodySpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Body")
	USceneComponent* PivotPlayerBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Body")
	UStaticMeshComponent* PlayerBodySM;

	// -- Arms --

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Body|Arms")
	TSubclassOf<UAnimInstance> PlayerSkillArmAnimInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Body|Arms")
	USkeletalMeshComponent* PlayerSkillArmSK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Body|Arms")
	TSubclassOf<UAnimInstance> PlayerWeaponArmAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Body|Arms")
	USkeletalMeshComponent* PlayerWeaponArmSK;

	// -- Camera --

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Camera")
	USpringArmComponent* Camera_SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	// -- First Person --

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Camera|FPS")
	USpringArmComponent* FPS_SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Camera|FPS")
	TSubclassOf<UAnimInstance> PlayerWeaponArmFPSAnimInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Camera|FPS")
	USkeletalMeshComponent* FPS_WeaponArmSK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Camera|FPS")
	TSubclassOf<UAnimInstance> PlayerSkillArmFPSAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Camera|FPS")
	USkeletalMeshComponent* FPS_SkillArmSK;

	// -- Movement System --
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Systems")
	class UCustomCharacterMovement* Movement;

	// -- Ammo System --
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Systems")
	class UAmmoComponent* AmmoComponent;

	// -- Weapon System --
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Systems")
	class UWeaponComponent* WeaponComponent;

	// -- Status System --
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Systems")
	class UStatusComponent* StatusComponent;

	// -- Audio System --
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Systems")
	class USoundCasterComponent* SoundCasterComponent;

	// -- Utility Systems --
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Utility")
	class ULerpComponent* Lerp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction|Utility")
	class USmoothSync* SmoothSync;
	
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BeginInitialize();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage_Implementation(EElementalType ElementalType, float Damage, const AActor* DamageInstigator,
		const UActorComponent* ImpactComponent, FVector ImpactPoint) override;
	virtual bool IsPlayer_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Getters|Views")
	FORCEINLINE USkeletalMeshComponent* GetFPS() const { return FPS_WeaponArmSK; }

	UFUNCTION(BlueprintCallable, Category = "Getters|Views")
    FORCEINLINE USkeletalMeshComponent* GetTPS() const { return PlayerWeaponArmSK; }

	UFUNCTION(BlueprintCallable, Reliable, Server)
	void TransferOwnership(AActor* Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_CustomizationColors)
	FPlayerCustomizationColors CustomizationColors;

	UFUNCTION()
	void OnRep_CustomizationColors();

	UFUNCTION(BlueprintCallable)
	void SetCosmetics_Request();

	UFUNCTION(Server, Reliable)
	void SetCosmetics_Response(const FPlayerCustomizationColors Colors);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CosmeticImplementation_Colors();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 PlayerIndex = -1;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual UAmmoComponent* RetrieveAmmoComponent_Implementation() override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	float CameraLeanLerp;
	
private:
	void OnFire_Start();
	void OnFire_End();
	
	void SetWeaponMode();
	
	void Reload();
	void SwapWeapon(int32 WeaponIndex);

	void MoveForward(float Value);
	void MoveRight(float Value);
	
	void Turn(float Value);
	void LookUp(float Value);

	bool bInitialized = false;

};
