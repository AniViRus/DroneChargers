#pragma once

#include "CoreMinimal.h"
#include "FGConstructDisqualifier.h"
#include "Hologram/FGBuildableHologram.h"
#include "FGBuildableDroneStation.h"
#include "FGFactoryConnectionComponent.h"
#include "AVRPDroneChargerHologram.generated.h"

#define LOCTEXT_NAMESPACE "DroneChargersLocnamespace"
UCLASS()
class DRONECHARGERS_API UDroneChargerCD_MustSnapToDroneStation : public UFGConstructDisqualifier
{
	GENERATED_BODY()
public:
	UDroneChargerCD_MustSnapToDroneStation()
	{
		mDisqfualifyingText = LOCTEXT("DroneCharger_MustSnapToDroneStation", "Must snap to a Drone Station!");
	}
};

UCLASS()
class DRONECHARGERS_API UDroneChargerCD_NoFuelInput : public UFGConstructDisqualifier
{
	GENERATED_BODY()
public:
	UDroneChargerCD_NoFuelInput()
	{
		mDisqfualifyingText = LOCTEXT("DroneCharger_NoFuelInput", "Drone Station has no fuel input connection!");
	}
};

UCLASS()
class DRONECHARGERS_API UDroneChargerCD_FuelInputOccupied : public UFGConstructDisqualifier
{
	GENERATED_BODY()
public:
	UDroneChargerCD_FuelInputOccupied()
	{
		mDisqfualifyingText = LOCTEXT("DroneCharger_FuelInputOccupied", "Fuel input connection is occupied!");
	}
};

#undef LOCTEXT_NAMESPACE

/**
 * Hologram for Drone Charger
 */
UCLASS()
class DRONECHARGERS_API AAVRPDroneChargerHologram : public AFGBuildableHologram
{
	GENERATED_BODY()
public:
	AAVRPDroneChargerHologram();
protected:
	virtual bool IsValidHitResult(const FHitResult& hitResult) const override;
	virtual bool TrySnapToActor(const FHitResult& hitResult) override;
	virtual void ConfigureActor(class AFGBuildable* inBuildable) const override;
	virtual void ConfigureComponents(class AFGBuildable* inBuildable) const override;
	virtual void CheckValidPlacement() override;
private:
	UPROPERTY(Transient)
	AFGBuildableDroneStation* Snapped = nullptr;
	UPROPERTY(Transient)
	UFGFactoryConnectionComponent* SnappedFactoryConnection = nullptr;
};