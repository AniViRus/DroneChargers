#pragma once

#include "CoreMinimal.h"
#include "FGDroneVehicle.h"
#include "Subsystem/ModSubsystem.h"
#include "AVRPBuildableDroneCharger.h"
#include "FGBuildableDroneStation.h"
#include "DroneChargers_Subsystem.generated.h"

/**
 * Drone chargers subsystem - stores 
 */
UCLASS()
class DRONECHARGERS_API ADroneChargers_Subsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void RegisterPortCharger(AFGBuildableDroneStation* port, AAVRPBuildableDroneCharger* charger);
	UFUNCTION(BlueprintCallable)
	void UnregisterPortCharger(AFGBuildableDroneStation* port);
private:
	UPROPERTY(SaveGame)
	TMap<AFGBuildableDroneStation*, AAVRPBuildableDroneCharger*> mPortChargers;
	UPROPERTY(SaveGame)
	TMap<AFGBuildableDroneStation*, FItemAmount> mQueuedRefuels;
};
