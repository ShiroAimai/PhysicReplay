// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EntityReplayComponent.h"
#include "ReflectionUtilsFunctionLibrary.h"
#include <Components/StaticMeshComponent.h>
#include "ReplayLogContext.h"

#pragma optimize("", off)
UEntityReplayComponent::UEntityReplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
}

void UEntityReplayComponent::InitializeComponent()
{
	Super::InitializeComponent();

	AActor* Owner = GetOwner();
	if (Owner != nullptr)
	{
		OwnerMeshComp = Owner->FindComponentByClass<UStaticMeshComponent>();
	}
}

void UEntityReplayComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	OwnerMeshComp = nullptr;
}

void UEntityReplayComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerMeshComp == nullptr)
	{
		UE_LOG(LogReplay, Warning, TEXT("Missing Owner Mesh Component"));
		return;
	}

	ReplayLocation = OwnerMeshComp->GetComponentLocation();
	ReplayRotation = OwnerMeshComp->GetComponentRotation();
	ReplayVelocity = OwnerMeshComp->GetPhysicsLinearVelocity();
	ReplayAngularVelocity = OwnerMeshComp->GetPhysicsAngularVelocityInDegrees();
}

void UEntityReplayComponent::GetReplayData(const FString& RequestedTrackProperties, TArray<FVector>& OutTrackPropertiesValues)
{
	/**
	 *	RequestedTrackProperties should be split based on ","
	 *
	 */
	if (RequestedTrackProperties.Len() == 0)
	{
		UE_LOG(LogReplay, Warning, TEXT("Passed invalid properties to method GetReplayData"));
		return;
	}

	TArray<FString> TrackedProperties;
	RequestedTrackProperties.ParseIntoArray(TrackedProperties, TEXT(","));

	for (const FString& PropertyName : TrackedProperties)
	{
		FString Path = "This";
		Path += ".";
		Path += PropertyName;

		void* OutObject = nullptr;
		FProperty* Property = UReflectionUtilsFunctionLibrary::RetrieveProperty(this, Path, OutObject);

		if (Property)
		{
			void* StructAddress = Property->ContainerPtrToValuePtr<void>(OutObject);

			if (StructAddress)
			{
				//check property is struct
				FStructProperty* StructProperty = CastField<FStructProperty>(Property);
				if (StructProperty)
				{
					//check property is vector
					if (StructProperty->Struct == TBaseStructure<FVector>::Get())
					{
						FVector Value = FVector::ZeroVector;
						StructProperty->CopyCompleteValue(&Value, StructAddress);
						UE_LOG(LogReplay, Error, TEXT("Vector value is : %s"), *Value.ToString())
						OutTrackPropertiesValues.Add(Value);
					}
				}
			}
		}

	}
}

void UEntityReplayComponent::SetReplayData(const TArray<FVector>& spec) const
{

}
#pragma optimize("", on)