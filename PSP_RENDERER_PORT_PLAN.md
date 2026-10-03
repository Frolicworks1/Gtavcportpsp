# PSP Renderer and Platform Port Plan

## Purpose

This document turns the current feasibility notes into a staged implementation plan. It is not a claim that the engine has been ported.

## Architecture decision

Use the reVC engine as the reference for game systems, and treat rendering/platform support as separate work. The desktop librw backends use D3D/OpenGL-family APIs that the PSP does not provide. A PSP build needs a native renderer backend targeting the PSP Graphics Engine (GU), plus a PSP-specific platform layer. Do not attempt to make the desktop backend compile by merely changing compiler flags.

A useful architectural precedent is the GameCube reVC port, which added a platform-native GX renderer backend while retaining the engine lineage. The user-provided GTA III PSP EBOOT is a second, more directly relevant reference: static inspection shows a MIPS ELF executable with `re3_psp` and many `librw-psp/src/psp/psprender.cpp` / `pspdevice.cpp` strings. This is evidence of a PSP-specific RenderWare renderer/device implementation in that build, not proof that its binary or source can be dropped into Vice City unchanged. PSP GU and GameCube GX are different APIs, GPUs, memory systems, and toolchains.

## Reference EBOOT findings (static inspection only)

The supplied `EBOOT.PBP` has PSP PBP metadata identifying its title as **Grand Theft Auto 3**, disc ID `UCJS10041`, and a `DATA.PSP` ELF whose machine field identifies MIPS. Relevant strings point to:
- A PSP-specific renderer/device implementation under `librw-psp/src/psp/`.
- Texture and raster handling, geometry, camera, frame, skinning, animation, and pipeline code.
- PSP controller input and a PSP save-data path (`ms0:/PSP/SAVEDATA/GTA3`).
- Game-data loading paths such as `DATA\\GTA3.DAT` and `MODELS\\GTA3.IMG`.
- Streaming, texture downsampling, memory-allocation diagnostics, and game initialization logs.

These strings are useful architectural clues only. A binary string scan cannot establish which code paths execute, the completeness of features, runtime performance, or whether the same implementation will work with Vice City. No proprietary game assets or executable code should be copied into this repository. Treat this EBOOT as a reference supplied by the user, not as a redistributable dependency.

## Implementation gates

### Gate 0 — Source and licensing
- Pin the exact reVC and librw revisions used for experiments.
- Identify and review the source/license terms of any PSP renderer reference before incorporating source code.
- Keep upstream source as a separate, auditable dependency until licensing and dependency terms are reviewed.
- Do not commit proprietary game data. Runtime testing must use data supplied by the user from a legally obtained copy.

**Pass condition:** documented revisions, dependency/submodule list, and license review before source redistribution.

### Gate 1 — Build the unmodified engine for a host
- Build reVC and its librw dependency in CI on Linux using the upstream-supported configuration.
- Keep this as a separate host build, not as a PSP artifact.
- Record the build command and revision in CI logs.

**Pass condition:** host build completes reproducibly. This validates the source/dependency baseline only.

### Gate 2 — PSP platform boundary
Introduce a distinct PSP platform implementation for:
- startup, exit callbacks, and logging;
- monotonic timing and frame pacing;
- controller mapping to the engine's input abstraction;
- file paths and asset file access;
- audio output and streaming interfaces;
- memory allocation and low-memory diagnostics.

The GTA III reference suggests keeping file/streaming failures and allocation diagnostics visible from the beginning. Do not hard-code GTA III paths as Vice City paths; make data roots and expected filenames configurable. Do not map Vita APIs (often marked PSP2) to PSP APIs by name. They are different systems and architectures.

**Pass condition:** small platform tests compile and run on PPSSPP, with input, timing, filesystem, and audio tested independently.

### Gate 3 — librw PSP GU backend
Implement a new backend rather than trying to emulate desktop OpenGL wholesale. Use the reference binary's symbols/strings as a checklist for subsystems to investigate, not as proof of implementation details:
- device/framebuffer initialization and frame submission;
- texture upload, formats, swizzling, and texture cache policy;
- vertex/index buffer handling and transforms;
- depth test/write, blending, alpha test, culling, fog, and material state;
- camera/view transforms, clipping, and viewport/scissor;
- raster readback or screenshots for regression testing where feasible;
- geometry, animation/skinning, and render-pipeline integration once basic rasterization is stable.

Maintain a renderer feature matrix. Any unsupported RenderWare state must be explicit and covered by a test scene.

**Pass condition:** deterministic renderer test scenes (flat geometry, textured geometry, alpha blending, depth, fog, and a simple animated model) render correctly in PPSSPP.

### Gate 4 — Engine boot and scene loading
- Resolve every PSP compile/link error without broad global compatibility macros.
- Add memory/streaming instrumentation before attempting the full city.
- Load a minimal user-supplied scene, then the player model and camera, before enabling missions or traffic.
- Log missing assets, allocation failures, and unsupported render states to a file.
- Keep game-specific data paths separate from renderer code; do not assume GTA III's data layout exactly matches Vice City.

**Pass condition:** an actual Vice City scene and player model load from user-supplied game data; the player can move and the camera follows.

### Gate 5 — Playability and performance
- Test on PPSSPP and, if available, real PSP hardware.
- Verify controls, pause/menu flow, save/load, audio, world streaming, and repeated area transitions.
- Profile memory and frame time; do not assume desktop settings fit the PSP.
- Publish screenshots/logs and list known limitations with each artifact.

**Pass condition:** reproducible gameplay testing, not just a successful compile or a blank/solid-colour screen.

## First concrete engineering tasks

1. Establish a reproducible upstream host build in CI and pin its source/submodule revisions.
2. Identify the source repository corresponding to the PSP RenderWare implementation indicated by the reference binary, then review its license, revision, and dependencies before considering any code reuse.
3. Compare reVC's RenderWare/platform interfaces with that PSP implementation and write a concrete gap list; do not assume GTA III game code is interchangeable with Vice City.
4. Prototype the PSP GU backend in isolation with renderer test scenes before connecting the entire game engine.
5. Keep the existing PSP smoke test as a separate toolchain check; never rename its EBOOT as a game build.

## Current state

Only the PSP SDK smoke test is known to compile in this repository. The host reVC build succeeds on Linux, but no reVC engine, GU renderer, Vice City scene, or gameplay loop has been integrated here. The supplied GTA III EBOOT provides useful architecture clues, but has not been runtime-tested or used as source code. Every gate above remains future work until its pass condition is demonstrated by a build and runtime evidence.
