// MIT LicenseCopyright (c) 2021 LinMAD


#include "GameModes/HFBaseGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AActor* AHFBaseGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PreferredSpawns;
	TArray<AActor*> PlayerStarts;
	APlayerStart* BestStart = nullptr;

	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		APlayerStart* TestStart = Cast<APlayerStart>(PlayerStarts[i]);

		PreferredSpawns.Add(TestStart);
	}

	if (PreferredSpawns.Num() > 0)
		BestStart = PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];

	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}

bool AHFBaseGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}
