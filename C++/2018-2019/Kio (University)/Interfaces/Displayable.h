// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "Displayable.generated.h"

/**
 *
 */

UINTERFACE( BlueprintType )
class KIO_API UDisplayable : public UInterface {
	GENERATED_BODY()

		UDisplayable( const FObjectInitializer & ObjectInitializer );
};
class KIO_API IDisplayable {
	GENERATED_BODY()

public:

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Displayable Interface" )
		void Display( bool DisplayValue );
};