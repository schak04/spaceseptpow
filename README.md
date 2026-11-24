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
> I know the name is a bit silly, but it feels unique and I like it.

---

## Gameplay and Design

**Core Loop:** Move, shoot, collect energy, survive harder waves, die, restart.

### Controls

Gamepad bindings use raylib's standard mapping for PC gamepads and Steam Deck.

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

- **Movement:** Move in 8 directions with inertia. Keyboard input uses `WASD` or the arrow keys; gamepad input uses the left stick or D-pad. Acceleration moves the player spaceship, while drag slows it down.
- **Shooting:** Press `Space` on a keyboard or `A` on a gamepad to fire a shot straight up. Shots deal 50 damage. There is no aiming; ship position determines where shots land.
- **Health (HP):** Starts at 500 HP. Contact with regular or special enemies, and hits from boss projectiles, deal 50 damage. Reaching zero triggers Game Over.
- **Pickups:** Destroying a special enemy with a player shot has a 50% chance to drop an energy pickup. Contact damage does not cause drops. Collecting one restores 100 HP, up to 500 HP; each pickup expires after 2 seconds.
- **Waves:** Wave 1 requires defeating 12 regular and 3 special enemies. Every fifth wave is a boss fight with no regular or special enemies. Enemies that leave the arena are replaced and do not count as defeats. Contacting regular or special enemies damages the player and replaces the enemy without counting as a defeat. A wave clears after all required defeats and no enemies remain active. A four-second intermission follows; its countdown pauses with gameplay.
- **Boss fights:** The boss has 4000 HP and fires 4 projectiles in a spread aimed around the player's current position every 1.5 seconds. Each hit deals 50 damage.
- **Wave scaling:** Scaling is deterministic. Every two waves, the regular enemy defeat target increases by 2 (maximum 24), enemy speeds increase by 5% (maximum 40% above base), and regular/special spawn intervals decrease by 0.05/0.1 seconds (floors of 0.5/1.5 seconds). Every four waves, the special enemy defeat target increases by 1 (maximum 6), and regular/special HP increase by 25/50 (maximum 200/400 HP). Counts cap at wave 13, HP and speed at wave 17, and spawn intervals at wave 21; values stay the same from then on.
- **Score:** Destroying a regular enemy awards 5 points, a special enemy 10, and a boss 100. There is no win condition; survive as long as possible and have fun shooting the bad guy ships.
- **States:** Start, Playing, Game Over, and Credits. Press `C` or gamepad `Y` on the start screen to open Credits.

### Enemies

| Type    | Behaviour                                                      | HP       | Score | Drops Energy     |
| ------- | -------------------------------------------------------------- | -------- | ----- | ---------------- |
| Regular | Spawns at arena edge, moves vertically, deals contact damage   | 100 base | 5     | No               |
| Special | Moves in abrupt random zig-zags and deals contact damage       | 200 base | 10    | Yes (50% chance) |
| Boss    | Stationary; fires a four-projectile spread aimed at the player | 4000     | 100   | No               |

### UI

- **HUD:** HP bar, score, wave counter, and next-wave warning with a countdown during the pause. The warning appears below the wave-cleared message.
- **Pause and quit:** Press `Escape` (keyboard) or tap `Start / Menu` (gamepad) to pause or resume gameplay. Hold either for 1.5 seconds to quit; a message appears at the top right while held. The wave countdown also pauses.
- **Start screen:** Title, start prompt, and a credits prompt. `Enter` or gamepad `Start / Menu` starts the game; `C` or gamepad `Y` opens Credits.
- **Credits screen:** Developer and asset credits. Return with `C`, `Y`, `Enter`, or `Start / Menu`.
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

> These instructions are for experimentation and exploration only. Contributions are not being sought, as this is a personal hobby project made purely for learning and enjoyment.

<details>
<summary>View Instructions</summary>

### Prerequisites

- CMake 3.25 or later
- Git
- A C++17-compatible compiler
- Make (Linux and macOS) or Ninja (Windows)

The first CMake configuration downloads raylib 5.5, so an internet connection is required.

### Platform-specific setup

**Linux**

On Linux systems, install the compiler, build tools, and required development libraries (if not already installed, obviously):

For Arch (btw):

```sh
sudo pacman -S --needed base-devel cmake git alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
```

For other distros, use the equivalent package manager to install the corresponding development packages (obviously).

**Windows**

Install Visual Studio 2022 Build Tools with the C++ workload and Ninja. Run the build commands from Developer PowerShell.

**macOS**

Install the Xcode Command Line Tools.

### Building and packaging

**Linux and macOS**

Configure, build, and package using the Make preset:

```sh
cmake --preset release-make
cmake --build --preset release-make
cmake --build --preset release-make --target package
```

**Windows**

Configure, build, and package using the Ninja preset:

```sh
cmake --preset release
cmake --build --preset release
cmake --build --preset release --target package
```

### Package output

Packages are written to the corresponding preset directory under `build`. CMake produces:

- **Linux:** `.tar.gz` archive
- **Windows:** ZIP archive
- **macOS:** DMG containing a macOS app bundle

Each package includes the executable, its assets, `PLAYER_GUIDE.txt`, and this README with full asset credits.

Use the native compiler architecture for the target machine. The Steam Deck package should be built for `x86_64 Linux`.

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
