#pragma once

#include "ButtonClickable.generated.h"

/**
*
*/

UINTERFACE(BlueprintType)
class BITFARERS_API	UButtonClickable : public UInterface {
	GENERATED_BODY()
	
	UButtonClickable();



};
class BITFARERS_API IButtonClickable {
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Button Interface" )
        void DoButtonClick(int32 PlayerId, int32 FunctionalIndex);
};