// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomCharacterMovement.h"

#include "GameFramework/Character.h"

UCustomCharacterMovement::UCustomCharacterMovement(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DodgeStrength = 2000.f;
}

void UCustomCharacterMovement::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaTime, OldLocation, OldVelocity);

	if (!CharacterOwner)
	{
		return;
	}

	//Set Max Walk Speed
	if (bRequestMaxWalkSpeedChange)
	{
		bRequestMaxWalkSpeedChange = false;
		MaxWalkSpeed = MyNewMaxWalkSpeed;
	}

	//Dodge
	if (bWantsToDodge)
	{
		bWantsToDodge = false;
		MoveDirection.Normalize();
		const float Strength = this->Velocity.Z != 0 ? 1000 : DodgeStrength;
		FVector ActorForwardVector = this->GetOwner()->GetActorForwardVector();
		const FVector Direction = (MoveDirection.X + MoveDirection.Y )== 0 ? ActorForwardVector : MoveDirection;
		FVector DodgeVelocity = Direction * DodgeStrength;
		DodgeVelocity.Z = 0.0f;
		Launch(DodgeVelocity);		
	}
}

bool UCustomCharacterMovement::HandlePendingLaunch()
{
	if (!PendingLaunchVelocity.IsZero() && HasValidData())
	{
		Velocity = PendingLaunchVelocity;
		//Remmed out so our dodge move won't play the falling animation.
		//SetMovementMode(MOVE_Falling);
		PendingLaunchVelocity = FVector::ZeroVector;
		bForceNextFloorCheck = true;
		return true;
	}

	return false;
}


void UCustomCharacterMovement::UpdateFromCompressedFlags(uint8 Flags)//Client only
{
	Super::UpdateFromCompressedFlags(Flags);

	bRequestMaxWalkSpeedChange = (Flags&FSavedMove_Character::FLAG_Custom_0) != 0;
	bWantsToDodge = (Flags&FSavedMove_Character::FLAG_Custom_1) != 0;
}

class FNetworkPredictionData_Client* UCustomCharacterMovement::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);
	//check(PawnOwner->Role < ROLE_Authority);

	if (!ClientPredictionData)
	{
		UCustomCharacterMovement* MutableThis = const_cast<UCustomCharacterMovement*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_My(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UCustomCharacterMovement::FSavedMove_My::Clear()
{
	Super::Clear();

	bSavedRequestMaxWalkSpeedChange = false;
	bSavedWantsToDodge = false;
	SavedMoveDirection = FVector::ZeroVector;
}

uint8 UCustomCharacterMovement::FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedRequestMaxWalkSpeedChange)
	{
		Result |= FLAG_Custom_0;
	}

	if (bSavedWantsToDodge)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UCustomCharacterMovement::FSavedMove_My::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.	
	if (bSavedRequestMaxWalkSpeedChange != ((FSavedMove_My*)&NewMove)->bSavedRequestMaxWalkSpeedChange)
	{
		return false;
	}
	if (bSavedWantsToDodge != ((FSavedMove_My*)&NewMove)->bSavedWantsToDodge)
	{
		return false;
	}
	if (SavedMoveDirection != ((FSavedMove_My*)&NewMove)->SavedMoveDirection)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UCustomCharacterMovement::FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UCustomCharacterMovement* CharacterMovement = Cast<UCustomCharacterMovement>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		bSavedRequestMaxWalkSpeedChange = CharacterMovement->bRequestMaxWalkSpeedChange;
		bSavedWantsToDodge = CharacterMovement->bWantsToDodge;
		SavedMoveDirection = CharacterMovement->MoveDirection;
	}
}

void UCustomCharacterMovement::FSavedMove_My::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UCustomCharacterMovement* CharacterMovement = Cast<UCustomCharacterMovement>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		CharacterMovement->MoveDirection = SavedMoveDirection;
	}
}

UCustomCharacterMovement::FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UCustomCharacterMovement::FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_My());
}


void UCustomCharacterMovement::Server_SetMaxWalkSpeed_Implementation(const float NewMaxWalkSpeed)
{
	MyNewMaxWalkSpeed = NewMaxWalkSpeed;
}

void UCustomCharacterMovement::SetMaxWalkSpeed(float NewMaxWalkSpeed)
{
	if (PawnOwner->IsLocallyControlled())
	{
		MyNewMaxWalkSpeed = NewMaxWalkSpeed;
		Server_SetMaxWalkSpeed(NewMaxWalkSpeed);
	}

	bRequestMaxWalkSpeedChange = true;
}


//Dodge RPC to transfer the current Move Direction from the Owning Client to the Server
bool UCustomCharacterMovement::Server_MoveDirection_Validate(const FVector& MoveDir)
{
	return true;
}

void UCustomCharacterMovement::Server_MoveDirection_Implementation(const FVector& MoveDir)
{
	MoveDirection = MoveDir;
}

//Trigger the Dodge ability on the Owning Client
void UCustomCharacterMovement::Dodge()
{
	if (PawnOwner->IsLocallyControlled())
	{
		MoveDirection = PawnOwner->GetLastMovementInputVector();
		Server_MoveDirection(MoveDirection);
	}

	bWantsToDodge = true;
}

