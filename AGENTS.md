# AGENTS.md

This file provides guidance to agents when working with the SUQS plugin.

## 1. Plugin Overview

**SUQS** (Steve's Unreal Quest System) is a simple, data-driven quest system for Unreal Engine. It helps define quest structures and track progress against those quests for a player in a simple way.

### Key Capabilities

| Capability | Description |
|------------|-------------|
| **Data-Driven Quests** | Quests defined in data tables or JSON with schema validation |
| **Quest Branching** | Predefined branches within quests, quest dependencies |
| **Task Progression** | Tasks track actual progress with target numbers and time limits |
| **Objective Groups** | Objectives group tasks (sequential, flexible, or "N of M" completion) |
| **Event System** | Callbacks and query API for quest state changes |
| **Multiplayer Support** | `USuqsGameStateComponent` for network replication |
| **Save Integration** | `USuqsProgression` can be serialized for save games |

## 2. Runtime Requirements

| Environment | Minimum Version | Recommended Version |
|-------------|-----------------|---------------------|
| Unreal Engine | 5.0 | 5.7 |
| Windows | 10 | 11 |

## 3. Dependencies

### Build Dependencies (Build.cs)

| Module | Type | Required |
|--------|------|----------|
| Core | Public | Yes |
| CoreUObject | Public | Yes |
| Engine | Public | Yes |

### Plugin Dependencies (uplugin)

| Plugin | Purpose |
|--------|---------|
| None | Standalone plugin |

## 4. Module & Loading

| Property | Value |
|----------|-------|
| Module name | `SUQS` |
| Module type | `Runtime` |
| Loading phase | `Default` |
| Test module | `SUQSTest` (DeveloperTool) |

## 5. Key Classes

| Class | Type | Purpose |
|-------|------|---------|
| `USuqsProgression` | `UObject` | Main class tracking quest state for a player |
| `USuqsQuest` | `UDataAsset` | Quest definition data |
| `USuqsQuestState` | `UObject` | Runtime state of a quest |
| `USuqsObjectiveState` | `UObject` | Runtime state of an objective |
| `USuqsTaskState` | `UObject` | Runtime state of a task |
| `USuqsWaypointComponent` | `UActorComponent` | Waypoint for quest navigation |
| `USuqsWaypointActor` | `AActor` | Waypoint actor |
| `USuqsGameStateComponent` | `UActorComponent` | Multiplayer replication component |
| `USuqsProgressView` | `UObject` | Read-only view for UI in multiplayer |

## 6. Key Structs

| Struct | Blueprint Type | Purpose |
|--------|---------------|---------|
| `FSuqsQuest` | Yes | Quest definition structure |
| `FSuqsObjective` | Yes | Objective definition |
| `FSuqsTask` | Yes | Task definition |
| `FSuqsProgressionEventDetails` | Yes | Event details for callbacks |
| `ESuqsQuestStatus` | Yes | Quest status enum (Inactive, Active, Completed, Failed) |
| `ESuqsTaskStatus` | Yes | Task status enum |

## 7. Key Interfaces

| Interface | Methods |
|-----------|---------|
| `ISuqsParameterProvider` | `GetNamedFormatParameter()` - Provide text substitution values |

## 8. Project Structure

```
SUQS/
├── Source/
│   ├── SUQS/                    # Runtime module
│   │   ├── Public/
│   │   │   ├── SuqsProgression.h    # Main progression class
│   │   │   ├── SuqsQuest.h          # Quest definition
│   │   │   ├── SuqsQuestState.h     # Quest state
│   │   │   ├── SuqsObjectiveState.h # Objective state
│   │   │   ├── SuqsTaskState.h      # Task state
│   │   │   ├── SuqsParameterProvider.h # Text parameter interface
│   │   │   ├── SuqsWaypointComponent.h # Waypoint component
│   │   │   └── SuqsSaveData.h       # Save data structure
│   │   └── Private/
│   └── SUQSTest/                # Test module
├── docs/
│   ├── Quests.md
│   ├── Objectives.md
│   ├── Tasks.md
│   ├── Branching.md
│   ├── Progression.md
│   ├── Saving.md
│   ├── Multiplayer.md
│   └── questschema.json
```

## 9. Usage Patterns

### Defining Quests (JSON Example)

```json
[
  {
    "Identifier": "Q_Smol",
    "Title": "NSLOCTEXT(\"TestQuests\", \"SmolQuestTitle\", \"Smol Quest\")",
    "DescriptionWhenActive": "NSLOCTEXT(\"TestQuests\", \"SmolQuestDesc\", \"The smallest possible quest\")",
    "Objectives": [
      {
        "Tasks": [
          {
            "Identifier": "T_Smol",
            "Title": "NSLOCTEXT(\"TestQuests\", \"SmolQuestTaskTitle\", \"Easiest possible thing\")"
          }
        ]
      }
    ]
  }
]
```

### Quest Progression (C++)

```cpp
// Initialize with quest data tables
Progression->InitWithQuestDataTables(Tables);

// Accept a quest
Progression->AcceptQuest(FName("Q_MyQuest"));

// Complete a task
Progression->CompleteTask(FName("Q_MyQuest"), FName("T_MyTask"));

// Progress a task (for "collect N items" style tasks)
Progression->ProgressTask(FName("Q_MyQuest"), FName("T_Collect"), 1);
```

### Event Handling

```cpp
// Subscribe to quest events
Progression->OnQuestCompleted.AddLambda([](USuqsQuestState* Quest) {
    // Handle quest completion
});

Progression->OnTaskCompleted.AddLambda([](USuqsTaskState* Task) {
    // Handle task completion
});
```

## 10. Non-Obvious Code Patterns

- **Init required before use**: `InitWithQuestDataTables()` or similar must be called before any other `USuqsProgression` methods.
- **Single instance per player**: `USuqsProgression` should only have one instance per player.
- **Save serialization**: `USuqsProgression` must be included in save games. Use `SaveToData()` and `LoadFromData()`.
- **Multiplayer UI**: Use `USuqsProgressView` for client UI, not `USuqsProgression` directly.
- **Text localization**: Use `NSLOCTEXT` in JSON for localizable text.
- **Parameter providers**: Implement `ISuqsParameterProvider` for dynamic text substitution in quest text.

## 11. Common Pitfalls for New Contributors

1. **Not calling Init methods**: `USuqsProgression` must be initialized with quest data before use.
2. **Multiple progression instances**: Only one `USuqsProgression` per player, or state will be inconsistent.
3. **Direct UI binding in multiplayer**: Use `USuqsProgressView` on clients, not `USuqsProgression`.
4. **Forgetting to save progression**: `USuqsProgression` state must be explicitly saved/loaded.
5. **Quest ID typos**: Quest IDs are strings - typos silently fail. Copy from definitions.
6. **Not handling quest dependencies**: Dependent quests need proper setup in quest definitions.

## 12. Build & Test Commands

### Build Editor Target
```bash
"<EnginePath>/Engine/Build/BatchFiles/Build.bat" SUQSEditor Win64 Development -Project="<ProjectPath>/YourProject.uproject" -WaitMutex -FromMSBuild
```

### Run SUQS Tests
```bash
RunUAT BuildCookRun -project=YourProject.uproject -noPMT -skipbuild -skiplog -clientconfig=Development -serverconfig=Development -runautomationtest -test=SUQSTest
```

## 13. Documentation

- [Quest Definitions](docs/Quests.md)
- [Quest Progression](docs/Progression.md)
- [Objectives](docs/Objectives.md)
- [Tasks](docs/Tasks.md)
- [Branching](docs/Branching.md)
- [Saving](docs/Saving.md)
- [Multiplayer](docs/Multiplayer.md)
- [Quest Schema](docs/questschema.json)

## 14. Human Review Required Before Implementing

- Changes to `USuqsProgression` public API
- Changes to `FSuqsQuest`, `FSuqsObjective`, `FSuqsTask` structures
- Changes to save data format
- Changes to quest JSON schema
- Changes to `ISuqsParameterProvider` interface