# SUQS

Data-driven quest system for Unreal Engine.

## Capabilities

| Capability | Description |
|------------|-------------|
| **Data-Driven Quests** | Defined in data tables or JSON with schema validation |
| **Quest Branching** | Predefined branches within quests, quest dependencies |
| **Task Progression** | Tasks track progress with target numbers and time limits |
| **Objective Groups** | Sequential, flexible, or "N of M" completion |
| **Event System** | Callbacks and query API for quest state changes |
| **Multiplayer Support** | `USuqsGameStateComponent` for network replication |
| **Save Integration** | Works with SPUD for quest persistence |

## Key Classes

| Class | Purpose |
|-------|---------|
| `USuqsProgression` | Main class tracking quest state |
| `USuqsQuest` | Quest definition data asset |
| `USuqsQuestState` | Runtime quest state |
| `USuqsObjectiveState` | Runtime objective state |
| `USuqsTaskState` | Runtime task state |
| `USuqsWaypointComponent` | Waypoint for quest navigation |
| `USuqsGameStateComponent` | Multiplayer replication component |
| `USuqsProgressView` | Read-only view for UI in multiplayer |

## Key Structs & Interfaces

| Name | Type | Purpose |
|------|------|---------|
| `FSuqsQuest` | Struct | Quest definition |
| `FSuqsObjective` | Struct | Objective definition |
| `FSuqsTask` | Struct | Task definition |
| `FSuqsProgressionEventDetails` | Struct | Event details for callbacks |
| `ESuqsQuestStatus` | Enum | Inactive, Active, Completed, Failed |
| `ESuqsTaskStatus` | Enum | Task status values |
| `ISuqsParameterProvider` | Interface | `GetNamedFormatParameter()` for text substitution |

## Common Pitfalls

- Not calling `InitWithQuestDataTables()` before use
- Multiple `USuqsProgression` instances per player — breaks state
- Direct UI binding in multiplayer — use `USuqsProgressView` instead
- Forgetting to save/load `USuqsProgression` state
- Quest ID typos — strings fail silently
- Not handling quest dependencies properly

See `.agents/patterns.md` for implementation patterns.

## Integration Points

- Integrates with: SPUD (persistence), FlowGraph (SUQSFlow)
- See `.agents/plugin-integration.md` for cross-plugin dependency matrix

## Human Review Required

- Changes to `USuqsProgression` public API
- Changes to `FSuqsQuest`, `FSuqsObjective`, `FSuqsTask` structures
- Changes to quest JSON schema or save data format
- See `.agents/human-review-checklist.md` for full list