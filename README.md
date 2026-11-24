# SpaceSept Pow!

This project is a 2D top-down space shooter game that I'm writing in C++ using raylib.

- **Platforms:** Linux (developed on Arch Linux, with the Steam Deck as a target), Windows, and macOS.
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
> I know the name is a bit wanky, but it feels unique and I like it (...I also couldn't think of a better name and I'll rename it if I happen to.).

---

## Gameplay and Design

**Core Loop:** Move, shoot, collect energy, survive harder waves, die, restart.

### Controls

| Keyboard          | Gamepad / Steam Deck | Action                 |
| ----------------- | -------------------- | ---------------------- |
| WASD / Arrow Keys | Left stick / D-pad   | Move                   |
| Space             | A                    | Shoot                  |
| Enter             | Tap Start / Menu     | Start or restart       |
| Escape            | Tap Start / Menu     | Pause or resume        |
| Hold Escape       | Hold Start / Menu    | Quit after 1.5 seconds |
| C                 | Y                    | Open or close credits  |

### Playing the game

Download the package for your platform from the repository's Releases page and keep its contents together when extracting it.

- **Linux:** Extract the `.tar.gz` package and launch `spaceseptpow` from the extracted folder.
- **Steam Deck:** Switch to Desktop Mode, download and extract the Linux `.tar.gz` package, then add `spaceseptpow` as a non-Steam game in Steam. Return to Game Mode to launch it. The built-in controller layout supports movement, shooting, starting, restarting, credits, and pause.
- **Windows:** Extract the ZIP file and launch `spaceseptpow.exe`.
- **macOS:** Open the DMG and drag `spaceseptpow.app` to Applications. The app is not signed or notarised yet, so macOS may block its first launch. Control-click the app and choose **Open** to allow it.

### Rules and Mechanics

- **Movement:** 8 directions with inertia as the primary mechanic. Input provides acceleration, drag dampens speed, and position updates from velocity and delta time.
- **Shooting:** Fires straight up with `Space`. No rotation or mouse aiming: ship positioning dictates hits.
- **Health (HP):** Starts at 500 HP. Enemy contact deals 50 damage; reaching zero triggers Game Over.
- **Pickups:** Special enemies have a 50% chance to drop an energy pickup when destroyed by a projectile. Player-enemy collision does not cause drops. Collecting a pickup restores 100 HP, up to the 500 HP maximum. Uncollected pickups expire after two seconds.
- **Waves:** Wave 1 requires 12 regular and 3 special enemy defeats. Every fifth wave is a boss fight and does not spawn regular or special enemies. Enemies that leave the arena are replaced and do not count as defeats. Enemy contact also replaces the enemy without counting as a defeat, but deals player damage. The wave clears only after all required defeats are made and no enemies remain active. Gameplay pauses for a four-second warning before the next wave.
- **Wave scaling:** Scaling is deterministic and based on the wave number. Every two waves, regular enemy count increases by 2 (maximum 24), enemy speed increases by 5% (maximum 40% above base), and spawn intervals decrease by 0.05 seconds for regular enemies and 0.1 seconds for special enemies (floors of 0.5 and 1.5 seconds). Every four waves, special enemy count increases by 1 (maximum 6), and regular and special HP increase by 25 and 50 (maximum 200 and 400 HP, twice base). The caps are reached at wave 13 for regular and special enemy counts, wave 17 for HP and speed, and wave 21 for spawn intervals. Values remain at their caps thereafter.
- **Score:** Destroying a regular enemy awards 5 points, a special enemy awards 10 points, and a boss awards 100 points. No win condition: survive as long as possible.
- **States:** Start, Playing, and Game Over.

### Enemies

| Type    | Behaviour                                                                  | HP       | Score | Drops Energy     |
| ------- | -------------------------------------------------------------------------- | -------- | ----- | ---------------- |
| Regular | Spawns at arena edge, moves vertically straight, deals contact damage      | 100 base | 5     | No               |
| Special | Similar to regular with abrupt random zig-zag motion, deals contact damage | 200 base | 10    | Yes (50% chance) |
| Boss    | Large, stationary ship that fires aimed projectiles                        | 4000     | 100   | No               |

### UI

- **HUD:** HP bar, score, wave counter, and next-wave warning with a countdown during the pause. The warning appears below the wave-cleared message.
- **Pause and quit:** Press `Escape` (keyboard) or tap `Start / Menu` (gamepad) to pause or resume gameplay. Hold either for 1.5 seconds to quit; a message appears at the top right while held. The wave countdown also pauses.
- **Start screen:** Title, controls, and start prompt
- **Game Over screen:** Final score and restart prompt

---

## Roadmap and Tuning

### Playtest Tuning

Parameters to dial in during playtesting:

- [x] Starting energy, pickup amounts, and drop chance
- [x] Wave scaling values and constraints

### Stretch Goals (Post-MVP)

- [ ] More interesting and engaging boss fights
- [ ] Persistent high score

---

## Building and packaging

> These instructions are only for experimentation/exploration. Contributions are not being sought, as this is a personal hobby project of mine, purely for learning and enjoyment.

<details>
<summary>View Instructions</summary>

Build packages on the platform they target. Install CMake 3.25 or later, Git, and a C++17 compiler. Linux and macOS use Make; Windows uses Ninja. The first CMake configure downloads raylib 5.5, so it needs network access.

On Arch Linux, install the compiler, build tools, and Linux development libraries with:

```sh
sudo pacman -S --needed base-devel cmake git alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
```

Windows builds use Visual Studio 2022 Build Tools with the C++ workload and Ninja, run from Developer PowerShell. macOS builds use the Xcode Command Line Tools.

On Linux and macOS, configure, build, and package with Make:

```sh
cmake --preset release-make
cmake --build --preset release-make
cmake --build --preset release-make --target package
```

On Windows, use the Ninja preset:

```sh
cmake --preset release
cmake --build --preset release
cmake --build --preset release --target package
```

Packages are written to the matching preset directory under `build`. CMake produces a `.tar.gz` archive on Linux, a ZIP on Windows, and a DMG containing a macOS app bundle on macOS. Each package contains the executable, its assets, `PLAYER_GUIDE.txt`, and this README with full asset credits. Use the native compiler architecture for the target machine; the Steam Deck package should be built for x86_64 Linux.

</details>

---

## Credits and References

### Assets

- **Spaceships:** [FREE Spaceships Vol. 1](https://gisha.itch.io/spaceships-asset-pack) by [gishadev](https://itch.io/profile/gisha), licensed under [CC0 1.0 Universal](https://creativecommons.org/publicdomain/zero/1.0/).
- **Background:** [Space Parallax Background](https://opengameart.org/content/space-parallax-background)
- **Sounds:**
  - [Theme](https://freesound.org/people/PhalanxKing/sounds/848138/)
  - [Regular Wave](https://freesound.org/people/TERMINEX/sounds/826075/)
  - [Shooting](https://freesound.org/people/qubodup/sounds/162404/)
  - [Boss Fight](https://freesound.org/people/Sirkoto51/sounds/443128/)
  - [Wave Cleared](https://freesound.org/people/BloodPixelHero/sounds/572624/)
  - [Game Over](https://freesound.org/people/AdamWeeden/sounds/157218/)

### References

- https://www.raylib.com/cheatsheet/cheatsheet.html
- https://www.raylib.com/examples/core/loader.html?name=core_basic_window
- https://www.raylib.com/examples/core/loader.html?name=core_delta_time
- https://www.gafferongames.com/post/spring_physics
- https://mathworld.wolfram.com/ExponentialDecay.html

### Tools

**Aseprite** and **ImageMagick** to analyse and inspect sprites.

---

## Author

&copy; 2026 [Saptaparno Chakraborty](https://github.com/schak04).  
All Rights Reserved.

---
