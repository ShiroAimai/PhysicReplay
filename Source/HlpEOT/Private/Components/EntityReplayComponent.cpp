// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EntityReplayComponent.h"
#include "ReflectionUtilsFunctionLibrary.h"
#include <Components/StaticMeshComponent.h>
#include "ReplayLogContext.h"

#pragma optimize("", off)
UEntityReplayComponent::UEntityReplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ReplayState = EReplayState::NONE;
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

	if (ReplayState == EReplayState::PLAYER_DRIVEN)
	{
		ReplayLocation = OwnerMeshComp->GetComponentLocation();
		ReplayRotation = OwnerMeshComp->GetComponentRotation();
		ReplayVelocity = OwnerMeshComp->GetPhysicsLinearVelocity();
		ReplayAngularVelocity = OwnerMeshComp->GetPhysicsAngularVelocityInDegrees();
	}
	else
	{
		AActor* Owner = GetOwner();
		Owner->SetActorLocation(ReplayLocation);
		Owner->SetActorRotation(ReplayRotation);

		OwnerMeshComp->SetPhysicsLinearVelocity(ReplayVelocity);
		OwnerMeshComp->SetPhysicsAngularVelocityInDegrees(ReplayAngularVelocity);
	}
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
		void* OutObject = nullptr;
		FProperty* Property = GetLocalPropertyByString(PropertyName, OutObject);

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
					//check if it is rotator
					if (StructProperty->Struct == TBaseStructure<FRotator>::Get())
					{
						FRotator Value = FRotator::ZeroRotator;
						StructProperty->CopyCompleteValue(&Value, StructAddress);
						UE_LOG(LogReplay, Error, TEXT("Rotator value is : %s"), *Value.ToString())
						OutTrackPropertiesValues.Add(Value.Euler()); //we extract Euler representation
					}
				}
			}
		}
	}
}

void UEntityReplayComponent::SetReplayData(const FString& ReplayProperties, const TArray<FVector>& Specs)
{
	/**
	 *	ReplayProperties should be split based on ","
	 *
	 */
	if (ReplayProperties.Len() == 0 || Specs.Num() == 0)
	{
		UE_LOG(LogReplay, Warning, TEXT("Passed invalid parameters to method SetReplayData"));
		return;
	}

	TArray<FString> ReplayingProperties;
	ReplayProperties.ParseIntoArray(ReplayingProperties, TEXT(","));

	if (ReplayingProperties.Num() != Specs.Num())
	{
		UE_LOG(LogReplay, Error, TEXT("ReplayProperties and Specs are of different lenghts. Must be of the same length"));
		return;
	}

	for(size_t i = 0; i < ReplayingProperties.Num();++i)
	{
		void* OutObject = nullptr;
		FProperty* Property = GetLocalPropertyByString(ReplayingProperties[i], OutObject);

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
						StructProperty->CopyCompleteValue(StructAddress, &Specs[i]);
					}
					//check if it is rotator
					if (StructProperty->Struct == TBaseStructure<FRotator>::Get())
					{
						FRotator Rotation = FRotator::MakeFromEuler(Specs[i]);
						StructProperty->CopyCompleteValue(StructAddress, &Rotation);
					}
				}
			}
		}
	}
}

void UEntityReplayComponent::UpdateReplayStateWith(EReplayState NewState)
{
	ReplayState = NewState;
}

FProperty* UEntityReplayComponent::GetLocalPropertyByString(const FString& PropertyName, void*& OutObject)
{
	FString Path = "This";
	Path += ".";
	Path += PropertyName;

	return UReflectionUtilsFunctionLibrary::RetrieveProperty(this, Path, OutObject);
}

#pragma optimize("", on)