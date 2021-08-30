// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ArenaWeapon.h"
#include "ArenaWorldWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"


AArenaPlayer::AArenaPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetRelativeRotation(FRotator(-70.f, 0, 0));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->TargetArmLength = 600.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AArenaPlayer::BeginPlay()
{
	Super::BeginPlay();
	Weapon = GetWorld()->SpawnActor<AArenaWeapon>(selectedWeapon.Get(), GetTransform());
	if (Weapon == nullptr) return;

	const FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(GetMesh(), rules, TEXT("GunSocket"));
	onFireStart.BindDynamic(Weapon, &AArenaWeapon::StartFire);
	onFireStop.BindDynamic(Weapon, &AArenaWeapon::StopFire);
}

void AArenaPlayer::Tick(float DeltaSeconds)
{
	LookAtMouseCursor();
}

// Called to bind functionality to input
void AArenaPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUp", this, &AArenaPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArenaPlayer::MoveRight);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AArenaPlayer::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AArenaPlayer::CeaseFire);
}

void AArenaPlayer::MoveForward(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
}

void AArenaPlayer::MoveRight(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
}

void AArenaPlayer::LookAtMouseCursor()
{
	FVector characterPosition = GetActorLocation();
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	FVector mousePosition = FVector::ZeroVector, mouseRotation = FVector::ZeroVector;
	controller->DeprojectMousePositionToWorld(mousePosition, mouseRotation);

	FVector intersectionPoint = FMath::RayPlaneIntersection(mousePosition, mouseRotation, FPlane(characterPosition, FVector::UpVector));

	FRotator NewActorRotation = UKismetMathLibrary::FindLookAtRotation(characterPosition, intersectionPoint);

	SetActorRotation(NewActorRotation);
}

void AArenaPlayer::Fire()
{
	onFireStart.ExecuteIfBound();
}

void AArenaPlayer::CeaseFire()
{
	onFireStop.ExecuteIfBound();
}
