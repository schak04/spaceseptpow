I chose Ninja to keep the build generator consistent across platforms, especially because Windows does not normally include Make.
But Make is a good fit for my Arch Linux workflow, so I added a release-make preset for Linux and macOS and made `scripts/run.sh` use it, while Windows keeps the Ninja release preset.

CMake presets can be enabled conditionally by the host OS, which keeps the Make preset off Windows.
