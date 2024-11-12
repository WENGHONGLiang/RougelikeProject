// Fill out your copyright notice in the Description page of Project Settings.


#include "RLProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "RougelikeProject/AbilitySystem/RLGameplayTags.h"


void URLProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TODO: 找到最近的敌人，设为目标位置
	//TargetLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + GetAvatarActorFromActorInfo()->GetActorForwardVector();
}

void URLProjectileSpell::SpawnProjectile()
{
	// 生成 Projectile
	const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FRotator Rotation = (TargetLocation - SpawnLocation).Rotation();
	
	Rotation.Pitch = 0;
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	ARLProjectile* Projectile = GetWorld()->SpawnActorDeferred<ARLProjectile>(ProjectileClass,
								SpawnTransform,
								GetOwningActorFromActorInfo(),
								Cast<APawn>(GetOwningActorFromActorInfo()),
								ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	
	// Give The Projectile a GE for causing damage.
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	
	const float RealDamage = BaseDamage * (1 + 0.1 * (GetAbilityLevel()-1)); // 每级提升 0.1
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FRLGameplayTags::Get().GE_Damage, RealDamage);
	
	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}