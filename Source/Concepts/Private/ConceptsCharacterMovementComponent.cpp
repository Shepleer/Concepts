// Fill out your copyright notice in the Description page of Project Settings.


#include "ConceptsCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UConceptsCharacterMovementComponent::UConceptsCharacterMovementComponent()
{
	NavAgentProps.bCanCrouch = true;
}

FNetworkPredictionData_Client* UConceptsCharacterMovementComponent::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		UConceptsCharacterMovementComponent* MutableThis = const_cast<UConceptsCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Concepts(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UConceptsCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UConceptsCharacterMovementComponent::OnMovementUpdated(float DeltaTime, const FVector& OldLocation, const FVector& OldVelocity)
{
}

float UConceptsCharacterMovementComponent::GetMaxSpeed() const
{
	if (MovementMode == MOVE_Walking && Safe_bWantsToSprint && !IsCrouching())
	{
		return MaxSprintSpeed;
	}

	return Super::GetMaxSpeed();
}

void UConceptsCharacterMovementComponent::StartCrouching()
{
	bWantsToCrouch = true;
}

void UConceptsCharacterMovementComponent::StopCrouching()
{
	if (IsCrouching()) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("asd"));
	}
	bWantsToCrouch = false;
}

void UConceptsCharacterMovementComponent::StartRunning()
{
	Safe_bWantsToSprint = true;
}

void UConceptsCharacterMovementComponent::StopRunning()
{
	Safe_bWantsToSprint = false;
}

void UConceptsCharacterMovementComponent::FSavedMove_Concepts::Clear()
{
	FSavedMove_Character::Clear();
	Saved_bWantsToSprint = 0;
}

void UConceptsCharacterMovementComponent::FSavedMove_Concepts::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UConceptsCharacterMovementComponent* CharacterMovement = Cast<UConceptsCharacterMovementComponent>(C->GetCharacterMovement());
	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UConceptsCharacterMovementComponent::FSavedMove_Concepts::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UConceptsCharacterMovementComponent* CharacterMovement = Cast<UConceptsCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

bool UConceptsCharacterMovementComponent::FSavedMove_Concepts::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	const FSavedMove_Concepts* SavedMove = static_cast<FSavedMove_Concepts*>(NewMove.Get());

	if (Saved_bWantsToSprint != SavedMove->Saved_bWantsToSprint) {
		return false;
	}

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

uint8 UConceptsCharacterMovementComponent::FSavedMove_Concepts::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToSprint) {
		Result |= FLAG_Custom_0;
	}

	return Result;
}

UConceptsCharacterMovementComponent::FNetworkPredictionData_Client_Concepts::FNetworkPredictionData_Client_Concepts(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr UConceptsCharacterMovementComponent::FNetworkPredictionData_Client_Concepts::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Concepts());
}
