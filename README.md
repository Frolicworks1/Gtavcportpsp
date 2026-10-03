# GTA Vice City PSP Port (work in progress)

**Status: not playable. This repository does not currently boot GTA Vice City.**

The current EBOOT is a PSP GU renderer probe. The latest source adds a procedurally generated 64×64 checkerboard texture mapped onto a rotating 3D cube, alongside depth testing, GUM transforms, texture filtering/wrapping, and LEFT/RIGHT rotation controls. It contains no Vice City engine, world, player, missions, or game assets. Do not use that EBOOT expecting the game to start.

## Current verified progress

- A PSP SDK executable has compiled successfully in earlier GitHub Actions smoke-test runs.
- The upstream reVC Miami branch successfully configured and built on Linux in [host baseline run](https://github.com/Frolicworks1/Gtavcportpsp/actions/runs/37145378347). This confirms the upstream project builds for its host platform, **not PSP**.
- The updated texture-probe source triggers the PSP SDK workflow; check its run before treating the latest code as build-verified.
- The texture probe is still a graphics/platform test, **not a game port**.

## What is still required before gameplay is possible

1. Pin and integrate a complete, license-reviewed Vice City engine source tree.
2. Implement PSP SDK versions of startup, filesystem, controller, timing, audio, and logging interfaces.
3. Port or replace the renderer with a complete PSP GU backend for the engine's RenderWare/librw interfaces.
4. Resolve dependencies and build the engine with the PSP MIPS Allegrex toolchain.
5. Adapt streaming, memory usage, textures, audio, and controls to PSP limits.
6. Load a scene using game data supplied by the user who owns the game, then test movement, camera, missions, saving, and stability in PPSSPP and ideally real hardware.

The available candidate engine source at [re3fork/re3-miami-vita](https://github.com/re3fork/re3-miami-vita) targets PlayStation Vita, not PSP, so its Vita executable cannot run on PSP. The [reVC Miami branch](https://github.com/mrxenginner/reVC/tree/miami) builds on Linux but uses host-platform dependencies; a PSP platform layer and renderer port are still needed. Review source licensing and third-party dependency terms before incorporating it.

## Build the renderer probe only

The workflow [Build PSP platform foundation smoke test](https://github.com/Frolicworks1/Gtavcportpsp/actions/workflows/psp-platform-smoke.yml) builds the diagnostic artifact named `vc-psp-platform-foundation-not-game`.

## Playable-build acceptance criteria

We will only label a build playable when it starts the actual Vice City engine, loads a real in-game scene from user-provided game data, accepts gameplay controls, and has been tested in PPSSPP with evidence. A successful compile or a renderer test is not enough.

See [PORTING_STATUS.md](PORTING_STATUS.md) and [PSP_RENDERER_PORT_PLAN.md](PSP_RENDERER_PORT_PLAN.md) for the engineering blockers and milestones.
