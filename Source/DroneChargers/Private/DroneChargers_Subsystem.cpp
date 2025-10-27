#include "DroneChargers_Subsystem.h"
#include "FGInventoryComponent.h"
#include "Patching/NativeHookManager.h"
#include "ContentTagRegistry.h"

void ADroneChargers_Subsystem::BeginPlay()
{
	Super::BeginPlay();
	if (hooksInitialized) return;
	hookHandler_before = SUBSCRIBE_METHOD(AFGDroneVehicle::RefuelFromDockedStation, [this](auto& scope, AFGDroneVehicle* self, float amount) {
		if (mPortChargers.Contains(self->GetDockedStation())) {
			FInventoryStack stack;
			if (self->GetDockedStation()->GetFuelInventory()->GetStackFromIndex(0, stack)) {
				mQueuedRefuels.Add(self, FStationFuelAmount(self->GetDockedStation(), FItemAmount(stack.Item.GetItemClass(), stack.NumItems)));
			}
		}
	});
	hookHandler_after = SUBSCRIBE_METHOD_AFTER(AFGDroneVehicle::RefuelFromDockedStation, [this](AFGDroneVehicle* self, float amount) {
		if (mQueuedRefuels.Contains(self)) {
			if (mPortChargers.Contains(mQueuedRefuels[self].Station)) {
				int32 itemsLeft = 0;
				FInventoryStack stack;
				if (mQueuedRefuels[self].Station->GetFuelInventory()->GetStackFromIndex(0, stack)) {
					itemsLeft = stack.NumItems;
				}
				mPortChargers[mQueuedRefuels[self].Station]->OnFuelRemoved(mQueuedRefuels[self].RefuelAmount.ItemClass, mQueuedRefuels[self].RefuelAmount.Amount - itemsLeft);
			}
			mQueuedRefuels.Remove(self);
		}
	});
	hooksInitialized = true;
}

void ADroneChargers_Subsystem::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	if (hookHandler_before.IsValid()) UNSUBSCRIBE_METHOD(AFGDroneVehicle::RefuelFromDockedStation, hookHandler_before);
	if (hookHandler_after.IsValid()) UNSUBSCRIBE_METHOD(AFGDroneVehicle::RefuelFromDockedStation, hookHandler_after);
}

void ADroneChargers_Subsystem::RegisterPortCharger(AFGBuildableDroneStation* port, AAVRPBuildableDroneCharger* charger)
{
	if (!mPortChargers.Contains(port)) mPortChargers.Add(port, charger);
}

void ADroneChargers_Subsystem::UnregisterPortCharger(AFGBuildableDroneStation* port)
{
	if (mPortChargers.Contains(port)) mPortChargers.Remove(port);
}

FStationFuelAmount::FStationFuelAmount()
{
	Station = nullptr;
	RefuelAmount = FItemAmount();
}

FStationFuelAmount::FStationFuelAmount(AFGBuildableDroneStation* station, FItemAmount refuelAmount)
{
	Station = station;
	RefuelAmount = refuelAmount;
}
