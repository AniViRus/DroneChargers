#include "DroneChargers_Subsystem.h"
#include "FGInventoryComponent.h"
#include "Patching/NativeHookManager.h"
#include "ContentTagRegistry.h"

void ADroneChargers_Subsystem::BeginPlay()
{
	Super::BeginPlay();
	SUBSCRIBE_METHOD(AFGDroneVehicle::RefuelFromDockedStation, [this](auto& scope, AFGDroneVehicle* self, float amount) {
		if (mPortChargers.Contains(self->GetDockedStation())) {
			FInventoryStack stack;
			self->GetDockedStation()->GetFuelInventory()->GetStackFromIndex(0, stack);
			mQueuedRefuels.Add(self->GetDockedStation(), FItemAmount(stack.Item.GetItemClass(), stack.NumItems));
		}
	});
	SUBSCRIBE_METHOD_AFTER(AFGDroneVehicle::RefuelFromDockedStation, [this](AFGDroneVehicle* self, float amount) {
		if (mPortChargers.Contains(self->GetDockedStation())) {
			int32 itemsLeft = 0;
			FInventoryStack stack;
			if (self->GetDockedStation()->GetFuelInventory()->GetStackFromIndex(0, stack)){
				itemsLeft = stack.NumItems;
			}
			mPortChargers[self->GetDockedStation()]->OnFuelRemoved(mQueuedRefuels[self->GetDockedStation()].ItemClass, mQueuedRefuels[self->GetDockedStation()].Amount - itemsLeft);
			mQueuedRefuels.Remove(self->GetDockedStation());
		}
	});
}

void ADroneChargers_Subsystem::RegisterPortCharger(AFGBuildableDroneStation* port, AAVRPBuildableDroneCharger* charger)
{
	mPortChargers.Add(port, charger);
}

void ADroneChargers_Subsystem::UnregisterPortCharger(AFGBuildableDroneStation* port)
{
	if (mPortChargers.Contains(port)) mPortChargers.Remove(port);
}