# SUQS

Steve's Unreal Quest System: data-driven quests defined in DataTables. Third-party library (Ji-Rath fork of sinbad/SUQS, git submodule). **Prefer using it over modifying it.** Modules: `SUQS` (Runtime), `SUQSTest` (automation tests, `Automation RunTests SUQSTest`). Log category: `LogSUQS`.

## Usage in HorrorGame

- `Plugins/HorrorFeatures/Content/Systems/BP_HorrorGameState` owns a `USuqsGameStateComponent` (holds the `USuqsProgression`) and a quest DataTable variable.
- Quest state is saved via `USuqsFlowSaveObject` (SUQSFlow) inside the SPUD save — don't add another save path.
- `WBP_HorrorGameUI` displays quests through `FSuqsProgressView` / `USuqsProgressViewHelpers`.
- `Flow_MainStory` does not drive quests yet; SUQSFlow nodes are available for that.

## Key Types (`Source/SUQS/Public/`)

| Type | Purpose |
|---|---|
| `FSuqsQuest` / `FSuqsObjective` / `FSuqsTask` | Quest definition (DataTable row type `FSuqsQuest`) |
| `USuqsProgression` | All quest state; `InitWithQuestDataTables`, `AcceptQuest`, `ProgressTask`/`CompleteTask`, events, `SaveToData`/`LoadFromData` (`FSuqsSaveData`) |
| `USuqsQuestState` / `USuqsObjectiveState` / `USuqsTaskState` | Runtime state objects |
| `ESuqsQuestStatus` (Incomplete/Completed/Failed), `ESuqsTaskStatus` (NotStarted/InProgress/…) | Status enums |
| `USuqsGameStateComponent` | Owns + replicates progression via `FSuqsProgressView` |
| `FSuqsProgressView` + `USuqsProgressViewHelpers` | Read-only, replicated quest view for UI |
| `FSuqsProgressionEventDetails` | Payload for progression events |
| `ISuqsParameterProvider` | `GetQuestParameters(QuestID, TaskID, USuqsNamedFormatParams*)` for `{Param}` text substitution |
| `USuqsWaypointComponent` | Marks quest waypoints for a task |

## Common Pitfalls

- Call `InitWithQuestDataTables()` before using a progression
- Quest/task IDs are `FName`s — typos fail silently
- UI should read the progress view, not mutate `USuqsProgression` directly

## Human Review Required

- Any change to SUQS source (fork of a third-party library)
- Changes to `USuqsProgression` public API, `FSuqsQuest`/`FSuqsObjective`/`FSuqsTask`, or `FSuqsSaveData`
- See root `.agents/human-review-checklist.md`
