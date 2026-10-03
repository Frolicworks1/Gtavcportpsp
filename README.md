# GTA Vice City PSP Port (work in progress)

**Status: not playable. This repository does not currently boot GTA Vice City.**

The green/red screen build is only a PSP graphics/controller diagnostic: START toggles the screen colour. It contains no Vice City engine, world, player, missions, or game assets. Do not use that EBOOT expecting the game to start.

## Current verified progress

- A PSP SDK smoke-test executable compiles in GitHub Actions.
- The smoke test can initialize the display and detect START input (colour changes).
- This verifies only a small part of the PSP toolchain and input/display path. It does **not** verify a game port.

## What is still required before gameplay is possible

1. Integrate a complete, license-reviewed Vice City engine source tree.
2. Replace the Vita-specific platform layer with PSP SDK implementations (startup, filesystem, controller, timing, audio, and logging).
3. Port or replace the renderer with a PSP-compatible renderer using the PSP GU.
4. Resolve dependencies and build the engine with the PSP MIPS Allegrex toolchain.
5. Adapt streaming, memory usage, textures, audio, and controls to PSP limits.
6. Load a scene using game data supplied by the user who owns the game, then test movement, camera, missions, saving, and stability in PPSSPP and ideally real hardware.

The available candidate engine source at [re3fork/re3-miami-vita](https://github.com/re3fork/re3-miami-vita) targets PlayStation Vita, not PSP, so its Vita executable cannot run on PSP. Source licensing and third-party dependency terms must be reviewed before incorporating it.

## Build the diagnostic only

The workflow [Build PSP platform foundation smoke test](https://github.com/Frolicworks1/Gtavcportpsp/actions/workflows/psp-platform-smoke.yml) builds the diagnostic artifact named `vc-psp-platform-foundation-not-game`.

## Playable-build acceptance criteria

We will only label a build playable when it starts the actual Vice City engine, loads a real in-game scene from user-provided game data, accepts gameplay controls, and has been tested in PPSSPP with evidence. A successful compile or a colour-changing screen is not enough.

See [PORTING_STATUS.md](PORTING_STATUS.md) for the engineering blockers and milestones.
