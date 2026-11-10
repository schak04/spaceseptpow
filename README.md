# SpaceSept Pow!

This project is a 2D top-down space shooter game that I'm writing in C++ using raylib. This is, in fact, my first raylib project.

- **Platform:** Linux native first, macOS/Windows later. Steam deck support intended once the initial version is complete.
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

| Input             | Action |
| ----------------- | ------ |
| WASD / Arrow Keys | Move   |
| Space             | Shoot  |

### Rules and Mechanics

- **Movement:** 8 directions with inertia as the primary mechanic. Input provides acceleration, drag dampens speed, and position updates from velocity and delta time. Must feel responsive, not floaty.
- **Shooting:** Fires straight up with `Space`. No rotation or mouse aiming: ship positioning dictates hits.
- **Energy (HP):** Fixed starting pool. Enemy contact drains it; reaching zero triggers Game Over.
- **Pickups:** Special enemies can drop energy pickups, but only through projectile-inflicted damage (not player-enemy collision). Flying into one restores a portion of HP. They expire in 2 seconds, so the player must be quick at picking them up.
- **Waves:** Wave 1 requires 12 regular and 3 special enemy defeats. Enemies that leave the arena or hit the player are replaced and do not count as defeats, so only destroying the full wave clears it. Clearing a wave pauses gameplay for a four-second warning before the next wave. Difficulty increases deterministically with these caps: every two waves add 2 regular enemies (up to 24), increase enemy speed by 5% (up to 40%), and shorten spawn intervals by 0.05 seconds for regular enemies and 0.1 seconds for special enemies (floors of 0.5 and 1.5 seconds); every four waves add 1 special enemy (up to 6) and increase regular and special HP by 25 and 50 respectively (up to double their base HP). The values stop increasing once their caps are reached.
- **Score:** Points awarded per kill, with higher payouts for special enemies. No win condition: survive as long as possible.
- **States:** Start, Playing, and Game Over. Waves run strictly inside Playing.

### Enemies

| Type    | Behaviour                                                             | HP     | Score  | Drops Energy       |
| ------- | --------------------------------------------------------------------- | ------ | ------ | ------------------ |
| Regular | Spawns at arena edge, moves vertically straight, deals contact damage | Low    | Low    | No                 |
| Special | Similar to regular with random zig-zag motion, deals contact damage   | Higher | Higher | Yes (chance based) |

Enemies deal contact damage only in v1 (no pathfinding or enemy projectiles).

### UI

- **HUD:** Energy, score, wave counter, and next-wave warning with a countdown during the pause
- **Start screen:** Title, controls, and start prompt
- **Game Over screen:** Final score, highest wave, restart prompt, and random one-liner flavour text

---

## Roadmap and Tuning

### Playtest Tuning

Parameters to dial in during playtesting:

- [x] Starting energy, pickup amounts, and drop chance
- [ ] Wave scaling values and constraints

### Stretch Goals (Post-MVP)

Sound effects, starfield background, simple boss, ranged-attacking enemy, persistent high score, gamepad support, and a Steam Deck build.

---

## Credits and References

### Assets and Credits

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
