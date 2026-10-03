# PSP Renderer and Platform Port Plan

## Purpose

This document turns the current feasibility notes into a staged implementation plan. It is not a claim that the engine has been ported.

## Architecture decision

Use the reVC engine as the reference for game systems, and treat rendering/platform support as separate work. The desktop librw backends use D3D/OpenGL-family APIs that the PSP does not provide. A PSP build needs a native renderer backend targeting the PSP Graphics Engine (GU), plus a PSP-specific platform layer. Do not attempt to make the desktop backend compile by merely changing compiler flags.

A useful architectural precedent is the GameCube reVC port, which added a platform-native GX renderer backend while retaining the engine lineage. This demonstrates the general shape of a console port, not code that can be reused directly on PSP: GX and PSP GU are different APIs, GPUs, memory systems, and toolchains.

## Implementation gates

### Gate 0 — Source and licensing
- Pin the exact reVC and librw revisions used for experiments.
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

Do not map Vita APIs (often marked PSP2) to PSP APIs by name. They are different systems and architectures.

**Pass condition:** small platform tests compile and run on PPSSPP, with input, timing, filesystem, and audio tested independently.

### Gate 3 — librw PSP GU backend
Implement a new backend rather than trying to emulate desktop OpenGL wholesale:
- device/framebuffer initialization and frame submission;
- texture upload, formats, swizzling, and texture cache policy;
- vertex/index buffer handling and transforms;
- depth test/write, blending, alpha test, culling, fog, and material state;
- camera/view transforms, clipping, and viewport/scissor;
- raster readback or screenshots for regression testing where feasible.

Maintain a renderer feature matrix. Any unsupported RenderWare state must be explicit and covered by a test scene.

**Pass condition:** deterministic renderer test scenes (flat geometry, textured geometry, alpha blending, depth, fog, and a simple animated model) render correctly in PPSSPP.

### Gate 4 — Engine boot and scene loading
- Resolve every PSP compile/link error without broad global compatibility macros.
- Add memory/streaming instrumentation before attempting the full city.
- Load a minimal user-supplied scene, then the player model and camera, before enabling missions or traffic.
- Log missing assets, allocation failures, and unsupported render states to a file.

**Pass condition:** an actual Vice City scene and player model load from user-supplied game data; the player can move and the camera follows.

### Gate 5 — Playability and performance
- Test on PPSSPP and, if available, real PSP hardware.
- Verify controls, pause/menu flow, save/load, audio, world streaming, and repeated area transitions.
- Profile memory and frame time; do not assume desktop settings fit the PSP.
- Publish screenshots/logs and list known limitations with each artifact.

**Pass condition:** reproducible gameplay testing, not just a successful compile or a blank/solid-colour screen.

## First concrete engineering tasks

1. Establish a reproducible upstream host build in CI and pin its source/submodule revisions.
2. Inspect the engine's platform abstractions and enumerate all direct OS, graphics, audio, threading, and filesystem calls.
3. Prototype the PSP GU backend in isolation with the renderer test scenes before connecting the entire game engine.
4. Keep the existing PSP smoke test as a separate toolchain check; never rename its EBOOT as a game build.

## Current state

Only the PSP SDK smoke test is known to compile in this repository. No reVC engine, GU renderer, game scene, or gameplay loop has been integrated here. Every gate above remains future work until its pass condition is demonstrated by a build and runtime evidence.
