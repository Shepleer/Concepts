// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ConceptsCharacterMovementComponent.generated.h"

UCLASS()
class CONCEPTS_API UConceptsCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Concepts : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToSprint : 1;

		/** Clear saved move properties, so it can be re-used. */
		virtual void Clear() override;

		/** Called to set up this saved move (when initially created) to make a predictive correction. */
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;

		/** Called before ClientUpdatePosition uses this SavedMove to make a predictive correction	 */
		virtual void PrepMoveFor(ACharacter* C) override;

		/** Returns true if this move can be combined with NewMove for replication without changing any behavior */
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

		/** Returns a byte containing encoded special movement information (jumping, crouching, etc.)	 */
		virtual uint8 GetCompressedFlags() const override;
	};

	class FNetworkPredictionData_Client_Concepts : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Concepts(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	UPROPERTY(Category = "Character Movement: Walking", EditDefaultsOnly, meta = (ClampMin = "0", UIMin = "0", ForceUnits = "cm/s"))
	float MaxSprintSpeed = 700.f;

private:
	bool Safe_bWantsToSprint;

public:
	UConceptsCharacterMovementComponent();

public:
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity) override;

public:
	virtual float GetMaxSpeed() const override;

public:
	void StartCrouching();
	void StopCrouching();
	void StartRunning();
	void StopRunning();
};