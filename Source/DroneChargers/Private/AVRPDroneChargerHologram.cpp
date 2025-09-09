#include "AVRPDroneChargerHologram.h"
#include "AVRPBuildableDroneCharger.h"
#include "FGFactoryConnectionComponent.h"

AAVRPDroneChargerHologram::AAVRPDroneChargerHologram() : Super() {}

bool AAVRPDroneChargerHologram::IsValidHitResult(const FHitResult& hitResult) const
{
	return true;
}

bool AAVRPDroneChargerHologram::TrySnapToActor(const FHitResult& hitResult)
{
	const auto Actor = hitResult.GetActor();
	if (!IsValid(Actor) || !Actor->IsA<AFGBuildableDroneStation>())
	{
		Snapped = nullptr;
		SnappedFactoryConnection = nullptr;
		AddConstructDisqualifier(UDroneChargerCD_MustSnapToDroneStation::StaticClass());
		return false;
	}
	Snapped = Cast<AFGBuildableDroneStation>(Actor);
	Snapped->ForEachComponent<UFGFactoryConnectionComponent>(true, [&](UFGFactoryConnectionComponent* FactoryConn) {
		if (FactoryConn->ComponentHasTag("Fuel") && FactoryConn->GetDirection() == EFactoryConnectionDirection::FCD_INPUT) {
			SnappedFactoryConnection = FactoryConn;
		}
		});
	if (!IsValid(SnappedFactoryConnection)) {
		Snapped = nullptr;
		SnappedFactoryConnection = nullptr;
		AddConstructDisqualifier(UDroneChargerCD_NoFuelInput::StaticClass());
		return false;
	}
	if (!SnappedFactoryConnection->IsConnected())
	{
		SetActorLocationAndRotation(SnappedFactoryConnection->GetComponentLocation(), SnappedFactoryConnection->GetComponentRotation());
		return true;
	}
	Snapped = nullptr;
	SnappedFactoryConnection = nullptr;
	AddConstructDisqualifier(UDroneChargerCD_FuelInputOccupied::StaticClass());
	return false;
}

void AAVRPDroneChargerHologram::ConfigureActor(AFGBuildable* inBuildable) const
{
	Super::ConfigureActor(inBuildable);
	Cast<AAVRPBuildableDroneCharger>(inBuildable)->SetDroneStation(Snapped);
}

void AAVRPDroneChargerHologram::ConfigureComponents(AFGBuildable* inBuildable) const
{
	Super::ConfigureComponents(inBuildable);
	Cast<AAVRPBuildableDroneCharger>(inBuildable)->ConnectWithStation(SnappedFactoryConnection);
}

void AAVRPDroneChargerHologram::CheckValidPlacement(){}
