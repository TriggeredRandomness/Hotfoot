// MIT LicenseCopyright (c) 2021 LinMAD

#include "Vehicle/HFVehicle.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

AHFVehicle::AHFVehicle()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	// Base skeletal mesh
	VehicleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VehicleMesh"));
	VehicleMesh->PrimaryComponentTick.TickGroup = PrimaryActorTick.TickGroup;

	SetRootComponent(VehicleMesh);
}

void AHFVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AHFVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * VehicleData.MaxDrivingForce * VehicleData.Throttle;
	Force += CalculateAirResistance();
	Force += CalculateRollingResistance();

	FVector Acceleration = Force / VehicleData.Mass;
	VehicleData.Velocity = VehicleData.Velocity + Acceleration * DeltaTime;

	UpdateRotation(DeltaTime);
	UpdateLocation(DeltaTime);
}

void AHFVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Throttle", this, &AHFVehicle::Throttle);
	PlayerInputComponent->BindAxis("Steering", this, &AHFVehicle::Steering);
}

#pragma region Vehicle dynamics

FVector AHFVehicle::CalculateAirResistance()
{
	return -VehicleData.Velocity.GetSafeNormal() * VehicleData.Velocity.SizeSquared() * VehicleData.DragCoefficent;
}

FVector AHFVehicle::CalculateRollingResistance()
{
	float AccelerationRelayedOnGravity = -this->GetWorld()->GetGravityZ() / 100;
	float NormalForce = VehicleData.Mass * AccelerationRelayedOnGravity;

	return -VehicleData.Velocity.GetSafeNormal() * VehicleData.RollingResistanceCoefficent * NormalForce;
}

void AHFVehicle::UpdateRotation(float DeltaTime)
{
	float Location = FVector::DotProduct(GetActorForwardVector(), VehicleData.Velocity) * DeltaTime;
	float RotationAngle = Location / VehicleData.MinTurningRadius * VehicleData.SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	VehicleData.Velocity = RotationDelta.RotateVector(VehicleData.Velocity);

	AddActorWorldRotation(RotationDelta);
}

void AHFVehicle::UpdateLocation(float DeltaTime)
{
	// Calculate movement in centimeters per second
	FVector Translation = VehicleData.Velocity * DeltaTime * 100;

	FHitResult Hit;
	AddActorWorldOffset(Translation, true, &Hit);

	// There is object on front of the car
	if (Hit.IsValidBlockingHit())
	{
		// TODO Apply collision effect
		VehicleData.Velocity = FVector::ZeroVector;
	}
}

#pragma endregion Vehicle dynamics

#pragma region Player input controlls

void AHFVehicle::Throttle(float Value)
{
	VehicleData.Throttle = Value;
}

void AHFVehicle::Steering(float Value)
{
	VehicleData.SteeringThrow = Value;
}

#pragma endregion Player input controlls

