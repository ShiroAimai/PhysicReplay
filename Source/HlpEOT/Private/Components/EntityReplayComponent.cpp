// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EntityReplayComponent.h"
#include "ReflectionUtilsFunctionLibrary.h"
#include <Components/StaticMeshComponent.h>
#include <PhysicsEngine/BodyInstance.h>
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
		UStaticMeshComponent* OwnerMeshComp = Owner->FindComponentByClass<UStaticMeshComponent>();
		if (OwnerMeshComp)
		{
			OwnerBI = OwnerMeshComp->GetBodyInstance();
		}
	}
}

void UEntityReplayComponent::UninitializeComponent()
{
	Super::UninitializeComponent();

	OwnerBI = nullptr;
}

void UEntityReplayComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerBI == nullptr)
	{
		UE_LOG(LogReplay, Warning, TEXT("Missing Owner Body Intance"));
		return;
	}
	AActor* Owner = GetOwner();
	if (ReplayState == EReplayState::PLAYER_DRIVEN)
	{
		ReplayLocation = Owner->GetActorLocation();
		ReplayRotation = Owner->GetActorRotation();

		ReplayVelocity = OwnerBI->GetUnrealWorldVelocity();
		ReplayAngularVelocity = OwnerBI->GetUnrealWorldAngularVelocityInRadians();
	}
	else if (ReplayState == EReplayState::REPLAY_DRIVEN)
	{
		Owner->SetActorLocation(ReplayLocation);
		Owner->SetActorRotation(ReplayRotation);
		
		OwnerBI->SetLinearVelocity(ReplayVelocity, false);
		OwnerBI->SetAngularVelocityInRadians(ReplayAngularVelocity, false);
	}
}

void UEntityReplayComponent::GetReplayData(const FString& RequestedTrackProperties, TArray<FVector>& OutTrackPropertiesValues)
{
	if (RequestedTrackProperties.Len() == 0)
	{
		UE_LOG(LogReplay, Warning, TEXT("Passed invalid properties to method GetReplayData"));
		return;
	}

	TArray<FString> TrackedProperties;
	ExtractTrackedProperties(RequestedTrackProperties, TrackedProperties);

	for (const FString& PropertyName : TrackedProperties)
	{
		//check property is struct
		void* StructAddress = nullptr;
		FStructProperty* StructProperty = RetrieveStructPropertyByString(PropertyName, StructAddress);
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

void UEntityReplayComponent::SetReplayData(const FString& ReplayProperties, const TArray<FVector>& Specs)
{
	if (ReplayProperties.Len() == 0 || Specs.Num() == 0)
	{
		UE_LOG(LogReplay, Warning, TEXT("Passed invalid parameters to method SetReplayData"));
		return;
	}

	TArray<FString> ReplayingProperties;
	ExtractTrackedProperties(ReplayProperties, ReplayingProperties);

	if (ReplayingProperties.Num() != Specs.Num())
	{
		UE_LOG(LogReplay, Error, TEXT("ReplayProperties and Specs are of different lenghts. Must be of the same length"));
		return;
	}

	for (size_t i = 0; i < ReplayingProperties.Num(); ++i)
	{
		//check property is struct
		void* StructAddress = nullptr;
		FStructProperty* StructProperty = RetrieveStructPropertyByString(ReplayingProperties[i], StructAddress);
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

void UEntityReplayComponent::UpdateReplayStateWith(EReplayState NewState)
{
	ReplayState = NewState;
}

void UEntityReplayComponent::ExtractTrackedProperties(const FString& RequestedProperties, TArray<FString>& OutSplittedProperties) const
{
	RequestedProperties.ParseIntoArray(OutSplittedProperties, TEXT(","));
}

FStructProperty* UEntityReplayComponent::RetrieveStructPropertyByString(const FString& PropertyName, void*& OutPropertyAddressInObject)
{
	void* OutObject = nullptr;
	FProperty* Property = GetLocalPropertyByString(PropertyName, OutObject);

	if (Property)
	{
		OutPropertyAddressInObject = Property->ContainerPtrToValuePtr<void>(OutObject);

		if (OutPropertyAddressInObject)
		{
			//check property is struct
			FStructProperty* StructProperty = CastField<FStructProperty>(Property);
			return StructProperty;
		}
	}

	OutPropertyAddressInObject = nullptr;
	return nullptr;
}

FProperty* UEntityReplayComponent::GetLocalPropertyByString(const FString& PropertyName, void*& OutObject)
{
	FString Path = "This";
	Path += ".";
	Path += PropertyName;

	return UReflectionUtilsFunctionLibrary::RetrieveProperty(this, Path, OutObject);
}

#pragma optimize("", on)