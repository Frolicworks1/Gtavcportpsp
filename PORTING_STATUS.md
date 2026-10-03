# GTA Vice City PSP Port — Engineering Status

## Current state (2026-10-03)

This repository does **not** contain a bootable Vice City engine yet. The small WIP archive currently in the repo is a graphics diagnostic, not the game. Do not label its EBOOT as a GTA Vice City build.

An upstream Vice City engine source was audited from `re3fork/re3-miami-vita` and the audit workflow now pins revision `5de9de0d8072c3644f6cb961160b0fd7bfb067fb` for repeatability. The audit found about 460 C/C++/header source files, but its build targets **PS Vita (ARM)** and links Vita-specific libraries (`vita2d`, `vitagl`, `Sce*_stub`, SDL2/OpenAL). No top-level license/copying file was found by the shallow license-file scan, so license and dependency terms need review before copying source into this repository. PSP is a different platform (MIPS Allegrex); a Vita EBOOT/VPK cannot be used as a PSP executable.

## Porting work required

1. **Bring engine source in reproducibly** — pin an upstream commit and record its license/submodule dependencies; keep upstream separate from PSP-specific changes.
2. **Create a PSP platform layer** — PSP SDK startup, callbacks/exit handling, controller input, audio output, filesystem paths, timing, and logging.
3. **Port the renderer dependency** — the Vita renderer is not a PSP renderer. A PSP-compatible RenderWare/librw backend or substantial renderer rewrite is required; map textures, state changes, geometry, and frame buffers to GU.
4. **Port the build system** — use the PSP MIPS toolchain and PSP SDK libraries, not `arm-vita-eabi` or Vita system stubs.
5. **Adapt game systems** — memory limits, streaming, audio, video, controls, and asset loading need PSP-specific tuning.
6. **Incremental tests** — first compile a PSP-native platform smoke test, then initialize engine systems, then load a scene, then verify gameplay in PPSSPP and real hardware.

## Completed milestone: PSP toolchain smoke-test build

A separate PSP-native platform foundation now compiles in GitHub Actions with PSPSDK. The workflow verifies that both `EBOOT.PBP` and the ELF are non-empty and uploads them as the `vc-psp-platform-foundation-not-game` artifact. This program initializes PSP graphics and toggles the screen colour when START is pressed; it is a toolchain/platform smoke test, not Vice City. It intentionally does not exit on START while the exit crash is unverified. The green/red display confirms only the diagnostic graphics and button-input path.

Build workflow: https://github.com/Frolicworks1/Gtavcportpsp/actions/runs/37142675706

## Immediate blocker

The current repository does not have the complete engine or a PSP-compatible renderer integrated. The next meaningful milestone is a license-reviewed engine integration plan and PSP renderer/platform port, not another diagnostic cube or a renamed EBOOT.

## Acceptance criteria for calling it playable

Only call a build playable after a real Vice City scene loads from legally supplied game data, controls work, and it has been tested in PPSSPP with logs/screenshots. A successful compile or blank-screen boot is not sufficient.


## Additional engine-source feasibility check

A second source candidate, `mrxenginner/reVC` (Miami branch), documents working targets for Windows, Android, Linux, macOS, and FreeBSD, with rendering through desktop/OpenGL-family librw backends; its README does not claim PSP support. It also requires the owner's Vice City game assets. This is a more general engine reference than the Vita-only fork, but it is not a drop-in PSP port: the PSP MIPS build, GU renderer backend, audio backend, controller/platform layer, memory budget, and asset streaming still need engineering work. Its published MIT-PoU terms and upstream component licenses must be reviewed before any source is incorporated.

Reference: https://github.com/mrxenginner/reVC
