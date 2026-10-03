# GTA Vice City PSP Port — Engineering Status

## Current state (2026-10-04)

This repository does **not** contain a bootable Vice City engine yet. The PSP EBOOT is a renderer test, not the game.

## Verified build baselines

- The PSP SDK probe has compiled in earlier GitHub Actions runs.
- The upstream `mrxenginner/reVC` Miami branch successfully configured and built on Linux in [host baseline run](https://github.com/Frolicworks1/Gtavcportpsp/actions/runs/37145378347). That only verifies a host build; it does not mean the code builds for PSP.
- The latest PSP renderer source change should be verified through [the PSP smoke-test workflow](https://github.com/Frolicworks1/Gtavcportpsp/actions/workflows/psp-platform-smoke.yml).

## Renderer probe milestone (in progress)

The probe currently:
- Initializes PSP GU display, color buffer, and depth buffer.
- Sets up perspective projection and GUM model transforms.
- Submits a six-face cube as triangles with 3D vertices.
- Generates a 64×64 RGBA checkerboard texture in memory.
- Enables GU texturing, linear filtering, repeat wrapping, and texture modulation.
- Uses depth testing and double buffering.
- Reads LEFT/RIGHT controls to adjust rotation.

This is a focused test of texture upload/addressing and triangle rendering. It is **not** connected to Vice City engine code, and its visual output has not been verified in PPSSPP or on hardware yet.

## Immediate engineering blockers

1. There is no integrated, pinned Vice City engine source tree in this repository.
2. The engine's RenderWare/librw renderer has no PSP GU backend here.
3. PSP platform interfaces for audio, files, timing, startup, and streaming are not implemented.
4. The host reVC build is not a PSP MIPS/Allegrex build.
5. Actual game data must be supplied by the user; no copyrighted game assets are bundled.

## Next technical milestone

After the texture probe's PSP build is verified, the next renderer tests should cover alpha blending, depth behavior, and texture state changes. In parallel, engine integration needs a pinned source revision and a license/dependency review, then a deliberate PSP platform port. Do not simply copy a Vita executable or its ARM toolchain: PSP uses MIPS Allegrex and different system libraries.

## Acceptance criteria for calling it playable

Only call a build playable after a real Vice City scene loads from legally supplied game data, controls work, and it has been tested in PPSSPP with logs/screenshots. A successful compile or blank-screen boot is not sufficient.

## Engine source feasibility references

- [reVC Miami branch](https://github.com/mrxenginner/reVC/tree/miami): host-supported source candidate; PSP-specific renderer and platform support remain to be implemented.
- [re3fork/re3-miami-vita](https://github.com/re3fork/re3-miami-vita): targets Vita ARM, not PSP MIPS Allegrex.
