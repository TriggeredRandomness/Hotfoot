// MIT LicenseCopyright (c) 2021 LinMAD

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ObjectMacros.h"
#include "HFVehicle.generated.h"

UCLASS()
class HOTFOOT_API AHFVehicle : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	AHFVehicle();

private:
	UPROPERTY(EditAnywhere, Category = "Config|Camera")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = "Config|Camera")
	USpringArmComponent* SpringArm;

	#pragma region Vehicle State Control
	bool bIsDead;
	#pragma endregion Vehicle State Control
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	#pragma region Vehicle dynamics

	#pragma endregion Vehicle dynamics

private:

	#pragma region Player input controlls

	void InputLookUp(float Value);
	void InputLookRight(float Value);

	void InputThrottle(float Value);
	void InputSteering(float Value);

	void OnInputHandbrakePressed();
	void OnInputHandbrakeReleased();

	#pragma endregion Player input controlls
};
