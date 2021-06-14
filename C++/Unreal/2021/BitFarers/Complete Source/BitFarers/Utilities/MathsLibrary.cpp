
#include "MathsLibrary.h"

bool UMathsLibrary::VectorSmallerCheck( FVector V1, FVector V2 ) {
	if ( FloatSmallerCheck( V1.X, V2.X ) && FloatSmallerCheck( V1.Y, V2.Y ) && FloatSmallerCheck( V1.Z, V2.Z ) ) return true;
	return false;
}

bool UMathsLibrary::FloatSmallerCheck( float F1, float F2 ) {
	if ( F1 < F2 ) return true;
	return false;
}

float UMathsLibrary::SetScale( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale ) {

	return ( CurrentValue - OldMinScale ) * ( NewMaxScale - NewMinScale ) / ( OldMaxScale - OldMinScale ) + NewMinScale;
}

float UMathsLibrary::VectorToVectorPercentageCheck( FVector V1, FVector V2, FVector OriginScale ) {
	float X1 = V1.X;
	float Y1 = V1.Y;
	float Z1 = V1.Z;

	float X2 = V2.X;
	float Y2 = V2.Y;
	float Z2 = V2.Z;

	float XPercentage = SetScale( X1, OriginScale.X, X2, 0, 100 );
	float YPercentage = SetScale( Y1, OriginScale.Y, Y2, 0, 100 );
	float ZPercentage = SetScale( Z1, OriginScale.Z, Z2, 0, 100 );

	return ( XPercentage + YPercentage + ZPercentage ) / 3;
}

float UMathsLibrary::SetScalePure( float CurrentValue, float OldMinScale, float OldMaxScale, float NewMinScale, float NewMaxScale ) {

	return ( CurrentValue - OldMinScale ) * ( NewMaxScale - NewMinScale )  / ( OldMaxScale - OldMinScale )  + NewMinScale;
}
