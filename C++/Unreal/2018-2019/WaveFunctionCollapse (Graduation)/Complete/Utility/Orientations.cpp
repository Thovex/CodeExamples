// Fill out your copyright notice in the Description page of Project Settings.


#include "Orientations.h"

const TMap<EOrientations, FIntVector> UOrientations::OrientationEulers = TMap<EOrientations, FIntVector> {
		{
			EOrientations::FORWARD, FIntVector( 0, 0, 0 )
		},
		{
			EOrientations::BACK, FIntVector( 0, 0, 180 )
		},
		{
			EOrientations::RIGHT, FIntVector( 0, 0, 90 )
		},
		{
			EOrientations::LEFT,  FIntVector( 0, 0, -90 )
		},
		{
			EOrientations::UP, FIntVector( 0, -90, 0 )
		},
		{
			EOrientations::DOWN, FIntVector( 0, 90, 0 )
		}
};

const TMap<FName, EOrientations> UOrientations::OrientationByFName = TMap<FName, EOrientations> {
		{
			FName( TEXT( "F" ) ), EOrientations::FORWARD
		},
		{
			FName( TEXT( "B" ) ), EOrientations::BACK
		},
		{
			FName( TEXT( "R" ) ), EOrientations::RIGHT
		},
		{
			FName( TEXT( "L" ) ),  EOrientations::LEFT
		},
		{
			FName( TEXT( "U" ) ), EOrientations::UP
		},
		{
			FName( TEXT( "D" ) ), EOrientations::DOWN
		}
};

#define ForwardUnitVector FIntVector(1, 0, 0)
#define BackUnitVector ForwardUnitVector * - 1

#define RightUnitVector FIntVector(0, 1, 0)
#define LeftUnitVector RightUnitVector * - 1

#define UpUnitVector FIntVector(0, 0, 1)
#define DownUnitVector UpUnitVector * - 1

const TMap<EOrientations, FIntVector> UOrientations::OrientationUnitVectors = TMap<EOrientations, FIntVector> {
		{
			EOrientations::FORWARD, ForwardUnitVector
		},
		{
			EOrientations::BACK, BackUnitVector
		},
		{
			EOrientations::RIGHT, RightUnitVector
		},
		{
			EOrientations::LEFT, LeftUnitVector
		},
		{
			EOrientations::UP, UpUnitVector
		},
		{
			EOrientations::DOWN, DownUnitVector
		},
		{
			EOrientations::RIGHT_UP, RightUnitVector + UpUnitVector
		},
		{
			EOrientations::RIGHT_DOWN,  RightUnitVector + DownUnitVector
		},
		{
			EOrientations::LEFT_UP,  LeftUnitVector + UpUnitVector
		},
		{
			EOrientations::LEFT_DOWN,  LeftUnitVector + DownUnitVector
		},
		{
			EOrientations::FORWARD_RIGHT, ForwardUnitVector + RightUnitVector
		},
		{
			EOrientations::FORWARD_LEFT, ForwardUnitVector + LeftUnitVector
		},
		{
			EOrientations::FORWARD_UP, ForwardUnitVector + UpUnitVector
		},
		{
			EOrientations::BACK_DOWN, BackUnitVector + DownUnitVector
		},
		{
			EOrientations::FORWARD_DOWN, ForwardUnitVector + DownUnitVector
		},
		{
			EOrientations::BACK_UP, BackUnitVector + UpUnitVector
		},
		{
			EOrientations::FORWARD_LEFT_UP, ForwardUnitVector + LeftUnitVector + UpUnitVector
		},
		{
			EOrientations::FORWARD_LEFT_DOWN, ForwardUnitVector + LeftUnitVector + DownUnitVector
		},
		{
			EOrientations::FORWARD_RIGHT_UP, ForwardUnitVector + RightUnitVector + UpUnitVector
		},
		{
			EOrientations::FORWARD_RIGHT_DOWN, ForwardUnitVector + RightUnitVector + DownUnitVector
		},
		{
			EOrientations::BACK_RIGHT, BackUnitVector + RightUnitVector
		},
		{
			EOrientations::BACK_LEFT, BackUnitVector + LeftUnitVector
		},
		{
			EOrientations::BACK_LEFT_UP, BackUnitVector + LeftUnitVector + UpUnitVector
		},
		{
			EOrientations::BACK_LEFT_DOWN, BackUnitVector + LeftUnitVector + DownUnitVector
		},
		{
			EOrientations::BACK_RIGHT_UP, BackUnitVector + RightUnitVector + UpUnitVector
		},
		{
			EOrientations::BACK_RIGHT_DOWN, BackUnitVector + RightUnitVector + DownUnitVector
		},
		{
			EOrientations::NONE, FIntVector( 0, 0, 0 )
		}
};

TMap<EOrientations, FIntVector> UOrientations::GetOrientationEulers() {
	return OrientationEulers;
}

TMap<FName, EOrientations> UOrientations::GetOrientationByFName() {
	return OrientationByFName;
}

TMap<EOrientations, FIntVector> UOrientations::GetOrientationUnitVectors() {
	return OrientationUnitVectors;
}

EOrientations UOrientations::EulerToOrientation( FIntVector RotationVector ) {
	int32 RotationValue = RotationVector.Z;

	RotationValue %= 360;

	if ( RotationValue % 360 == 0 ) {
		return EOrientations::FORWARD;
	}

	if ( RotationValue % 180 == 0 ) {
		return EOrientations::BACK;
	}

	if ( RotationValue % 90 == 0 ) {
		if ( RotationValue == -90 || RotationValue == 270 ) {
			return EOrientations::LEFT;
		}

		if ( RotationValue == 90 || RotationValue == -270 ) {
			return EOrientations::RIGHT;
		}
	}

	return EOrientations::NONE;
}

