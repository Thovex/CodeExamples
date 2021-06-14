
#include "AccessorLibrary.h"


ANetworkGameState* UAccessorLibrary::GetNetworkGameState(const UObject* WorldContextObject)
{
    return Cast<ANetworkGameState>(WorldContextObject->GetWorld()->GetGameState());
    
}
