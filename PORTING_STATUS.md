# GTA Vice City PSP Port — Engineering Status

## Current state (2026-10-04)

This repository does **not** contain a bootable Vice City engine yet. The current WIP executable is a PSP GU renderer probe, not the game. Do not label its EBOOT as a GTA Vice City build.

An upstream source audit of `re3fork/re3-miami-vita` found that it targets **PS Vita (ARM)** and links Vita-specific libraries. PSP is a different platform (MIPS Allegrex); a Vita EBOOT/VPK cannot be used as a PSP executable. Licensing and dependency terms must be reviewed before incorporating source.

## Porting work required

1. **Bring engine source in reproducibly** — pin an upstream commit and record its license/submodule dependencies; keep upstream separate from PSP-specific changes.
2. **Create a PSP platform layer** — PSP SDK startup, callbacks/exit handling, controller input, audio output, filesystem paths, timing, and logging.
3. **Port the renderer dependency** — implement a PSP GU backend for the engine's RenderWare/librw interfaces; a standalone GU cube test is only a small first step.
4. **Port the build system** — use the PSP MIPS toolchain and PSP SDK libraries, not `arm-vita-eabi` or Vita system stubs.
5. **Adapt game systems** — memory limits, streaming, audio, video, controls, and asset loading need PSP-specific tuning.
6. **Incremental tests** — test GU primitives and transforms, then renderer state/texture features, then initialize engine systems, then load a scene, then verify gameplay in PPSSPP and real hardware.

## Renderer probe milestone (in progress)

The previous green/red screen test has been replaced with a standalone PSP GU test that submits a six-colour cube as triangles, configures projection/model transforms with GUM, enables depth testing, swaps buffers, and uses LEFT/RIGHT to adjust rotation. This can expose low-level renderer setup problems, but it is not connected to any Vice City engine code. The latest source change has triggered a GitHub Actions build; verify that run before claiming the updated probe compiles.

Build workflow: https://github.com/Frolicworks1/Gtavcportpsp/actions/workflows/psp-platform-smoke.yml

## Immediate blocker

The current repository does not have the complete engine or a PSP-compatible renderer integrated. Next, the renderer needs a feature-tested GU backend, followed by a pinned, license-reviewed engine integration. The probe must remain clearly labeled as not-game.

## Acceptance criteria for calling it playable

Only call a build playable after a real Vice City scene loads from legally supplied game data, controls work, and it has been tested in PPSSPP with logs/screenshots. A successful compile or blank-screen boot is not sufficient.

## Additional engine-source feasibility check

A second source candidate, `mrxenginner/reVC` (Miami branch), documents Windows, Android, Linux, macOS, and FreeBSD targets with desktop/OpenGL-family librw backends; its README does not claim PSP support. It requires the owner's Vice City game assets. The PSP MIPS build, GU renderer backend, audio backend, controller/platform layer, memory budget, and asset streaming remain engineering work. Review its published MIT-PoU terms and upstream component licenses before incorporating source.

Reference: https://github.com/mrxenginner/reVC
