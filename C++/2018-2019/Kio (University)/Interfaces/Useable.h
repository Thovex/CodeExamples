// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "Useable.generated.h"

/**
 *
 */

UINTERFACE( BlueprintType )
class KIO_API UUseable : public UInterface {
	GENERATED_BODY()

		UUseable( const FObjectInitializer & ObjectInitializer );
};
class KIO_API IUseable {
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Useable Interface" )
		void Use();

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Useable Interface" )
		void StopUsing();
};