// LOKI - Health / damage / death component (C1).

#include "LokiHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

ULokiHealthComponent::ULokiHealthComponent()
{
	// Ticks only while the debug display is on (SetDebugDisplay).
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

bool ULokiHealthComponent::IsActorDead(const AActor* Actor)
{
	const ULokiHealthComponent* Comp = Actor ? Actor->FindComponentByClass<ULokiHealthComponent>() : nullptr;
	return Comp != nullptr && Comp->IsDead();
}

void ULokiHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	bDead = false;

	if (AActor* Owner = GetOwner())
	{
		Owner->OnTakeAnyDamage.AddUniqueDynamic(this, &ULokiHealthComponent::HandleTakeAnyDamage);
	}
}

void ULokiHealthComponent::HandleTakeAnyDamage(AActor* /*DamagedActor*/, float Damage, const UDamageType* /*DamageType*/, AController* /*InstigatedBy*/, AActor* DamageCauser)
{
	ApplyHealthDamage(Damage, DamageCauser);
}

float ULokiHealthComponent::ApplyHealthDamage(float Damage, AActor* DamageCauser)
{
	if (Damage <= 0.f || bDead || bInvulnerable)
	{
		return 0.f;
	}
	if (bIgnoreSelfDamage && DamageCauser != nullptr && DamageCauser == GetOwner())
	{
		return 0.f;
	}

	const float OldHealth = Health;
	Health = FMath::Max(0.f, Health - Damage);
	const float Taken = OldHealth - Health;

	OnHealthChanged.Broadcast(Health, -Taken, DamageCauser);

	if (Health <= 0.f)
	{
		bDead = true;
		OnDeath.Broadcast(DamageCauser);
	}
	return Taken;
}

void ULokiHealthComponent::Heal(float Amount)
{
	if (Amount <= 0.f || bDead)
	{
		return;
	}
	const float OldHealth = Health;
	Health = FMath::Min(MaxHealth, Health + Amount);
	if (Health != OldHealth)
	{
		OnHealthChanged.Broadcast(Health, Health - OldHealth, nullptr);
	}
}

void ULokiHealthComponent::ResetHealth()
{
	const float OldHealth = Health;
	Health = MaxHealth;
	bDead = false;
	if (Health != OldHealth)
	{
		OnHealthChanged.Broadcast(Health, Health - OldHealth, nullptr);
	}
}

void ULokiHealthComponent::SetDebugDisplay(bool bEnable)
{
	SetComponentTickEnabled(bEnable);
	if (!bEnable && GEngine)
	{
		GEngine->RemoveOnScreenDebugMessage(GetDebugMessageKey());
	}
}

void ULokiHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GEngine)
	{
		const FString Text = FString::Printf(TEXT("[%s] HP %.0f/%.0f | Invulnerable: %s%s"),
			GetOwner() ? *GetOwner()->GetActorNameOrLabel() : TEXT("?"),
			Health, MaxHealth,
			bInvulnerable ? TEXT("YES") : TEXT("NO"),
			bDead ? TEXT(" | DEAD") : TEXT(""));
		GEngine->AddOnScreenDebugMessage(GetDebugMessageKey(), 0.f, bInvulnerable ? FColor::Yellow : FColor::Green, Text);
	}
}

uint64 ULokiHealthComponent::GetDebugMessageKey() const
{
	return static_cast<uint64>(GetUniqueID()) + 0x4C4F4B4900000000ull; // "LOKI" prefix keeps it apart from Blueprint keys
}
