#include "AVRPBuildableDroneCharger.h"
#include "FGCharacterPlayer.h"
#include "ContentTagRegistry.h"
#include "UnrealNetwork.h"

AAVRPBuildableDroneCharger::AAVRPBuildableDroneCharger() : Super()
{
	fuelOutputConnection = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("OutputFuelConnection"));
}

void AAVRPBuildableDroneCharger::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAVRPBuildableDroneCharger, mCurrentBatteryClass);
	DOREPLIFETIME(AAVRPBuildableDroneCharger, mSavedBatteryAmount);
	DOREPLIFETIME(AAVRPBuildableDroneCharger, mCurrentRechargeProgress);
}

bool AAVRPBuildableDroneCharger::CanProduce_Implementation() const
{
	return HasPower() && IsValid(GetDroneStation()) && mSavedBatteryAmount > 0 && GetFuelInventory()->HasEnoughSpaceForItem(FInventoryItem(mCurrentBatteryClass));
}

void AAVRPBuildableDroneCharger::Factory_TickProducing(float dt)
{
	Super::Factory_TickProducing(dt);
	if (!HasAuthority()) {
		return;
	} 
	mCurrentRechargeProgress += 1000 / UFGItemDescriptor::GetEnergyValue(mCurrentBatteryClass) * dt;
	while (mCurrentRechargeProgress >= 1 && GetFuelInventory()->HasEnoughSpaceForItem(FInventoryItem(mCurrentBatteryClass)) && mSavedBatteryAmount > 0) {
		mCurrentRechargeProgress -= 1;
		mSavedBatteryAmount -= 1;
		GetFuelInventory()->AddItem(FInventoryItem(mCurrentBatteryClass));
	}
}

void AAVRPBuildableDroneCharger::OnFuelRemoved(TSubclassOf<UFGItemDescriptor> itemClass, int32 numRemoved)
{
	if (!HasAuthority()) return;
	bool isBattery = UContentTagRegistry::Get(this)->GetGameplayTagContainerFor(itemClass).HasTag(FGameplayTag::RequestGameplayTag("Item.Battery"));
	if (isBattery) {
		if (mCurrentBatteryClass != itemClass) {
			mCurrentBatteryClass = itemClass;
			mCurrentRechargeProgress = 0;
			mSavedBatteryAmount = 0;
		}
		mSavedBatteryAmount += numRemoved;
	}
}