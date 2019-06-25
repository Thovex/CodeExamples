// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "Components/ActorComponent.h"
#include "DebugEditorTickingComponent.generated.h"


UCLASS( ClassGroup = ( Debug ), meta = ( BlueprintSpawnableComponent ), BlueprintType, Blueprintable )
class KIO_API UDebugEditorTickingComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UDebugEditorTickingComponent();


protected:
	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

};
