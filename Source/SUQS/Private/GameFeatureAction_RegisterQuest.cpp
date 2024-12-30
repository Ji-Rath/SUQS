// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatureAction_RegisterQuest.h"

#include "GameFeaturesSubsystem.h"
#include "SuqsRegisterQuestSubsystem.h"

void UGameFeatureAction_RegisterQuest::HandleGameInstanceStart(UGameInstance* GameInstance)
{
	for (auto& Quest : QuestTables)
	{
		RegisterQuestSubsystem = GameInstance->GetSubsystem<USuqsRegisterQuestSubsystem>();
		RegisterQuestSubsystem->RegisterQuest(Quest);
	}
}

void UGameFeatureAction_RegisterQuest::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	StartGameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddUObject(this, 
		&UGameFeatureAction_RegisterQuest::HandleGameInstanceStart);

	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		if (Context.ShouldApplyToWorldContext(WorldContext))
		{
			HandleGameInstanceStart(WorldContext.World()->GetGameInstance());
		}
	}
}

void UGameFeatureAction_RegisterQuest::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	FWorldDelegates::OnStartGameInstance.Remove(StartGameInstanceHandle);

	if (RegisterQuestSubsystem)
	{
		for (auto& Quest : QuestTables)
		{
			RegisterQuestSubsystem->UnregisterQuest(Quest);
		}
	}
}