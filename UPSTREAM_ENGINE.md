# Upstream engine source dependency

## Candidate engine

- Repository: https://github.com/re3fork/re3-miami-vita
- Purpose: reverse-engineered Vice City engine source adapted for PlayStation Vita.
- It is **not** a PSP build and must not be copied into a PSP EBOOT unchanged.
- The audit workflow currently clones the repository at its default branch head. Before shipping or redistributing source, pin an exact commit and review the upstream license and all submodule/dependency licenses.

## Confirmed platform assumptions from the audit

- Compiler prefix: `arm-vita-eabi`
- Build defines include `PSP2`, `LIBRW`, `RW_GL3`, `AUDIO_OAL`, `LIBRW_GLAD`
- Build outputs Vita `.vpk` / `.velf`, not a PSP `EBOOT.PBP`
- Vita-only link dependencies include `vita2d`, `vitagl`, multiple `Sce*_stub` libraries, and Vita audio/controller APIs.
- Platform compatibility code includes `src/core/psp2_compat.h` and `src/core/psp2_compat.c`; those names and APIs are Vita-specific despite the substring “psp”.

## PSP integration rules

1. Do not define `PSP2` for a PSP build. It selects Vita headers and code.
2. Use PSP SDK headers and MIPS Allegrex compiler/linker.
3. Implement a distinct PSP platform layer; do not alias Vita system calls to PSP functions by name.
4. Replace or port the renderer backend; Vita GL/GXM libraries are unavailable on PSP.
5. Keep proprietary GTA data out of this repository. The user must supply game data from a legally owned copy.
6. Do not publish an artifact as playable until an actual game scene loads and controls/audio are tested.

## Next gate

A real engine port should not be attempted as a single global compiler-flag change. First make the engine's startup/platform dependencies explicit, then port the PSP system layer, then tackle the renderer and resource streaming. The current WIP diagnostic EBOOT is not part of the engine dependency.
