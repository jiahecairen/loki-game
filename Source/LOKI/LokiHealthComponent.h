// LOKI - Health / damage / death component (C1).

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LokiHealthComponent.generated.h"

class UDamageType;
class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FLokiOnHealthChanged, float, NewHealth, float, Delta, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLokiOnDeath, AActor*, Killer);

/**
 * Owns an actor's health. Listens to the owner's OnTakeAnyDamage, so any
 * ApplyDamage / ApplyRadialDamage aimed at the owner reaches it.
 * Gameplay reactions (HUD text, restart, ragdoll) stay in Blueprint via the events.
 */
UCLASS(ClassGroup=(LOKI), meta=(BlueprintSpawnableComponent), DisplayName="Loki Health")
class LOKI_API ULokiHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULokiHealthComponent();

	/** Health at BeginPlay / after ResetHealth. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOKI|Health", meta=(ClampMin="1"))
	float MaxHealth = 100.f;

	/** While true, all damage is ignored (e.g. during cutscenes). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOKI|Health")
	bool bInvulnerable = false;

	/** Ignore damage whose causer is the owner itself (own ApplyRadialDamage attacks). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LOKI|Health")
	bool bIgnoreSelfDamage = true;

	/** Fired after every accepted hit or heal. Delta is negative for damage. */
	UPROPERTY(BlueprintAssignable, Category="LOKI|Health")
	FLokiOnHealthChanged OnHealthChanged;

	/** Fired once when Health reaches 0. */
	UPROPERTY(BlueprintAssignable, Category="LOKI|Health")
	FLokiOnDeath OnDeath;

	UFUNCTION(BlueprintPure, Category="LOKI|Health")
	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintPure, Category="LOKI|Health")
	float GetHealthPercent() const { return MaxHealth > 0.f ? Health / MaxHealth : 0.f; }

	UFUNCTION(BlueprintPure, Category="LOKI|Health")
	bool IsDead() const { return bDead; }

	/** True when Actor has a LokiHealth component that is dead. Actors without one count as alive. */
	UFUNCTION(BlueprintPure, Category="LOKI|Health")
	static bool IsActorDead(const AActor* Actor);

	/** Applies damage directly (same rules as engine damage). Returns the damage actually taken. */
	UFUNCTION(BlueprintCallable, Category="LOKI|Health")
	float ApplyHealthDamage(float Damage, AActor* DamageCauser);

	/** Heals a living owner, up to MaxHealth. */
	UFUNCTION(BlueprintCallable, Category="LOKI|Health")
	void Heal(float Amount);

	/** Full health, alive again. */
	UFUNCTION(BlueprintCallable, Category="LOKI|Health")
	void ResetHealth();

	/** Debug only: show "HP x/y | Invulnerable: YES/NO" on screen every frame while enabled. */
	UFUNCTION(BlueprintCallable, Category="LOKI|Health|Debug")
	void SetDebugDisplay(bool bEnable);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:
	uint64 GetDebugMessageKey() const;

	UPROPERTY(VisibleInstanceOnly, Category="LOKI|Health")
	float Health = 100.f;

	UPROPERTY(VisibleInstanceOnly, Category="LOKI|Health")
	bool bDead = false;
};
