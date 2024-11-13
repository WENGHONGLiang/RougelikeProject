// Fill out your copyright notice in the Description page of Project Settings.


#include "RLEnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "RougelikeProject/CharacterSet/RLCharacter.h"
#include "RougelikeProject/GameMode/RLGameMode.h"

ARLEnemySpawner::ARLEnemySpawner()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	RootComponent = BoxComp;

	SpawnTurn = 1;
	CurTurn = 0;
}

void ARLEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARLEnemySpawner::BeginOverlap);
}

void ARLEnemySpawner::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARLCharacter* RLCharacter = Cast<ARLCharacter>(OtherActor);
	if(!RLCharacter) return;
	
	if(!bStartSpawn)
	{
		const FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ARLEnemySpawner::SpawnEnemy);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, Delegate, 2, true);
	}
}

void ARLEnemySpawner::SpawnEnemy()
{
	if(bStartSpawn)
	{
		for (const auto Enemy : CurEnemies)
		{
			if(Enemy->GetCharacterAlive()) // 检查敌人是否都死亡是否
				return;
		}
	}
	else
	{
		bStartSpawn = true;
	}

	CurEnemies.Reset();
	
	int EnemyIndex = FMath::RandRange(0, EnemyConfig.Num()-1);
	ECharacterType EnemyType = EnemyConfig[EnemyIndex].EnemyType;
	int EnemyNumber = EnemyConfig[EnemyIndex].EnemyNumber;
	ARLGameMode* RLGameMode = Cast<ARLGameMode>(UGameplayStatics::GetGameMode(this));
	FVector BoxExtent = BoxComp->GetCollisionShape().GetExtent();
	int SpawnCnt = 0;
	
	for(int i = 0; i < EnemyNumber; i++)
	{
		FVector SpawnLocation = FVector(GetActorLocation().X + FMath::RandRange(-BoxExtent.X, BoxExtent.X), GetActorLocation().Y + FMath::RandRange(-BoxExtent.Y, BoxExtent.Y), 10);
		ARLEnemy* Enemy = RLGameMode->SpawnEnemyAtLocation(EnemyType, SpawnLocation);

		if(!Enemy) // 生成失败，重新生成
		{
			i--;
		}
		else
		{
			CurEnemies.Add(Enemy);
		}

		if(SpawnCnt++ >= 100) // 防死循环
			break;
	}

	CurTurn++;
	if(CurTurn == SpawnTurn)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}