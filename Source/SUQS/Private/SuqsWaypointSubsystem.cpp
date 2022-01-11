﻿#include "SuqsWaypointSubsystem.h"
#include "SuqsWaypointComponent.h"

void USuqsWaypointSubsystem::RegisterWaypoint(USuqsWaypointComponent* Waypoint)
{
	auto& List = WaypointsByQuest.FindOrAdd(Waypoint->GetQuestID());

	// group by task, then if there's >1 waypoint for a single task, order by sequence index for later convenience
	bool bInserted = false;
	bool bExistingTask = false;
	for (int i = 0; i < List.Num(); ++i)
	{
		const auto Curr = List[i];

		// Avoid duplicates
		if (Curr == Waypoint)
			bInserted = true;
		
		if (Curr->GetTaskID() == Waypoint->GetTaskID())
		{
			bExistingTask = true;
		}

		// If we've seen a waypoint for this task previously:
		// Insert here if we're now on a diff task (thus, group)
		//  or if we're on the same task and our sequence index is before this one
		if (bExistingTask &&
			(Curr->GetSequenceIndex() > Waypoint->GetSequenceIndex() ||
			Curr->GetTaskID() != Waypoint->GetTaskID()))
		{
			List.Insert(Waypoint, i);
			bInserted = true;
			break;
		}

	}

	if (!bInserted)
		List.Add(Waypoint);
}

void USuqsWaypointSubsystem::UnregisterWaypoint(USuqsWaypointComponent* Waypoint)
{
	const auto pList = WaypointsByQuest.Find(Waypoint->GetQuestID());
	if (pList)
	{
		pList->RemoveSingle(Waypoint);
	}
	
}

USuqsWaypointComponent* USuqsWaypointSubsystem::GetWaypoint(const FName& QuestID,
                                                            const FName& TaskID,
                                                            bool bOnlyEnabled)
{
	const auto pList = WaypointsByQuest.Find(QuestID);
	if (pList)
	{
		for (auto W : *pList)
		{
			if (W->GetTaskID() == TaskID &&
				(!bOnlyEnabled || W->IsEnabled()))
			{
				// List is pre-sorted by sequence index & grouped by task so this is the first
				return W;
			}
		}
	}
	return nullptr;
}

bool USuqsWaypointSubsystem::GetWaypoints(const FName& QuestID,
                                          const FName& TaskID,
                                          bool bOnlyEnabled,
                                          TArray<USuqsWaypointComponent*>& OutWaypoints)
{
	const auto pList = WaypointsByQuest.Find(QuestID);
	bool bAnyFound = false;
	if (pList)
	{
		bool bFoundTask = false;
		for (auto W : *pList)
		{
			if (W->GetTaskID() == TaskID)
			{
				bFoundTask = true;
				if (!bOnlyEnabled || W->IsEnabled())
				{
					// Waypoints are pre-sorted by SequenceIndex
					OutWaypoints.Add(W);
					// we won't use OutWayPoints.Num() to determine if we found anything, because we're not clearing the list
					// to give callers the flexibility of appending to an existing list if they want
					bAnyFound = true;
				}
			}
			else if (bFoundTask)
			{
				// Tasks are grouped in the list already so we can process in sequence
				// Once we exit the task we know we're done
				break;
			}
		}
	}
	return bAnyFound;
}

void USuqsWaypointSubsystem::NotifyWaypointMoved(USuqsWaypointComponent* Waypoint) const
{
	// Just relay
	OnWaypointMoved.Broadcast(Waypoint);
}

void USuqsWaypointSubsystem::NotifyWaypointEnabledChanged(USuqsWaypointComponent* Waypoint) const
{
	// Just relay
	OnWaypointEnabledChanged.Broadcast(Waypoint);
}
