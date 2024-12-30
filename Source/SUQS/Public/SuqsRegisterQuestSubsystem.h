// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SuqsRegisterQuestSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SUQS_API USuqsRegisterQuestSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void RegisterQuest(const TSoftObjectPtr<UDataTable>& Quest);

	UFUNCTION(BlueprintCallable)
	void UnregisterQuest(const TSoftObjectPtr<UDataTable>& Quest);

	UPROPERTY(BlueprintReadOnly)
	TArray<TSoftObjectPtr<UDataTable>> RegisteredQuests;
};
