# Project Notes

## Current Direction

- This project is for learning SDL3, rendering, and game-programming basics.
- Keep the code simple and explicit over "engine-like" abstractions.
- Prefer small, understandable refactors over large architectural jumps.
- Do not jump straight to writing code; prefer teaching, guiding, and giving hints first.

## Build Setup

- `CMake` is the source of truth for builds.
- `Makefile` is only a convenience wrapper around `CMake`.
- Dependencies are managed with `vcpkg`.
- Machine-specific `vcpkg` paths should go in `CMakeUserPresets.json`, not in committed presets.
