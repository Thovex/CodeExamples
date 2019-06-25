// Copyright 2018-2019. Seals of Approval by Jesse van Vliet, Rik Geersing, Maria van Veelen, Jesse Heida and Marjolein Joosse. All Rights Reserved.

#pragma once

#include "Kio.h"
#include "Outlineable.generated.h"

/**
 *
 */

UINTERFACE( BlueprintType )
class KIO_API UOutlineable : public UInterface {
	GENERATED_BODY()

		UOutlineable( const FObjectInitializer & ObjectInitializer );
};
class KIO_API IOutlineable {
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Outlineable Interface" )
		void Outline( bool DisplayValue );
};