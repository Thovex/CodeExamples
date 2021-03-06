// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonExporter.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "Macros.h"
#include "PlatformFilemanager.h"
#include "Paths.h"

AJsonExporter::AJsonExporter() {

}

void AJsonExporter::ParseData( FWaveMatrix Wave, AModuleAssignee* ModuleAssignee ) {
	if ( !ModuleAssignee ) return;

	FWFCJsonStruct JsonStruct = FWFCJsonStruct();

	for3( Wave.SizeX, Wave.SizeY, Wave.SizeZ,
		  {
			  const FIntVector Coord = FIntVector( X, Y, Z );
			  FCoefficient Coefficient = Wave.GetDataAt( Coord );

			  FWFCJsonModuleCoordPattern NewCoordMap = FWFCJsonModuleCoordPattern();
			  NewCoordMap.Coord = Coord;
			  NewCoordMap.Value = Coefficient.LastAllowedPatternIndex();
			  NewCoordMap.Index = Wave.WaveCycle.FindRef( Coord );

			  if ( Wave.WaveValues.FindRef( Coord ).Empty ) {
				  NewCoordMap.ModuleData.Bit = Wave.WaveValues.FindRef( Coord ).Bit;
				  NewCoordMap.ModuleData.Empty = Wave.WaveValues.FindRef( Coord ).Empty;
				  NewCoordMap.ModuleData.ModuleID = "NULL";
				  NewCoordMap.ModuleData.RotationEuler = FIntVector::ZeroValue;
				  NewCoordMap.ModuleData.Scale = FIntVector::ZeroValue;
				  NewCoordMap.ModuleData.Symmetrical = false;
				  NewCoordMap.ModuleData.TSubClassOfAModule = "NULL";

			  } else {
				  NewCoordMap.ModuleData.Bit = Wave.WaveValues.FindRef( Coord ).Bit;
				  NewCoordMap.ModuleData.Empty = false;
				  NewCoordMap.ModuleData.ModuleID = Wave.WaveValues.FindRef( Coord ).ModuleID;
				  NewCoordMap.ModuleData.RotationEuler = Wave.WaveValues.FindRef( Coord ).RotationEuler;
				  NewCoordMap.ModuleData.Scale = Wave.WaveValues.FindRef( Coord ).Scale;
				  NewCoordMap.ModuleData.Symmetrical = Wave.WaveValues.FindRef( Coord ).Symmetrical;

				  auto ModuleSubclass = *ModuleAssignee->AssignedNames.FindKey( NewCoordMap.ModuleData.ModuleID );
				  NewCoordMap.ModuleData.TSubClassOfAModule = ModuleSubclass->GetFName().ToString();
			  }

			  JsonStruct.Data.Add( NewCoordMap );
		  }
	)

		FString JsonString;

		  FJsonObjectConverter::UStructToJsonObjectString( FWFCJsonStruct::StaticStruct(), &JsonStruct, JsonString, 0, 0 );

		  FString SaveDirectory = FPaths::ProjectDir() + FString( "WFC_Data/" );

		  FString FileName = FString( "WFC_" + FString::FromInt( Wave.SizeX ) + "x" + FString::FromInt( Wave.SizeY ) + "x" + FString::FromInt( Wave.SizeZ ) + "_" + FDateTime::Now().ToString() + ".json" );
		  FString TextToSave = JsonString;

		  bool AllowOverwriting = false;

		  IPlatformFile & PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		  if ( PlatformFile.CreateDirectoryTree( *SaveDirectory ) ) {
			  FString AbsoluteFilePath = SaveDirectory + "/" + FileName;

			  if ( AllowOverwriting || !PlatformFile.FileExists( *AbsoluteFilePath ) ) {
				  FFileHelper::SaveStringToFile( TextToSave, *AbsoluteFilePath );
			  }
		  }
}

FString AJsonExporter::ParseDataLocal( FWaveMatrix Wave, AModuleAssignee* ModuleAssignee ) {
	if ( !ModuleAssignee ) return "";

	FWFCJsonStruct JsonStruct = FWFCJsonStruct();

	for3( Wave.SizeX, Wave.SizeY, Wave.SizeZ,
		  {
			  const FIntVector Coord = FIntVector( X, Y, Z );
			  FCoefficient Coefficient = Wave.GetDataAt( Coord );

			  FWFCJsonModuleCoordPattern NewCoordMap = FWFCJsonModuleCoordPattern();
			  NewCoordMap.Coord = Coord;
			  NewCoordMap.Value = Coefficient.LastAllowedPatternIndex();
			  NewCoordMap.Index = Wave.WaveCycle.FindRef( Coord );

			  if ( Wave.WaveValues.FindRef( Coord ).Empty ) {
				  NewCoordMap.ModuleData.Bit = Wave.WaveValues.FindRef( Coord ).Bit;
				  NewCoordMap.ModuleData.Empty = Wave.WaveValues.FindRef( Coord ).Empty;
				  NewCoordMap.ModuleData.ModuleID = "NULL";
				  NewCoordMap.ModuleData.RotationEuler = FIntVector::ZeroValue;
				  NewCoordMap.ModuleData.Scale = FIntVector::ZeroValue;
				  NewCoordMap.ModuleData.Symmetrical = false;
				  NewCoordMap.ModuleData.TSubClassOfAModule = "NULL";

			  } else {
				  NewCoordMap.ModuleData.Bit = Wave.WaveValues.FindRef( Coord ).Bit;
				  NewCoordMap.ModuleData.Empty = false;
				  NewCoordMap.ModuleData.ModuleID = Wave.WaveValues.FindRef( Coord ).ModuleID;
				  NewCoordMap.ModuleData.RotationEuler = Wave.WaveValues.FindRef( Coord ).RotationEuler;
				  NewCoordMap.ModuleData.Scale = Wave.WaveValues.FindRef( Coord ).Scale;
				  NewCoordMap.ModuleData.Symmetrical = Wave.WaveValues.FindRef( Coord ).Symmetrical;

				  auto ModuleSubclass = *ModuleAssignee->AssignedNames.FindKey( NewCoordMap.ModuleData.ModuleID );
				  NewCoordMap.ModuleData.TSubClassOfAModule = ModuleSubclass->GetFName().ToString();
			  }

			  JsonStruct.Data.Add( NewCoordMap );
		  }
	)

		FString JsonString = "";
		  FJsonObjectConverter::UStructToJsonObjectString( FWFCJsonStruct::StaticStruct(), &JsonStruct, JsonString, 0, 0 );

		  return JsonString;
}

FWaveMatrix AJsonExporter::RetrieveDataFileName( FString FileName, bool IsPresetData, AModuleAssignee* ModuleAssignee ) {

	const FString LoadDirectory = FPaths::ProjectDir() + ( IsPresetData ? FString( "WFC_PresetInput/" ) : FString( "WFC_Data/" ) );
	FString FileData = "";

	FFileHelper::LoadFileToString( FileData, *FString( FPaths::ProjectDir() + ( IsPresetData ? FString( "WFC_PresetInput/" ) : FString( "WFC_Data/" ) ) + FileName ) );
	return JsonToWave( FileData, ModuleAssignee );
}

// Maybe this shouldn't be in the json exporter, but for now lets keep it
FWaveMatrix AJsonExporter::GetWaveSide( FWaveMatrix OriginalWave, EOrientations Side, bool KeepExisting ) {
	FWaveMatrix Wave = KeepExisting ? OriginalWave : FWaveMatrix();

	switch ( Side ) {
		case EOrientations::FORWARD:
		{
			for ( auto& Elem : OriginalWave.Array3D ) {
				if ( Elem.Key.X == OriginalWave.SizeX - 1 ) {
					TMap<int32, bool> AllowedPatterns;
					AllowedPatterns.Add( Elem.Value.LastAllowedPatternIndex(), true );

					FIntVector ElemKeyAdjusted = Elem.Key;
					ElemKeyAdjusted.X = 0;

					Wave.AddData( ElemKeyAdjusted, FCoefficient( AllowedPatterns ) );
				}
			}
		} break;
		case EOrientations::BACK:
		{
			for ( auto& Elem : OriginalWave.Array3D ) {
				if ( Elem.Key.X == 0 ) {
					TMap<int32, bool> AllowedPatterns;
					AllowedPatterns.Add( Elem.Value.LastAllowedPatternIndex(), true );

					FIntVector ElemKeyAdjusted = Elem.Key;
					ElemKeyAdjusted.X = OriginalWave.SizeX - 1;

					Wave.AddData( ElemKeyAdjusted, FCoefficient( AllowedPatterns ) );
				}
			}
		} break;
		case EOrientations::RIGHT:
		{
			for ( auto& Elem : OriginalWave.Array3D ) {
				if ( Elem.Key.Y == OriginalWave.SizeY - 1 ) {
					TMap<int32, bool> AllowedPatterns;
					AllowedPatterns.Add( Elem.Value.LastAllowedPatternIndex(), true );

					FIntVector ElemKeyAdjusted = Elem.Key;
					ElemKeyAdjusted.Y = 0;

					Wave.AddData( ElemKeyAdjusted, FCoefficient( AllowedPatterns ) );
				}
			}
		} break;
		case EOrientations::LEFT:
		{
			for ( auto& Elem : OriginalWave.Array3D ) {
				if ( Elem.Key.Y == 0 ) {
					TMap<int32, bool> AllowedPatterns;
					AllowedPatterns.Add( Elem.Value.LastAllowedPatternIndex(), true );

					FIntVector ElemKeyAdjusted = Elem.Key;
					ElemKeyAdjusted.Y = OriginalWave.SizeY - 1;

					Wave.AddData( ElemKeyAdjusted, FCoefficient( AllowedPatterns ) );
				}
			}
		} break;
		default: {} break;
	}

	Wave.SetSize( Wave.CalculateSize() );

	return Wave;
}

FWaveMatrix AJsonExporter::JsonToWave( FString JsonString, AModuleAssignee * ModuleAssignee ) {
	FWFCJsonStruct JsonStruct = FWFCJsonStruct();

	FJsonObjectConverter::JsonObjectStringToUStruct( JsonString, &JsonStruct, 0, 0 );

	FWaveMatrix Wave = FWaveMatrix();

	for ( auto& Elem : JsonStruct.Data ) {
		TMap<int32, bool> AllowedPatterns;
		AllowedPatterns.Add( Elem.Value, true );

		Wave.AddData( Elem.Coord, FCoefficient( AllowedPatterns ) );

		FModuleData NewData = FModuleData();

		NewData.ModuleID = Elem.ModuleData.ModuleID;
		NewData.RotationEuler = Elem.ModuleData.RotationEuler;
		NewData.Bit = Elem.ModuleData.Bit;
		NewData.Empty = Elem.ModuleData.Empty;
		NewData.Scale = Elem.ModuleData.Scale;
		NewData.Symmetrical = Elem.ModuleData.Symmetrical;

		if ( Elem.ModuleData.Empty ) {
			NewData.Module = nullptr;
		} else {
			NewData.Module = *ModuleAssignee->AssignedNames.FindKey( Elem.ModuleData.ModuleID );
		}
		Wave.WaveValues.Add( Elem.Coord, NewData );
		Wave.WaveCycle.Add( Elem.Coord, Elem.Index );
	}

	Wave.SetSize( Wave.CalculateSize() );

	return Wave;
}

FWaveMatrix AJsonExporter::RetrieveDataRandom( FIntVector Size, FString & OutFileName, AModuleAssignee * ModuleAssignee ) {
	const FString LoadDirectory = FPaths::ProjectDir() + FString( "WFC_Data/" );
	FString FileData = "";

	TArray<FString> FileNames;

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	PlatformFile.FindFiles( FileNames, *LoadDirectory, NULL );

	FString FileName = "";

	if ( Size.X > 0 || Size.Y > 0 || Size.Z > 0 ) {
		TArray<FString> FileNamesMatchingSize;

		for ( auto& LocalFileName : FileNames ) {
			if ( LocalFileName.Contains( FString::FromInt( Size.X ) + "x" + FString::FromInt( Size.Y ) + "X" + FString::FromInt( Size.Z ) ) ) {
				FileNamesMatchingSize.Add( LocalFileName );
			}
		}

		if ( FileNamesMatchingSize.Num() > 0 ) {
			FileName = FileNamesMatchingSize[FMath::RandRange( 0, FileNamesMatchingSize.Num() - 1 )];
		} else {
			UE_LOG( LogTemp, Error, TEXT( "No json found with size: %s" ), *Size.ToString() );
		}
	} else {
		FileName = FileNames[FMath::RandRange( 0, FileNames.Num() - 1 )];
	}

	OutFileName = FileName;
	FFileHelper::LoadFileToString( FileData, *FString( FileName ) );

	return JsonToWave( FileData, ModuleAssignee );
}

