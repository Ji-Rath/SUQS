// Fill out your copyright notice in the Description page of Project Settings.


#include "SuqsRegisterQuestSubsystem.h"

void USuqsRegisterQuestSubsystem::RegisterQuest(const TSoftObjectPtr<UDataTable>& Quest)
{
	RegisteredQuests.AddUnique(Quest);
}

void USuqsRegisterQuestSubsystem::UnregisterQuest(const TSoftObjectPtr<UDataTable>& Quest)
{
	RegisteredQuests.RemoveSwap(Quest);
}
