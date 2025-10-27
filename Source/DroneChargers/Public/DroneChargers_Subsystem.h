#pragma once

#include "CoreMinimal.h"
#include "FGDroneVehicle.h"
#include "Subsystem/ModSubsystem.h"
#include "AVRPBuildableDroneCharger.h"
#include "FGBuildableDroneStation.h"
#include "DroneChargers_Subsystem.generated.h"

USTRUCT()
struct FStationFuelAmount {
	GENERATED_BODY()
public:
	FStationFuelAmount();
	FStationFuelAmount(AFGBuildableDroneStation* station, FItemAmount refuelAmount);
	UPROPERTY()
	AFGBuildableDroneStation* Station;
	UPROPERTY()
	FItemAmount RefuelAmount;
};
/**
 * Drone chargers subsystem - stores attempts of drones to refuel and resolves them
 */
UCLASS()
class DRONECHARGERS_API ADroneChargers_Subsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	UFUNCTION(BlueprintCallable)
	void RegisterPortCharger(AFGBuildableDroneStation* port, AAVRPBuildableDroneCharger* charger);
	UFUNCTION(BlueprintCallable)
	void UnregisterPortCharger(AFGBuildableDroneStation* port);
protected:
	bool hooksInitialized = false;
	FDelegateHandle hookHandler_before;
	FDelegateHandle hookHandler_after;
private:
	UPROPERTY()
	TMap<AFGBuildableDroneStation*, AAVRPBuildableDroneCharger*> mPortChargers;
	UPROPERTY()
	TMap<AFGDroneVehicle*, FStationFuelAmount> mQueuedRefuels;
};
