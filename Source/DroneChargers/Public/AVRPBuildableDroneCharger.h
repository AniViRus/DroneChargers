#pragma once

#include "CoreMinimal.h"
#include "Buildables/FGBuildableFactory.h"
#include "FGBuildableDroneStation.h"
#include "FGFactoryConnectionComponent.h"
#include "AVRPBuildableDroneCharger.generated.h"

/**
 * Drone Charger which gets snapped to Drone Stations with fuel input connections and responsible for recharging batteries consumed by drones.
 */
UCLASS()
class DRONECHARGERS_API AAVRPBuildableDroneCharger : public AFGBuildableFactory
{
	GENERATED_BODY()
public:
	AAVRPBuildableDroneCharger();

	// Begin AActor interface
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End AActor interface

	// Begin FGBuildableFactory Interface
	virtual bool CanProduce_Implementation() const override;
	virtual void Factory_TickProducing(float dt) override;
	// End FGBuildableFactory Interface

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AFGBuildableDroneStation* GetDroneStation() const { return mDroneStation; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UFGInventoryComponent* GetFuelInventory() const { return GetDroneStation()->GetFuelInventory(); }
	UFUNCTION()
	FORCEINLINE void SetDroneStation(AFGBuildableDroneStation* newDroneStation) { mDroneStation = newDroneStation; }
	UFUNCTION()
	FORCEINLINE void ConnectWithStation(UFGFactoryConnectionComponent* stationConnection) { fuelOutputConnection->SetConnection(stationConnection); }
	UFUNCTION()
	void OnFuelRemoved(TSubclassOf< UFGItemDescriptor > itemClass, int32 numRemoved);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	UFGFactoryConnectionComponent* fuelOutputConnection;
	UPROPERTY(SaveGame, Replicated, BlueprintReadOnly)
	TSubclassOf<class UFGItemDescriptor> mCurrentBatteryClass;
	UPROPERTY(SaveGame, Replicated, BlueprintReadOnly)
	int32 mSavedBatteryAmount;
	UPROPERTY(SaveGame, Replicated, BlueprintReadOnly)
	float mCurrentRechargeProgress;
private:
	UPROPERTY(SaveGame)
	AFGBuildableDroneStation* mDroneStation;
};
