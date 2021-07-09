// MIT LicenseCopyright (c) 2021 LinMAD

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UObject/ObjectMacros.h"
#include "HFVehicle.generated.h"

USTRUCT() // Vehicle state data
struct FVehicleData
{
	GENERATED_USTRUCT_BODY()

	// Mass in (KG)
	UPROPERTY(EditDefaultsOnly, Category = "Config | Vehicle")
	float Mass;
	// Quantify the drag or resistance of an object in a fluid environment, such as air or water. (KG per M) 
	UPROPERTY(EditDefaultsOnly, Category = "Config | Vehicle")
	float DragCoefficent;
	// Ordinary car tires on concrete, resistance usually from 0.010 to 0.015.
	UPROPERTY(EditDefaultsOnly, Category = "Config | Vehicle")
	float RollingResistanceCoefficent;
	// Applied force when throttle is full down in (N - Newton)
	UPROPERTY(EditDefaultsOnly, Category = "Config | Vehicle")
	float MaxDrivingForce;
	// Vehicle turning circle (radius) in (M)
	UPROPERTY(EditDefaultsOnly, Category = "Config | Vehicle")
	float MinTurningRadius;

	//
	// Vehicle Dynamics
	float Throttle;
	float SteeringThrow;
	FVector Velocity;

	FVehicleData()
	{
		Mass = 1000.f;
		DragCoefficent = 16.f;
		RollingResistanceCoefficent = 0.015f;
		MaxDrivingForce = 10000.f;
		MinTurningRadius = 10.f;

		Throttle = 0.f;
		SteeringThrow = 0.f;
		
		Velocity = FVector::ZeroVector;
	}
};

UCLASS()
class HOTFOOT_API AHFVehicle : public APawn
{
	GENERATED_BODY()

public:
	AHFVehicle();

private:
	FVehicleData VehicleData;

	UPROPERTY(EditAnywhere, Category = "Config | Vehicle")
	USkeletalMeshComponent* VehicleMesh;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	#pragma region Vehicle dynamics

	FVector CalculateAirResistance();
	FVector CalculateRollingResistance();

	void UpdateRotation(float DeltaTime);
	void UpdateLocation(float DeltaTime);

	#pragma endregion Vehicle dynamics

private:
	#pragma region Player input controlls

	void Throttle(float Value);
	void Steering(float Value);

	#pragma endregion Player input controlls

	#pragma region Data access

	FORCEINLINE const FVehicleData& GetVehicleControl() const { return VehicleData; }

	#pragma endregion Data access
};
