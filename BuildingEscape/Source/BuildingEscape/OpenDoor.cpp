// Amshu 2016

#include "BuildingEscape.h"
#include "OpenDoor.h"
#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();

	if (!PressurePlate) { UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *(Owner->GetName())); }
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the Trigger Volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OnOpen.Broadcast();
	}
	else { OnClose.Broadcast(); }
}


float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;
	
	if (!PressurePlate) { return TotalMass; }

	// Find all overlappin actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through them adding their masses
	for (const auto& Actor:OverlappingActors) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s placed on pressure plate"), *(Actor->GetName()));
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
 
