# SpaceSept Pow!

This project is a 2D top-down space shooter game that I'm writing in C++ using raylib. This is, in fact, my first raylib project.

- **Tech:** C++ and raylib
- **Platform:** Linux native first, macOS/Windows later. Steam deck support intended once the initial version is complete.
- **Arena:** One fixed screen, 1280x720
- **Players:** Single player

> [!NOTE]  
> _"But Saptaparno, why did you name it "SpaceSept Pow!"? That sounds silly and stupid."_
>
> Alright, hear me out...
>
> - "Space" refers to the spaceship and the game's setting.
> - "Sept" comes from my name (Saptaparno; nickname/shortened name: Sapto).
> - "Pow" is an onomatopoeic expression associated with shooting, since this is a space **shooter**.
>
> What can I say, well, I know the name is a bit wanky but it's unique and I like it.

---

## Controls

| Input             | Action |
| ----------------- | ------ |
| WASD / Arrow Keys | Move   |
| Space             | Shoot  |

---

## Core Loop

Move, shoot, collect energy, survive harder waves, die, restart.

---

## Rules

- **Movement:** 8 directions. Input gives acceleration, drag slows the ship, position updates from velocity and delta time. Must stay controllable, not floaty.
- **Shooting:** `Space` fires a projectile. No mouse aiming, so positioning is part of combat.
- **Energy (HP):** Fixed start value. Enemy contact drains it. Zero means Game Over.
- **Pickups:** Special enemies can drop energy. Flying into one restores part of the HP.
- **Waves:** Next wave starts when the current one is cleared. Difficulty rises through enemy count, speed, HP, spawn rate and special enemy frequency. No new mechanics.
- **Score:** Points per kill, special enemies give more. No win condition, survive as long as possible.
- **States:** Start, Playing, Game Over. Waves run inside Playing.

---

## Enemies

| Type    | Behaviour                                                            | HP     | Score  | Drops energy      |
| ------- | -------------------------------------------------------------------- | ------ | ------ | ----------------- |
| Basic   | Spawns at the edge, moves straight at the player, damages on contact | Low    | Low    | No                |
| Special | Faster or different movement, same contact damage                    | Higher | Higher | Yes, chance based |

No pathfinding, no ranged attacks in v1.

---

## UI

- **HUD:** energy, score, wave
- **Start screen:** title, controls, start prompt
- **Game Over screen:** final score, highest wave, restart prompt, random one liner flavour text

---

## Decisions (for v1)

- Inertia is the signature mechanic, so acceleration and drag are to be tuned first.
- Enemies inflict contact damage only in v1. Enemy projectiles are a stretch goal.
- One fixed arena, no scrolling, no camera work.

## Decisions yet to be made

I'll decide while coding and playtesting:

- Firing direction (fixed up, or the way the ship faces)
- Arena edge behaviour (whether to use clamping or wrapping)
- Starting energy, pickup amount, drop chance
- Wave scaling values and constraints

---

## Stretch Goals (only after the MVP works)

Sound effects, starfield background, simple boss, ranged-attacking enemy, persistent high score, gamepad support, and Steam Deck build.

---

## References

- https://www.raylib.com/cheatsheet/cheatsheet.html
- https://www.raylib.com/examples/core/loader.html?name=core_basic_window
- https://www.raylib.com/examples/core/loader.html?name=core_delta_time
- https://www.gafferongames.com/post/spring_physics

---

## Author

&copy; 2026 [Saptaparno Chakraborty](https://github.com/schak04).  
All Rights Reserved.

---
