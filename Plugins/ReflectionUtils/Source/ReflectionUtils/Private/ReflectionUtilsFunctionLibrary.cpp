#include "ReflectionUtilsFunctionLibrary.h"

#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"

FProperty* UReflectionUtilsFunctionLibrary::RetrieveProperty(UObject* InObject, const FString& InPath, void*& OutTarget)
{
	// Example:

	//		This.HP

	//		CharacterStats.HP

	ensureAlways(InObject);
	if (InObject == nullptr)
	{
		return nullptr;
	}

	FString ObjectString;
	FString PropertyString;
	bool bSucceded = InPath.Split(".", &ObjectString, &PropertyString);

	ensureAlways(bSucceded);
	
	UObject* TargetObj = nullptr;
	if (ObjectString.Equals("This", ESearchCase::IgnoreCase))
	{
		TargetObj = InObject;
	}
	else
	{
		AActor* InActor = Cast<AActor>(InObject);

		ensureAlways(InActor);

		if (InActor != nullptr)
		{
			TArray<UActorComponent*> Components;
			InActor->GetComponents(Components);

			for (size_t ComponentIndex = 0; ComponentIndex < Components.Num(); ++ComponentIndex)
			{
				UActorComponent* Component = Components[ComponentIndex];
				if (ObjectString.Equals(Component->GetName(), ESearchCase::IgnoreCase))
				{
					TargetObj = Component;
					break;
				}
			}
		}
	}

	OutTarget = TargetObj;

	FProperty* OutProperty;
	
	if (ensureAlways(TargetObj))
	{
		UClass* TargetClass = TargetObj->GetClass();
		
		FName PropertyName = FName(*PropertyString);
		
		OutProperty = TargetClass->FindPropertyByName(PropertyName);
	}
	else
	{
		OutProperty = nullptr;
	}

	return OutProperty;
}