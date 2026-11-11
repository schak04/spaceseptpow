# SpaceSept Pow!

This project is a 2D top-down space shooter game that I'm writing in C++ using raylib.

- **Platform:** Linux native. macOS, Windows, and Steam deck support is planned once the initial version is complete.
- **Arena:** One fixed screen, 1280x720
- **Players:** Single player

> [!NOTE]
> _"But Saptaparno, why name it **SpaceSept Pow!**? That sounds silly."_
>
> Alright, hear me out...
>
> - "Space" refers to the spaceship and the game's setting.
> - "Sept" comes from my name (Saptaparno; nickname/shortened name: Sapto).
> - "Pow" is an onomatopoeic expression associated with shooting, since this is a space **shooter**.
>
> I know the name is a bit wanky, but it is unique and I like it.

---

## Gameplay and Design

**Core Loop:** Move, shoot, collect energy, survive harder waves, die, restart.

### Controls

| Input             | Action          |
| ----------------- | --------------- |
| WASD / Arrow Keys | Move            |
| Space             | Shoot           |
| Escape            | Pause or resume |
| Hold Escape       | Quit the game   |

### Rules and Mechanics

- **Movement:** 8 directions with inertia as the primary mechanic. Input provides acceleration, drag dampens speed, and position updates from velocity and delta time. Must feel responsive, not floaty.
- **Shooting:** Fires straight up with `Space`. No rotation or mouse aiming: ship positioning dictates hits.
- **Energy (HP):** Starts at 500 HP. Enemy contact deals 50 damage; reaching zero triggers Game Over.
- **Pickups:** Special enemies have a 50% chance to drop an energy pickup when destroyed by a projectile. Player-enemy collision does not cause drops. Collecting a pickup restores 100 HP, up to the 500 HP maximum. Uncollected pickups expire after two seconds.
- **Waves:** Wave 1 requires 12 regular and 3 special enemy defeats. Enemies that leave the arena are replaced and do not count as defeats. Enemy contact also replaces the enemy without counting as a defeat, but deals player damage. The wave clears only after all required defeats are made and no enemies remain active. Gameplay pauses for a four-second warning before the next wave.
- **Wave scaling:** Scaling is deterministic and based on the wave number. Every two waves, regular enemy count increases by 2 (maximum 24), enemy speed increases by 5% (maximum 40% above base), and spawn intervals decrease by 0.05 seconds for regular enemies and 0.1 seconds for special enemies (floors of 0.5 and 1.5 seconds). Every four waves, special enemy count increases by 1 (maximum 6), and regular and special HP increase by 25 and 50 (maximum 200 and 400 HP, twice base). The caps are reached at wave 13 for regular and special enemy counts, wave 17 for HP and speed, and wave 21 for spawn intervals. Values remain at their caps thereafter.
- **Score:** Destroying a regular enemy awards 5 points; destroying a special enemy awards 10 points. No win condition: survive as long as possible.
- **States:** Start, Playing, and Game Over. Waves run strictly inside Playing.

### Enemies

| Type    | Behaviour                                                                  | HP       | Score | Drops Energy     |
| ------- | -------------------------------------------------------------------------- | -------- | ----- | ---------------- |
| Regular | Spawns at arena edge, moves vertically straight, deals contact damage      | 100 base | 5     | No               |
| Special | Similar to regular with abrupt random zig-zag motion, deals contact damage | 200 base | 10    | Yes (50% chance) |

Regular enemies have 100 base HP and special enemies have 200 base HP. Both take 50 damage per projectile hit. Their HP bars appear for one second after taking damage. Enemies deal contact damage only in v1 (no pathfinding or enemy projectiles).

### UI

- **HUD:** HP bar, score, wave counter, and next-wave warning with a countdown during the pause. The warning appears below the wave-cleared message.
- **Pause and quit:** Press `Escape` to pause or resume gameplay. Holding `Escape` for 1.5 seconds quits; a message appears at the top right while the key is held. The wave countdown also pauses.
- **Start screen:** Title, controls, and start prompt
- **Game Over screen:** Final score and restart prompt. Highest wave and flavour text are not currently displayed.

---

## Roadmap and Tuning

### Playtest Tuning

Parameters to dial in during playtesting:

- [x] Starting energy, pickup amounts, and drop chance
- [x] Wave scaling values and constraints

### Stretch Goals (Post-MVP)

Sound effects, starfield background, simple boss, ranged-attacking enemy, persistent high score, gamepad support, and a Steam Deck build.

---

## Credits and References

### Assets

- **Spaceships:** [FREE Spaceships Vol. 1](https://gisha.itch.io/spaceships-asset-pack) by [gishadev](https://itch.io/profile/gisha), licensed under [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).

### References

- https://www.raylib.com/cheatsheet/cheatsheet.html
- https://www.raylib.com/examples/core/loader.html?name=core_basic_window
- https://www.raylib.com/examples/core/loader.html?name=core_delta_time
- https://www.gafferongames.com/post/spring_physics
- https://mathworld.wolfram.com/ExponentialDecay.html

---

## Author

&copy; 2026 [Saptaparno Chakraborty](https://github.com/schak04).  
All Rights Reserved.

---
