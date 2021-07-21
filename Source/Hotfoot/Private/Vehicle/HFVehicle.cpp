// MIT LicenseCopyright (c) 2021 LinMAD

#include "Vehicle/HFVehicle.h"
#include "Engine.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "WheeledVehicleMovementComponent4W.h"

AHFVehicle::AHFVehicle()
{
	SetReplicates(true);

	NetUpdateFrequency = 120.f;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	UWheeledVehicleMovementComponent4W* Vehicle4WComponent = Cast<UWheeledVehicleMovementComponent4W>(GetVehicleMovementComponent());

	// Tire Setup
	Vehicle4WComponent->MinNormalizedTireLoad = 0.f;
	Vehicle4WComponent->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4WComponent->MaxNormalizedTireLoad = 2.f;
	Vehicle4WComponent->MaxNormalizedTireLoadFiltered = 2.f;

	// Engine Torque Setup
	Vehicle4WComponent->MaxEngineRPM = 6000.f;
	Vehicle4WComponent->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4WComponent->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 400.f);
	Vehicle4WComponent->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1800.f, 500.f);
	Vehicle4WComponent->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5800.f, 400.f);

	// Steering Setup
	Vehicle4WComponent->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4WComponent->SteeringCurve.GetRichCurve()->AddKey(0.f, 1.f);
	Vehicle4WComponent->SteeringCurve.GetRichCurve()->AddKey(40.f, 0.7f);
	Vehicle4WComponent->SteeringCurve.GetRichCurve()->AddKey(120.f, 0.6f);

	// Differential Setup
	Vehicle4WComponent->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4WComponent->DifferentialSetup.FrontRearSplit = 0.7f;

	// Gearbox setup
	Vehicle4WComponent->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4WComponent->TransmissionSetup.GearSwitchTime = 0.1f;
	Vehicle4WComponent->TransmissionSetup.GearAutoBoxLatency = 1.f;

	//
	// Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleSpringArm"));
	SpringArm->PrimaryComponentTick.TickGroup = PrimaryActorTick.TickGroup;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCamera"));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;
	Camera->PrimaryComponentTick.TickGroup = PrimaryActorTick.TickGroup;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void AHFVehicle::BeginPlay()
{
	Super::BeginPlay();
}

void AHFVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHFVehicle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//
	// Camera
	PlayerInputComponent->BindAxis("LookUp", this, &AHFVehicle::InputLookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AHFVehicle::InputLookRight);

	//
	// Driving
	PlayerInputComponent->BindAxis("Throttle", this, &AHFVehicle::InputThrottle);
	PlayerInputComponent->BindAxis("Steering", this, &AHFVehicle::InputSteering);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AHFVehicle::OnInputHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AHFVehicle::OnInputHandbrakeReleased);
}

void AHFVehicle::InputLookUp(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Looking up value: %f"), Value)

	if (Value != 0.f)
		AddControllerPitchInput(Value);
}

void AHFVehicle::InputLookRight(float Value)
{
	if (Value != 0.f)
		AddControllerYawInput(Value);
}

void AHFVehicle::InputThrottle(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Throttle: %f"), Value);

	GetVehicleMovementComponent()->SetThrottleInput(Value);
}

void AHFVehicle::InputSteering(float Value)
{
	GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void AHFVehicle::OnInputHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AHFVehicle::OnInputHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}
