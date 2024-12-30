// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"

#include "GameFeatureAction_RegisterQuest.generated.h"

class USuqsRegisterQuestSubsystem;
/**
 * 
 */
UCLASS()
class SUQS_API UGameFeatureAction_RegisterQuest : public UGameFeatureAction
{
	GENERATED_BODY()

	void HandleGameInstanceStart(UGameInstance* GameInstance);
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

public:
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UDataTable>> QuestTables;

	UPROPERTY()
	USuqsRegisterQuestSubsystem* RegisterQuestSubsystem;
	FDelegateHandle StartGameInstanceHandle;
};
