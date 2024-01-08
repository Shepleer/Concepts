// Copyright Epic Games, Inc. All Rights Reserved.

#include "ConceptsGameMode.h"
#include "ConceptsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AConceptsGameMode::AConceptsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Character/ConceptsCharacter/BP_MainPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
