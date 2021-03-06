// Copyright 2017 - Jesse van Vliet & Kristo Hõrrak

#include "BuildingBase.h"
#include "Private/KismetTraceUtils.h"
#include "UnrealNetwork.h"
#include "Shaders/FacetsApparition.h"
#include "UnrealString.h"


// Sets default values
ABuildingBase::ABuildingBase(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {
	static ConstructorHelpers::FObjectFinder<UMaterial> BuildingMaterialConstructor(TEXT("Material'/Game/Materials/Buildings/Shaders/M_FacetsApparition.M_FacetsApparition'"));

	if(BuildingMaterialConstructor.Object != NULL) {
		BuildingMaterial = (UMaterial*)BuildingMaterialConstructor.Object;
	}

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("Class'/Game/Blueprints/Shaders/BP_Shader_FacetsApparition.BP_Shader_FacetsApparition_C'"));

	if(bpClassFinder.Object != NULL) {
		BuildingClass = bpClassFinder.Object;
	}

	BuildingMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Building Mesh");
	bNetLoadOnClient = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	BuildingMeshComponent->SetBoundsScale(5.0F);
}


void ABuildingBase::ServerTick(float DeltaTime) {
	Super::ServerTick(DeltaTime);


	if(AllowedTypes.Contains(BuildingType)) {
		ServerCreateSpheres();
		CheckForSelection(DeltaTime);
		ActivateSelection();
	}
}

void ABuildingBase::ClientTick(float DeltaTime) {
	Super::ClientTick(DeltaTime);

	if(AllowedTypes.Contains(BuildingType)) {
		CheckForSelection(DeltaTime);
		ActivateSelection();
	}

}


void ABuildingBase::BeginPlay() {
	Super::BeginPlay();
	if(HasAuthority()) { 
	}

}

bool ABuildingBase::ServerCreateSpheres_Validate() {
	return true;
}

void ABuildingBase::ServerCreateSpheres_Implementation() {
	if(!bMadeSpheres) {
		TArray<FName> SocketNames = BuildingMeshComponent->GetAllSocketNames();
		TArray<TEnumAsByte<EObjectTypeQuery>> TypesToHit = { UEngineTypes::ConvertToObjectType(ECC_WorldDynamic), UEngineTypes::ConvertToObjectType(ECC_WorldStatic) };
		TArray<AActor*> ActorsToIgnore;

		for(FName SocketName : SocketNames) {
			FTransform SocketTransform = BuildingMeshComponent->GetSocketTransform(SocketName);
			FHitResult SphereHit;

			UKismetSystemLibrary::SphereTraceSingleForObjects(
				GetWorld(),
				SocketTransform.GetLocation(),
				SocketTransform.GetLocation(),
				50.0f,
				TypesToHit,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				SphereHit,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.0F);

			if(SphereHit.GetComponent()) {
				if(!SphereHit.GetComponent()->IsA(UBuildingBaseSphereComponent::StaticClass())) {
					ServerAddSphere(SocketName, SocketTransform);
				}
			} else {
				ServerAddSphere(SocketName, SocketTransform);
			}
		}

		bMadeSpheres = true;
	}
}

bool ABuildingBase::ServerAddSphere_Validate(FName SocketName, FTransform SocketTransform) {
	return true;
}

void ABuildingBase::ServerAddSphere_Implementation(FName SocketName, FTransform SocketTransform) {
	UBuildingBaseSphereComponent* Component = NewObject<UBuildingBaseSphereComponent>(this, UBuildingBaseSphereComponent::StaticClass());
	Component->SetDetails(SocketName, SocketTransform, BuildingMeshComponent);
	SphereCollisions.Add(Component);
}

void ABuildingBase::CheckForSelection(float DeltaTime) {
	if(SelectedTimer >= 0) {
		SelectedTimer -= DeltaTime;
		bIsSelected = true;
	} else {
		bIsSelected = false;
		if(TypesLookingAt.Num() > 0) {
			TypesLookingAt.Empty();
		}
	}
}

void ABuildingBase::ActivateSelection() {
	if(bIsSelected) {
		for(EBuildingBookType TypeLookingAt : TypesLookingAt) {
			for(UBuildingBaseSphereComponent* Sphere : SphereCollisions) {
				FString GetType = GetEnumValueToString<EBuildingBookType>("EBuildingBookType", TypeLookingAt);
				TArray<FString> StringArray;
				GetType.ParseIntoArray(StringArray, TEXT("EBBT_"), true);

				if(Sphere->BuildingSphereTags.Contains(FName(*StringArray[1]))) {
					DrawDebugSphere(GetWorld(), Sphere->GetComponentLocation(), 32.0F, 8, FColor::Red, false, 1.0F, 1, 1.0F);
				}
			}
		}
	}
}

void ABuildingBase::AddType(EBuildingBookType TypeToAdd) {
	if(!TypesLookingAt.Contains(TypeToAdd)) {
		TypesLookingAt.Add(TypeToAdd);
	}

	SelectedTimer = 5.0F;
}

void ABuildingBase::RemoveSphere(UBuildingBaseSphereComponent* SphereToRemove) {
	if(SphereCollisions.Contains(SphereToRemove)) {
		ServerRemoveSphere(SphereToRemove);
	}
}

bool ABuildingBase::ServerRemoveSphere_Validate(UBuildingBaseSphereComponent* SphereToRemove) {
	return true;
}

void ABuildingBase::ServerRemoveSphere_Implementation(UBuildingBaseSphereComponent* SphereToRemove) {
	SphereCollisions.Remove(SphereToRemove);
	SphereToRemove->DestroyComponent();
	OnRep_SetCollisionArray();
}

void ABuildingBase::OnRep_SetBuildingMesh() {
	BuildingMeshComponent->SetStaticMesh(BuildingMesh);

	int32 TotalMaterialIndex = BuildingMeshComponent->GetNumMaterials();

	for(int i = 0; i < TotalMaterialIndex; i++) {
		OriginalMaterials.Add(BuildingMeshComponent->GetMaterial(i));
		BuildingMeshComponent->SetMaterial(i, UMaterialInstanceDynamic::Create(BuildingMaterial, BuildingMeshComponent));
	}

	FActorSpawnParameters SpawnParameters;

	AActor* TheActor = GetWorld()->SpawnActor<AActor>(BuildingClass, GetActorLocation() + FVector(0, 0, 2000), GetActorRotation(), SpawnParameters);
	AFacetsApparition* TheEffect = Cast<AFacetsApparition>(TheActor);

	if(TheEffect) {
		TheEffect->SelectedComponent = BuildingMeshComponent;
	}

	FTimerHandle DestroySpawner;
	GetWorldTimerManager().ClearTimer(DestroySpawner);
	GetWorldTimerManager().SetTimer(DestroySpawner, this, &ABuildingBase::ReturnMaterials, 3.0F, false, -1.0f);
}

void ABuildingBase::ReturnMaterials() {
	int32 TotalMaterialIndex = BuildingMeshComponent->GetNumMaterials();

	for(int i = 0; i < TotalMaterialIndex; i++) {
		BuildingMeshComponent->SetMaterial(i, OriginalMaterials[i]);
	}
}

void ABuildingBase::OnRep_SetCollisionArray() {
}

void ABuildingBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const {
	DOREPLIFETIME(ABuildingBase, BuildingMesh);
	DOREPLIFETIME(ABuildingBase, BuildingType);
	DOREPLIFETIME(ABuildingBase, SphereCollisions);
	DOREPLIFETIME(ABuildingBase, bIsSelected);
	DOREPLIFETIME(ABuildingBase, TypesLookingAt);
	DOREPLIFETIME(ABuildingBase, SelectedTimer);
}