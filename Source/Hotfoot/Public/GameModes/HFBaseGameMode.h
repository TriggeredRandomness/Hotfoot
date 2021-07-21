// MIT LicenseCopyright (c) 2021 LinMAD

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HFBaseGameMode.generated.h"

UCLASS()
class HOTFOOT_API AHFBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
};
