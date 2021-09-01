// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaCharacterStats.h"
#include "ReflectionUtilsFunctionLibrary.h"
UArenaCharacterStats::UArenaCharacterStats()
{
	PrimaryComponentTick.bCanEverTick = false;
}

/**
*	"StatName": Changing Property Name
* 
* 
*/
bool UArenaCharacterStats::ApplyStatChange(AActor* InstigatorActor, const FString& StatName, float Delta)
{
	if (StatName.IsEmpty() || Delta < 0.0f) return false;

	FString Path = "This.";
	Path += StatName;
	
	void* OutObject = nullptr;
	FProperty* ChangingProperty = UReflectionUtilsFunctionLibrary::RetrieveProperty(this, Path, OutObject);

	if(!ChangingProperty) return false;

	FFloatProperty* ChangingFloatProperty = CastField<FFloatProperty>(ChangingProperty);
	if(ChangingFloatProperty)
	{
		float OldValue = ChangingFloatProperty->GetPropertyValue_InContainer(OutObject);
		ChangingFloatProperty->SetPropertyValue_InContainer(OutObject, OldValue + Delta);
		float NewValue = ChangingFloatProperty->GetPropertyValue_InContainer(OutObject);
		float ActualDelta = NewValue - OldValue;

		if (ActualDelta != 0.0f)
		{
			MulticastStatsChanged(InstigatorActor, ChangingFloatProperty->GetFName(), NewValue, ActualDelta);
		}

		return ActualDelta != 0;
	}
	
	return false;
}

void UArenaCharacterStats::MulticastStatsChanged(AActor* InstigatorActor, FName PropertyName, float NewValue, float Delta)
{
	OnStatChanged.Broadcast(InstigatorActor, PropertyName, NewValue, Delta);
}

