# GTA Vice City PSP Port (work in progress)

**Status: not playable. This repository does not currently boot GTA Vice City.**

The current EBOOT is a PSP GU renderer probe: it draws a rotating, six-colour 3D cube with depth testing and GUM transforms; LEFT/RIGHT change the rotation direction. It contains no Vice City engine, world, player, missions, or game assets. Do not use that EBOOT expecting the game to start.

## Current verified progress

- A PSP SDK executable has compiled successfully in GitHub Actions in earlier smoke-test runs.
- The current renderer probe exercises PSP GU triangle submission, projection/model transforms, depth buffer setup, double buffering, and controller input.
- The updated renderer-probe build is triggered by changes under `psp-platform-smoke/`; check Actions for its result before treating the latest source as build-verified.
- This is still a graphics/platform test, **not** a game port.

## What is still required before gameplay is possible

1. Integrate a complete, license-reviewed Vice City engine source tree.
2. Implement PSP SDK versions of startup, filesystem, controller, timing, audio, and logging interfaces.
3. Port or replace the renderer with a complete PSP GU backend for the engine's RenderWare/librw interfaces.
4. Resolve dependencies and build the engine with the PSP MIPS Allegrex toolchain.
5. Adapt streaming, memory usage, textures, audio, and controls to PSP limits.
6. Load a scene using game data supplied by the user who owns the game, then test movement, camera, missions, saving, and stability in PPSSPP and ideally real hardware.

The available candidate engine source at [re3fork/re3-miami-vita](https://github.com/re3fork/re3-miami-vita) targets PlayStation Vita, not PSP, so its Vita executable cannot run on PSP. Source licensing and third-party dependency terms must be reviewed before incorporating it.

## Build the renderer probe only

The workflow [Build PSP platform foundation smoke test](https://github.com/Frolicworks1/Gtavcportpsp/actions/workflows/psp-platform-smoke.yml) builds the diagnostic artifact named `vc-psp-platform-foundation-not-game`.

## Playable-build acceptance criteria

We will only label a build playable when it starts the actual Vice City engine, loads a real in-game scene from user-provided game data, accepts gameplay controls, and has been tested in PPSSPP with evidence. A successful compile or a renderer test is not enough.

See [PORTING_STATUS.md](PORTING_STATUS.md) and [PSP_RENDERER_PORT_PLAN.md](PSP_RENDERER_PORT_PLAN.md) for the engineering blockers and milestones.
