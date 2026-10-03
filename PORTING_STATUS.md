# GTA Vice City PSP Port — Engineering Status

## Current state (2026-10-04)

This repository does **not** contain a bootable Vice City engine yet. The PSP EBOOT built by this repository is a renderer test, not the game.

## New reference inspected: user-provided GTA III PSP EBOOT

Static inspection of the uploaded `EBOOT.PBP` identifies the title as **Grand Theft Auto 3** (disc ID `UCJS10041`) and its embedded `DATA.PSP` as a MIPS ELF. Strings include `re3_psp`, PSP-specific RenderWare files under `librw-psp/src/psp/`, game-data loading, controller input, save-data paths, texture downsampling, streaming, and memory diagnostics.

This gives us a more relevant architectural direction than continuing with graphics-only probes: investigate a native PSP RenderWare/GU backend and the PSP platform layer as distinct components, then compare their interfaces with the Vice City engine source. The binary itself is only a reference. Static strings do not prove runtime behavior, feature completeness, or compatibility with Vice City. No executable code or proprietary assets have been copied into this repository.

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
6. The source repository, revision, and license terms behind the reference EBOOT's `librw-psp` implementation have not yet been identified or reviewed.

## Next technical milestone

Identify and review the source project corresponding to the reference's PSP RenderWare implementation, then compare its renderer/device and platform interfaces against the Vice City engine. Prefer learning from a real PSP-native renderer over expanding the standalone cube test indefinitely. Keep the renderer probe as a toolchain test, and do not claim gameplay until a real Vice City scene has been loaded and tested.

## Acceptance criteria for calling it playable

Only call a build playable after a real Vice City scene loads from legally supplied game data, controls work, and it has been tested in PPSSPP with logs/screenshots. A successful compile or blank-screen boot is not sufficient.

## Engine source feasibility references

- [reVC Miami branch](https://github.com/mrxenginner/reVC/tree/miami): host-supported source candidate; PSP-specific renderer and platform support remain to be implemented.
- [re3fork/re3-miami-vita](https://github.com/re3fork/re3-miami-vita): targets Vita ARM, not PSP MIPS Allegrex.
