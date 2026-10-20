# Simple Platform: boss AI with State Tree (Unreal Engine 5.8)

> Video: https://youtu.be/Bf3suteu0xc

A small 3D platformer built for the *Game Engine Programming* course (Master in Computer Game Development, University of Verona). My focus is the **enemy AI**: a boss driven by an Unreal **State Tree**, with sight-based perception, target memory and an investigation behavior, plus simple patrolling enemies written in C++.

## What the AI does

- **The boss** patrols/wanders until it sees the player, then chases and attacks. After each attack it becomes **vulnerable** for a short window (a design choice implemented at AI level).
- **It remembers**: When the player breaks line of sight, the boss does not keep chasing them blindly: it goes to the **last known location** and then returns to its idle behavior, or attacks again if it sees the player on the way.
- **Basic enemies** patrol between designer-defined points (C++).

## Architecture (perception → decision → action)

**Perception** lives in the AI controller (`AIC_Enemy`, Blueprint):
- `On Target Perception Updated` → cast to the player character → branch on `Successfully Sensed`
- *Sensed:* invalidate the forget timer, store the target, send the State Tree event `AI.Event.Attacking`
- *Lost:* store `LastKnownLocation`, start a `MemoryDuration` timer; when it fires, send `AI.Event.Lost`

The controller turns raw stimuli into **high-level events (gameplay tags)**, and the State Tree only reacts to those. Perception and decision stay decoupled.

**Decision** is the State Tree `ST_Enemy`:

```
Root
├─ Idle (Try Select Children In Order)       -- AI.Event.Attacking --> Attacking
│   ├─ Patrolling (enter condition: Is Object Valid)
│   └─ Wandering (WanderAroundLocation + Wait 2.0 ±0.5)
├─ Investigate: Move To Last Known Location -> Idle
│                                            -- AI.Event.Attacking --> Attacking
└─ Attacking (enter condition: Is Object Valid)   -- AI.Event.Lost --> Investigate
    ├─ MoveToAttackRange
    ├─ Attack
    ├─ Vulnerable (+ Delay 1.0)
    └─ Wait (0.5 ±0.1) -> Attacking
```

**Action** is performed by State Tree tasks (`STT Wander`, `STT Attack`, `STT Vulnerable`) and the built-in `Move To` task on the navmesh.

## Design choices and discarded alternatives

- **State Tree instead of Behavior Tree**: The boss has well-defined phases (idle, investigate, combat, vulnerable), which map naturally to states with explicit, event-driven transitions. State Tree is lighter and integrates cleanly with gameplay-tag events. A Behavior Tree with decorators would be more flexible for highly reactive, composable behaviors; I chose the model that fits this boss
- **Hierarchy to avoid transition explosion**: Idle and Attacking are parent states: transitions are declared once on the parent, not on every child
- **Race condition handled**: The forget timer is invalidated when the player is seen again, so a late `Lost` cannot override a fresh sighting
- **Attack range for designers**: `Attack Range` is an exposed variable so the designers could easily modify it
- **`Allow Partial Path` enabled**: If the player stands on an unreachable platform, the boss moves to the closest reachable point instead of failing the task and standing still. Trade-off: it can stop at a spot from which it cannot hit the player
- **Timing variance**: Waits and delays use random deviation so the behavior does not look mechanical
- **Debug flag**: `STT Print` tasks run only when the tree condition `DebugMode` is on

### Blueprint vs C++ (what is mine)

- **Boss: entirely Blueprint** (perception in `AIC_Enemy`, tasks as Blueprints). For a single boss the cost is negligible and iteration is faster. With many NPCs I would move the tasks to C++ for performance and stronger typing
- **Basic enemies: patrol in C++, written by me** (`AEnemyBase`). Event-driven: no Tick, just a timer and the `ReceiveMoveCompleted` callback, so it costs nothing while the enemy waits or walks. Patrol points are stored in local space and converted to world space once at start (the route follows the actor); movement is ping-pong between points, and a failed move reverses direction
- **Not mine:** the C++ under `Source/GEP_Platform/Variant_Combat/AI/` (`CombatAIController`, `CombatStateTreeUtility`, `EnvQueryContext_*`) comes from the Unreal Combat template

## Known limitations

- Perception is **sight only** (scope choice)
- `AI.Event.Lost` can interrupt an attack in progress
- The boss has a **single attack**, so its cycle is predictable
- After losing the player, the boss stays in Attacking for `MemoryDuration` before investigating
- `Allow Partial Path` can leave the boss out of attack range (see above)

## Roadmap

1. **Multiple attacks chosen with Considerations** (distance to player, cooldown, last attack used) instead of a fixed cycle
2. `AI.Event.LowHealth` event to trigger a **second phase**
3. Evaluators for distance/health, hearing sense, team ID instead of the direct cast
4. Investigate as a parent state with `MoveToLastKnown` + `LookAround`

## How to run

- Download the latest build from the Releases page, extract the zip and run GEP_Platform.exe (Windows only). If SmartScreen appears: More info → Run anyway

## Controls
- WASD: Move
- Spacebar: Jump
- Esc / P: Pause
- Mouse: look around

## Audio licence
All the audio are from Freesound, with Attribution 3.0 licence (https://creativecommons.org/licenses/by/3.0/):
- Coin sound by Prof.Mudkip (https://freesound.org/people/Prof.Mudkip/)